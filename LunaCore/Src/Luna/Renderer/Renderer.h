#pragma once

#include "RenderCommand.h"

namespace Luna
{
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

#ifdef LU_DEBUG
		// Poll debug messages for the active renderer api
		static void PollDebugMessages();
#endif

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}