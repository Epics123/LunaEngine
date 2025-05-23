#include "lupch.h"

#include "D3D12RendererAPI.h"
#include "WinWindow.h"

#ifdef LU_DEBUG
#include <dxgidebug.h>
#endif

#include <glm/gtc/type_ptr.hpp>

namespace Luna
{
	void D3D12RendererAPI::Init()
	{
		uint32_t DxgiFactoryFlags = 0;

#if defined LU_DEBUG
		{
			ComPtr<ID3D12Debug6> DebugController;
			// IID_PPV_ARGS used to guarantee correct interface id (IID) and helps prevent casting errors
			// See: https://learn.microsoft.com/en-us/windows/win32/learnwin32/com-coding-practices for more info
			if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
			{
				DebugController->EnableDebugLayer();

				// Enable additional debug layer
				DxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

				LU_CORE_INFO("D3D12 Debug Layer enabled.");
			}
		}
#endif
		ComPtr<IDXGIFactory7> Factory;
		ThrowIfFailed(CreateDXGIFactory2(DxgiFactoryFlags, IID_PPV_ARGS(&Factory)));
		
		CreateDevice(Factory);

#ifdef LU_DEBUG
		SetupDebugCallback();
#endif // LU_DEBUG

		CreateCommandQueue();
		CreateSwapChain(Factory);
		CreateDescriptorHeaps();
		CreateRenderTargetViews();

		ThrowIfFailed(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocators[mFrameIndex].Get(), nullptr, IID_PPV_ARGS(&mCommandList)));

		// Command lists are created in the recording state, but there is nothing to record yet.
		// The main loop expects it to be closed, so close it now.
		ThrowIfFailed(mCommandList->Close());

		CreateSyncronizationObjects();
	}

	void D3D12RendererAPI::Shutdown()
	{
		WaitForFences();

		CloseHandle(mFenceEvent);

		ComPtr<IDXGIDebug1> DxgiDebug;
		if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&DxgiDebug))))
		{
			DxgiDebug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
		}
	}

	void D3D12RendererAPI::BeginFrame()
	{
		ThrowIfFailed(mCommandAllocators[mFrameIndex]->Reset());
		ThrowIfFailed(mCommandList->Reset(mCommandAllocators[mFrameIndex].Get(), mPipelineState.Get()));

		// Indicate that the back buffer will be used as a render target
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
	}

	void D3D12RendererAPI::EndFrame()
	{
		// Indicate that the back buffer will now be used to present
		mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
		ThrowIfFailed(mCommandList->Close());

		ExecuteCommandLists();
		MoveToNextFrame();
	}

	void D3D12RendererAPI::Clear()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE RtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVDescriptorSize);
		mCommandList->OMSetRenderTargets(1, &RtvHandle, FALSE, nullptr);

		mCommandList->ClearRenderTargetView(RtvHandle, glm::value_ptr(mClearColor), 0, nullptr);
	}

	void D3D12RendererAPI::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		mWidth = Width;
		mHeight = Height;
	}

#ifdef LU_DEBUG
	void D3D12RendererAPI::PollDebugMessages()
	{
		ComPtr<ID3D12InfoQueue> InfoQueue;
		if(FAILED(mDevice->QueryInterface(IID_PPV_ARGS(&InfoQueue))))
		{
			return;
		}

		const uint64_t NumMsgs = InfoQueue->GetNumStoredMessages();
		for(uint64_t i = 0; i < NumMsgs; i++)
		{
			size_t MsgLen = 0;
			InfoQueue->GetMessage(i, nullptr, &MsgLen); // Get required size

			D3D12_MESSAGE* Msg = (D3D12_MESSAGE*)malloc(MsgLen);
			if(Msg)
			{
				InfoQueue->GetMessage(i, Msg, &MsgLen); // Get the message

				switch (Msg->Severity)
				{
				case D3D12_MESSAGE_SEVERITY_CORRUPTION:
				case D3D12_MESSAGE_SEVERITY_ERROR:
					LU_CORE_ERROR(Msg->pDescription);
					break;
				case D3D12_MESSAGE_SEVERITY_WARNING:
					LU_CORE_WARN(Msg->pDescription);
					break;
				default:
					LU_CORE_INFO(Msg->pDescription);
					break;
				}
				
				free(Msg);
			}
		}

		InfoQueue->ClearStoredMessages();
	}

	void D3D12RendererAPI::SetupDebugCallback()
	{
		ComPtr<ID3D12InfoQueue1> InfoQueue;
		if(SUCCEEDED(mDevice->QueryInterface(IID_PPV_ARGS(&InfoQueue))))
		{
			DWORD CallbackCookie = 0;
			InfoQueue->RegisterMessageCallback(DebugMsgCallback, D3D12_MESSAGE_CALLBACK_FLAG_NONE, nullptr, &CallbackCookie);
		}
	}

#endif

	void D3D12RendererAPI::GetHardwareAdapter(IDXGIFactory1* pInFactory, IDXGIAdapter1** ppOutAdapter, bool bRequestHighPerfAdapter)
	{
		*ppOutAdapter = nullptr;

		LU_CORE_INFO("Available GPUs:");

		ComPtr<IDXGIAdapter1> TmpAdapter; // Temp adapter to query adapter info
		ComPtr<IDXGIAdapter4> Adapter;

		ComPtr<IDXGIFactory7> Factory;
		if(SUCCEEDED(pInFactory->QueryInterface(IID_PPV_ARGS(&Factory)))) // Ensure we are using an IDXGIFactory7
		{
			for(uint32_t AdapterIndex = 0; ValidateAdapterEnumeration(AdapterIndex, bRequestHighPerfAdapter, Factory, &TmpAdapter); AdapterIndex++)
			{
				if(SUCCEEDED(TmpAdapter->QueryInterface(IID_PPV_ARGS(&Adapter)))) // Use TmpAdapter to upgrade from IDXGIAdapter1 to IDXGIAdapter4
				{
					DXGI_ADAPTER_DESC3 Desc;
					Adapter->GetDesc3(&Desc);

					if(Desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
					{
						// Skip software adapters
						continue;
					}

					// Check to see whether the adapter supports D3D12, but don't create the actual device yet
					if(SUCCEEDED(D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_2, _uuidof(ID3D12Device), nullptr)))
					{
						LU_CORE_INFO("Adapter {0} - {1}", AdapterIndex, WCharToStr(Desc.Description))
						break;
					}
				}
			}
		}

		// Fallback to system-defined adapter priority if adapter still hasn't been found by this point
		if(Adapter.Get() == nullptr)
		{
			for(uint32_t AdapterIndex = 0; SUCCEEDED(pInFactory->EnumAdapters1(AdapterIndex, &TmpAdapter)); AdapterIndex++)
			{
				if (SUCCEEDED(TmpAdapter->QueryInterface(IID_PPV_ARGS(&Adapter))))
				{
					DXGI_ADAPTER_DESC3 Desc;
					Adapter->GetDesc3(&Desc);

					if (Desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
					{
						// Skip software adapters
						continue;
					}

					// Check to see whether the adapter supports D3D12, but don't create the actual device yet
					if (SUCCEEDED(D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_12_2, _uuidof(ID3D12Device), nullptr)))
					{
						LU_CORE_INFO("Adapter {0} - {1}", AdapterIndex, WCharToStr(Desc.Description))
						break;
					}
				}
			}
		}

		*ppOutAdapter = Adapter.Detach();
	}

	void D3D12RendererAPI::CreateDevice(ComPtr<IDXGIFactory7> Factory)
	{
		ComPtr<IDXGIAdapter1> HardwareAdapter;
		GetHardwareAdapter(Factory.Get(), &HardwareAdapter);

		ThrowIfFailed(D3D12CreateDevice(HardwareAdapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&mDevice)));
	}

	void D3D12RendererAPI::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};
		CmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		CmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		ThrowIfFailed(mDevice->CreateCommandQueue(&CmdQueueDesc, IID_PPV_ARGS(&mCommandQueue)));
	}

	void D3D12RendererAPI::CreateSwapChain(ComPtr<IDXGIFactory7> Factory)
	{
		DXGI_SWAP_CHAIN_DESC1 SwapChainDesc = {};
		SwapChainDesc.BufferCount = sBufferCount;
		SwapChainDesc.Width = mWidth;
		SwapChainDesc.Height = mHeight;
		SwapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		SwapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> SwapChain;
		ThrowIfFailed(Factory->CreateSwapChainForHwnd(mCommandQueue.Get(), 
														WinWindow::GetActiveWindowHandle(), 
														&SwapChainDesc, 
														nullptr, // TODO: Fullscreen support
														nullptr,
														&SwapChain));

		// No fullscreen transitions. TODO: Fullscreen support
		ThrowIfFailed(Factory->MakeWindowAssociation(WinWindow::GetActiveWindowHandle(), DXGI_MWA_NO_ALT_ENTER));

		ThrowIfFailed(SwapChain.As(&mSwapChain));
		mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
	}

	void D3D12RendererAPI::CreateDescriptorHeaps()
	{
		D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
		RTVHeapDesc.NumDescriptors = sBufferCount;
		RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		RTVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(mDevice->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&mRTVHeap)));

		mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	void D3D12RendererAPI::CreateRenderTargetViews()
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE RTVHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

		mRenderTargets.resize(sBufferCount);
		mCommandAllocators.resize(sBufferCount);

		for(uint32_t i = 0; i < static_cast<uint32_t>(mRenderTargets.size()); i++)
		{
			ThrowIfFailed(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i])));
			mDevice->CreateRenderTargetView(mRenderTargets[i].Get(), nullptr, RTVHandle);
			RTVHandle.Offset(1, mRTVDescriptorSize);

			ThrowIfFailed(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocators[i])));
		}
	}

	void D3D12RendererAPI::CreateSyncronizationObjects()
	{
		mFenceValues.resize(sBufferCount);
		ThrowIfFailed(mDevice->CreateFence(mFenceValues[mFrameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));
		mFenceValues[mFrameIndex]++;

		mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		if(mFenceEvent == nullptr)
		{
			ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
		}

		WaitForFences();
	}

	void D3D12RendererAPI::ExecuteCommandLists()
	{
		std::vector<ID3D12CommandList*> CmdLists = { mCommandList.Get() };
		mCommandQueue->ExecuteCommandLists(static_cast<uint32_t>(CmdLists.size()), CmdLists.data());

		ThrowIfFailed(mSwapChain->Present(1, 0));
	}

	void D3D12RendererAPI::WaitForFences()
	{
		ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), mFenceValues[mFrameIndex]));

		// Wait until fence is processed
		ThrowIfFailed(mFence->SetEventOnCompletion(mFenceValues[mFrameIndex], mFenceEvent));
		WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

		mFenceValues[mFrameIndex]++;
	}

	void D3D12RendererAPI::MoveToNextFrame()
	{
		const uint64_t CurrentFenceValue = mFenceValues[mFrameIndex];
		ThrowIfFailed(mCommandQueue->Signal(mFence.Get(), CurrentFenceValue));

		mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

		if(mFence->GetCompletedValue() < mFenceValues[mFrameIndex])
		{
			ThrowIfFailed(mFence->SetEventOnCompletion(mFenceValues[mFrameIndex], mFenceEvent));
			WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
		}

		// Set the fence value for the next frame.
		mFenceValues[mFrameIndex] = CurrentFenceValue + 1;
	}

#ifdef LU_DEBUG
	void CALLBACK D3D12RendererAPI::DebugMsgCallback(D3D12_MESSAGE_CATEGORY Category, D3D12_MESSAGE_SEVERITY Severity, D3D12_MESSAGE_ID ID, LPCSTR pDescription, void* pContext)
	{
		switch (Severity)
		{
		case D3D12_MESSAGE_SEVERITY_CORRUPTION:
		case D3D12_MESSAGE_SEVERITY_ERROR:
			LU_CORE_ERROR(pDescription);
			break;
		case D3D12_MESSAGE_SEVERITY_WARNING:
			LU_CORE_WARN(pDescription);
			break;
		default:
			LU_CORE_INFO(pDescription);
			break;
		}
	}
#endif // LU_DEBUG
}