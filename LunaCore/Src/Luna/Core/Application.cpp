#include "lupch.h"
#include "Application.h"

#include "Luna/Renderer/Renderer.h"

namespace Luna
{

	Application::Application()
	{
		mWindow = Window::Create(WindowProperties());
		mWindow->SetEventCallback(LU_BIND_EVENT_FN(Application::OnWindowClosed)); // Tmp for closing window - TODO: proper event system
		RenderCommand::SetViewport(0, 0, mWindow->GetWidth(), mWindow->GetHeight()); // Probably temp for now

		Renderer::Init();
	}

	Application::~Application()
	{
		Renderer::Shutdown();
	}

	void Application::Run()
	{
		// TODO: Application layers, D3D12 debug output to console

		while (mRunning)
		{
			mWindow->OnUpdate();
		}
	}

	void Application::Close()
	{
		mRunning = false;
	}

	void Application::ProcessEvents()
	{
		
	}

	bool Application::OnWindowClosed()
	{
		mRunning = false;
		return true;
	}

}