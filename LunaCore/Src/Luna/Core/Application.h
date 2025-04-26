#pragma once

#include "Core.h"
#include "Window.h"
#include "Common.h"

#include <memory>

namespace Luna
{
	class LUNA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void Close();

		Window& GetWindow() { return *mWindow; }

		// Temp for now until we have some sort of event system
		void ProcessEvents();

	private:
		bool OnWindowClosed();

	private:
		UniquePtr<Window> mWindow;

		bool mRunning = true;
	};

	std::unique_ptr<Application> CreateApplication(); // To be defined in client
}