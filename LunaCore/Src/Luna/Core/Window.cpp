#include "lupch.h"
#include "Window.h"

#ifdef LU_PLATFORM_WINDOWS
#include "Luna/Platform/Windows/WinWindow.h"
#endif // LU_PLATFORM_WINDOWS


namespace Luna
{
	UniquePtr<Window> Window::Create(const WindowProperties& Props /*= WindowProperties()*/)
	{
#ifdef LU_PLATFORM_WINDOWS
		return MakeUnique<WinWindow>(Props);
#else
		assert(false, "Unknown platform!");
		return nullptr;
#endif // LU_PLATFORM_WINDOWS
	}

}