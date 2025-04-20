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

		virtual void Clear() override;

		virtual void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height) override;

	private:
		// Tries to acquire the first available hardware adapter that supports D3D12. If no adapter can be found, ppOutAdapter will be nullptr
		void GetHardwareAdapter(IDXGIFactory1* pInFactory, IDXGIAdapter1** ppOutAdapter, bool bRequestHighPerfAdapter = false);

		void CreateDevice(ComPtr<IDXGIFactory7> Factory);

		void CreateCommandQueue();

		void CreateSwapChain(ComPtr<IDXGIFactory7> Factory);

	private:
		ComPtr<ID3D12Device14> mDevice;
		ComPtr<ID3D12CommandQueue> mCommandQueue;
		ComPtr<IDXGISwapChain4> mSwapChain;
	};
}