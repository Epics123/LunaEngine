#include "lupch.h"

#include "D3D12RendererAPI.h"
#include "WinWindow.h"

namespace Luna
{
	void D3D12RendererAPI::Init()
	{
		uint32_t DxgiFactoryFlags = 0;

#if defined(_DEBUG)
		{
			ComPtr<ID3D12Debug6> DebugController;
			// IID_PPV_ARGS used to guarantee correct interface id (IID) and helps prevent casting errors
			// See: https://learn.microsoft.com/en-us/windows/win32/learnwin32/com-coding-practices for more info
			if(SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController))))
			{
				DebugController->EnableDebugLayer();

				// Enable additional debug layer
				DxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
			}
		}
#endif
		ComPtr<IDXGIFactory7> Factory;
		ThrowIfFailed(CreateDXGIFactory2(DxgiFactoryFlags, IID_PPV_ARGS(&Factory)));
		
		CreateDevice(Factory);
		CreateCommandQueue();
		CreateSwapChain(Factory);
	}

	void D3D12RendererAPI::Clear()
	{

	}

	void D3D12RendererAPI::SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
	{
		mWidth = Width;
		mHeight = Height;
	}

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
		SwapChainDesc.BufferCount = mBufferCount;
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

}