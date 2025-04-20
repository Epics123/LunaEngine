#pragma once

#ifdef LU_DEBUG
	#if defined(LU_PLATFORM_WINDOWS)
		#define LU_DEBUG_BREAK() __debugbreak()
	#else
		#error "Platform does not support debugbreak yet!"
	#endif
#else
	#define LU_DEBUG_BREAK()
#endif // LU_DEBUG


#include "Luna/Core/Pointers.h"
#include "Luna/Core/Log.h"