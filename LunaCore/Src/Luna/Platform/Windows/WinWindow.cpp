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
									this // data passed to WM_CREATE
									);

		ShowWindowAsync(sWindowHandle, SW_SHOW);
	}

	LRESULT CALLBACK WinWindow::WindowProc(HWND hWnd, uint32_t Message, WPARAM wParam, LPARAM lParam)
	{
		WinWindow* pWindow = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (Message)
		{
		case WM_CREATE:
			{
			// Save off WinWindow* that was passed into CreateWindow so we can use it later.
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			}
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);
			pWindow->EventCallback(); // Super temporary until we have a proper event system
			return 0;
		}

		return DefWindowProc(hWnd, Message, wParam, lParam);
	}

	void WinWindow::OnUpdate()
	{
		MSG WindowMsg = {};
		if(PeekMessage(&WindowMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&WindowMsg);
			DispatchMessage(&WindowMsg);
		}
	}
}