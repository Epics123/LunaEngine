#include "lupch.h"

#include "RendererAPI.h"

#include "Luna/Platform/Windows/D3D12RendererAPI.h"

#include <assert.h>


namespace Luna
{
	RendererAPI::API RendererAPI::sActiveAPI = RendererAPI::API::D3D12; // Default to DirectX12

	UniquePtr<RendererAPI> RendererAPI::Create()
	{
		switch (sActiveAPI)
		{
		case Luna::RendererAPI::API::None:
			assert(false, "API::None is not supported!");
			return nullptr;
			break;
		case Luna::RendererAPI::API::D3D12:
			return MakeUnique<D3D12RendererAPI>();
			break;
		case Luna::RendererAPI::API::Vulkan:
			assert(false, "Vulkan API is not currently supported!");
			return nullptr;
			break;
		}

		assert(false, "Unknown RendererAPI!");
		return nullptr;
	}
}