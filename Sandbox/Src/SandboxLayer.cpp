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
	Luna::RenderCommand::BeginFrame();
	Luna::RenderCommand::Clear();
	Luna::RenderCommand::EndFrame();
}
