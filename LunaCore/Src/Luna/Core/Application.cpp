#include "lupch.h"
#include "Application.h"

#include "Luna/Renderer/Renderer.h"

namespace Luna
{

	Application::Application()
	{
		mWindow = Window::Create(WindowProperties());
		RenderCommand::SetViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight()); // Probably temp for now

		Renderer::Init();
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (true)
		{

		}
	}

}