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

	void Renderer::BeginFrame()
	{
		RenderCommand::BeginFrame();
	}

	void Renderer::EndFrame()
	{
		RenderCommand::EndFrame();
	}

}