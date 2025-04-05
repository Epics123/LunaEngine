#pragma once

#ifdef LU_PLATFORM_WINDOWS
	#ifdef LU_BUILD_DLL
		#define LUNA_API __declspec(dllexport)
	#else
		#define LUNA_API __declspec(dllimport)
	#endif
#else
	#error Luna only supports Windows!
#endif