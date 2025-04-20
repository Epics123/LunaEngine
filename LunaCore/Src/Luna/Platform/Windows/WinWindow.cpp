#include "lupch.h"
#include "WinWindow.h"

#include "Luna/Renderer/RendererAPI.h"

#include "Luna/Platform/Windows/D3D12Helpers.h"

namespace Luna
{
	HWND WinWindow::sWindowHandle;
	
	WinWindow::WinWindow(const WindowProperties& WindowProps)
		:Width(WindowProps.Width), Height(WindowProps.Height), Title(WindowProps.Name)
	{
		Init(WindowProps);
	}

	WinWindow::~WinWindow()
	{

	}

	void WinWindow::Init(const WindowProperties& WindowProps)
	{
		WNDCLASSEX WindowClass = {0};
		WindowClass.cbSize = sizeof(WNDCLASSEX);
		WindowClass.style = CS_HREDRAW | CS_VREDRAW; // See https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles for more window styles
		WindowClass.lpfnWndProc = WindowProc;
		WindowClass.hInstance = GetModuleHandle(nullptr);
		WindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WindowClass.lpszClassName = L"WindowsWindow";
		RegisterClassEx(&WindowClass);

		RECT WindowRect = { 0, 0, static_cast<LONG>(Width), static_cast<LONG>(Height) };
		AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

		sWindowHandle = CreateWindow(WindowClass.lpszClassName, 
									StrToWChar(Title).c_str(), 
									WS_OVERLAPPEDWINDOW, 
									CW_USEDEFAULT, 
									CW_USEDEFAULT,
									WindowRect.right - WindowRect.left,
									WindowRect.bottom - WindowRect.top,
									nullptr, // No parent window
									nullptr, // No menus
									GetModuleHandle(nullptr),
									nullptr // data passed to WM_CREATE
									);


	}

	LRESULT CALLBACK WinWindow::WindowProc(HWND hWnd, uint32_t Message, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	void WinWindow::OnUpdate()
	{

	}
}