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

#define LU_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "Luna/Core/Pointers.h"
#include "Luna/Core/Log.h"