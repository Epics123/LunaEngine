#pragma once

#include "Luna/Renderer/RendererAPI.h"

namespace Luna
{
	class LUNA_API RenderCommand
	{
	public:
		static void Init()
		{
			sRendererAPI->Init();
		}

		static void Shutdown()
		{
			sRendererAPI->Shutdown();
		}

		static void Clear()
		{
			sRendererAPI->Clear();
		}

		static void SetViewport(uint32_t X, uint32_t Y, uint32_t Width, uint32_t Height)
		{
			sRendererAPI->SetViewport(X, Y, Width, Height);
		}

		static void BeginFrame()
		{
			sRendererAPI->BeginFrame();
		}

		static void EndFrame()
		{
			sRendererAPI->EndFrame();
		}

	private:
		static UniquePtr<RendererAPI> sRendererAPI;
	};
}