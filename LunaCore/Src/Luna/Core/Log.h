#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Luna
{
	class LUNA_API Log
	{
	public:
		static void Init();

		inline static class std::shared_ptr<spdlog::logger>& GetCoreLogger() { return sCoreLogger; }
		inline static class std::shared_ptr<spdlog::logger>& GetClientLogger() { return sClientLogger; }

	private:
		static class std::shared_ptr<spdlog::logger> sCoreLogger;
		static class std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

// Core log macros
#define LU_CORE_TRACE(...) Luna::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define LU_CORE_INFO(...)  Luna::Log::GetCoreLogger()->info(__VA_ARGS__);
#define LU_CORE_WARN(...)  Luna::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define LU_CORE_ERROR(...) Luna::Log::GetCoreLogger()->error(__VA_ARGS__);
#define LU_CORE_FATAL(...) Luna::Log::GetCoreLogger()->critical(__VA_ARGS__); assert(false);

// Client log macros
#define LU_TRACE(...) Luna::Log::GetClientLogger()->trace(__VA_ARGS__);
#define LU_INFO(...)  Luna::Log::GetClientLogger()->info(__VA_ARGS__);
#define LU_WARN(...)  Luna::Log::GetClientLogger()->warn(__VA_ARGS__);
#define LU_ERROR(...) Luna::Log::GetClientLogger()->error(__VA_ARGS__);
#define LU_FATAL(...) Luna::Log::GetClientLogger()->critical(__VA_ARGS__); assert(false);