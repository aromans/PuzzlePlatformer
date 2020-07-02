#pragma once

#include <memory.h>
#include "Core.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Engine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& CoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& ClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core Log Macros
#define ENG_CORE_TRACE(...)   ::Engine::Log::CoreLogger()->trace(__VA_ARGS__)
#define ENG_CORE_INFO(...)    ::Engine::Log::CoreLogger()->info(__VA_ARGS__)
#define ENG_CORE_WARN(...)    ::Engine::Log::CoreLogger()->warn(__VA_ARGS__)
#define ENG_CORE_ERROR(...)   ::Engine::Log::CoreLogger()->error(__VA_ARGS__)
#define ENG_CORE_FATAL(...)   ::Engine::Log::CoreLogger()->fatal(__VA_ARGS__)

// Client Log Macros
#define TRACE(...)        ::Engine::Log::ClientLogger()->trace(__VA_ARGS__)
#define INFO(...)         ::Engine::Log::ClientLogger()->info(__VA_ARGS__)
#define WARN(...)         ::Engine::Log::ClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)        ::Engine::Log::ClientLogger()->error(__VA_ARGS__)
#define FATAL(...)        ::Engine::Log::ClientLogger()->fatal(__VA_ARGS__)

