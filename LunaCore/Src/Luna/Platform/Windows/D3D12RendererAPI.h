#pragma once

#include "Luna/Renderer/RendererAPI.h"

#include "D3D12Helpers.h"

namespace Luna
{
	using Microsoft::WRL::ComPtr;

	class D3D12RendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void Clear() override;

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;

#ifdef LU_DEBUG
		virtual void PollDebugMessages() override;

		void SetupDebugCallback();
#endif // LU_DEBUG

	private:
		// Tries to acquire the first available hardware adapter that supports D3D12. If no adapter can be found, ppOutAdapter will be nullptr
		void GetHardwareAdapter(IDXGIFactory1* pInFactory, IDXGIAdapter1** ppOutAdapter, bool bRequestHighPerfAdapter = false);

		void CreateDevice(ComPtr<IDXGIFactory7> Factory);

		void CreateCommandQueue();

		void CreateSwapChain(ComPtr<IDXGIFactory7> Factory);

		void CreateDescriptorHeaps();

		void CreateRenderTargetViews();

		void CreateSyncronizationObjects();

#ifdef LU_DEBUG
		static void CALLBACK DebugMsgCallback(
			D3D12_MESSAGE_CATEGORY Category,
			D3D12_MESSAGE_SEVERITY Severity,
			D3D12_MESSAGE_ID ID,
			LPCSTR pDescription,
			void* pContext);
#endif // LU_DEBUG


	private:
		ComPtr<ID3D12Device> mDevice; // TODO: Query and use most recent ID3D12Device
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<IDXGISwapChain4> mSwapChain;
		ComPtr<ID3D12CommandAllocator> mCommandAllocator;
		ComPtr<ID3D12GraphicsCommandList> mCommandList;

		ComPtr<ID3D12DescriptorHeap> mRTVHeap;
		uint32_t mRTVDescriptorSize;
		std::vector<ComPtr<ID3D12Resource>> mRenderTargets;

		HANDLE mFenceEvent;
		ComPtr<ID3D12Fence> mFence;
		uint64_t mFenceValue;
	};
}