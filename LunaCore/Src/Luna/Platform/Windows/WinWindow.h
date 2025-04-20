#pragma once

#include "Luna/Core/Window.h"

#include <Windows.h>

namespace Luna
{
	class WinWindow : public Window
	{
	public:
		WinWindow(const WindowProperties& WindowProps);
		virtual ~WinWindow();

		void OnUpdate() override;

		uint32_t GetWidth() const override { return Width; }
		uint32_t GetHeight() const override { return Height; }
		const std::string& GetTitle() { return Title; }

		void* GetNativeWindow() const override { return nullptr; }

		static HWND GetActiveWindowHandle() { return sWindowHandle; }

	private:
		void Init(const WindowProperties& WindowProps);

		static LRESULT CALLBACK WindowProc(HWND hWnd, uint32_t Message, WPARAM wParam, LPARAM lParam);

	private:
		uint32_t Width;
		uint32_t Height;
		std::string Title;

		static HWND sWindowHandle;
	};
}