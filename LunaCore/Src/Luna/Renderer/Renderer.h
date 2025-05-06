#pragma once

#include "RenderCommand.h"

namespace Luna
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}