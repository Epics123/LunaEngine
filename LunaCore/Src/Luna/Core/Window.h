#pragma once

#include "Pointers.h"
#include "Common.h"

namespace Luna
{
	struct WindowProperties
	{
		std::string Name;
		uint32_t Width;
		uint32_t Height;

		WindowProperties(const std::string& WindowName = "Luna Engine", uint32_t WindowWidth = 1280, uint32_t WindowHeight = 720)
			:Name(WindowName), Width(WindowWidth), Height(WindowHeight)
		{
		}
	};

	// Generic interface representing a desktop window.
	class Window
	{
	public:
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static UniquePtr<Window> Create(const WindowProperties& Props = WindowProperties());
	};
}