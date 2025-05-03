#include "SandboxLayer.h"
#include "Luna/Renderer/RenderCommand.h"

SandboxLayer::SandboxLayer()
	:Luna::Layer("Sandbox")
{

}

void SandboxLayer::OnAttach()
{

}

void SandboxLayer::OnDetach()
{

}

void SandboxLayer::OnUpdate()
{
#ifdef LU_DEBUG
	Luna::RenderCommand::PollDebugMessages();
#endif
}
