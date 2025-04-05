#include "lupch.h"
#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Luna
{
	std::shared_ptr<spdlog::logger> Log::sCoreLogger;
	std::shared_ptr<spdlog::logger> Log::sClientLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$"); // see https://github.com/gabime/spdlog/wiki/3.-Custom-formatting for more custom formatting

		sCoreLogger = spdlog::stdout_color_mt("LUNA");
		sCoreLogger->set_level(spdlog::level::trace);
		
		sClientLogger = spdlog::stdout_color_mt("APP");
		sClientLogger->set_level(spdlog::level::trace);
	}

}