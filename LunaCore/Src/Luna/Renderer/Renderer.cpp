#include "lupch.h"

#include "Renderer.h"

namespace Luna
{
	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::Shutdown()
	{
		RenderCommand::Shutdown();
	}

#ifdef LU_DEBUG
	void Renderer::PollDebugMessages()
	{
		RenderCommand::PollDebugMessages();
	}
#endif
}