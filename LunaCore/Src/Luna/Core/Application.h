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

		Window& GetWindow() { return *mWindow; }

	private:
		UniquePtr<Window> mWindow;
	};

	std::unique_ptr<Application> CreateApplication(); // To be defined in client
}