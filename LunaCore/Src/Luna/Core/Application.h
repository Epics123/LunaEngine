#pragma once

#include "Core.h"

namespace Luna
{
	class LUNA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication(); // To be defined in client
}