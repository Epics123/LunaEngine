#pragma once

#include "Luna/Core/Log.h"

// D3D12 includes
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>

#include "d3dx12.h"
//

#include <wrl.h>
#include <comdef.h>

using Microsoft::WRL::ComPtr;

namespace Luna
{
	inline std::string WCharToStr(const WCHAR* wstr)
	{
		int Size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
		std::string Str(Size, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &Str[0], Size, nullptr, nullptr);
		return Str;
	}

	inline std::wstring StrToWChar(const std::string& Str)
	{
		int Size = MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, nullptr, 0);
		std::wstring WStr(Size, 0);
		MultiByteToWideChar(CP_UTF8, 0, Str.c_str(), -1, &WStr[0], Size);
		return WStr;
	}

	inline std::string HrToString(HRESULT hr)
	{
		_com_error err(hr);
		LPCTSTR errMsg = err.ErrorMessage();
		return WCharToStr(errMsg);
	}

	inline void ThrowIfFailed(HRESULT hr)
	{
		if(FAILED(hr))
		{
			LU_CORE_FATAL("HRESULT Failed: {0}", HrToString(hr));
		}
	}

	inline bool ValidateAdapterEnumeration(uint32_t AdapterIndex, bool bRequestHighPerformance, const ComPtr<IDXGIFactory7> Factory, IDXGIAdapter1** ppAdapter)
	{
		return SUCCEEDED(Factory->EnumAdapterByGpuPreference(
							AdapterIndex, 
							bRequestHighPerformance ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
							IID_PPV_ARGS(ppAdapter)));
	}
}