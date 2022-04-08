#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>

Ref<spdlog::logger> Logger::m_Logger;

void Logger::Initialize() {
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

	m_Logger = std::make_shared<spdlog::logger>("TDM", begin(logSinks), end(logSinks));
	spdlog::register_logger(m_Logger);

#ifdef DEBUG
    m_Logger->set_level(spdlog::level::debug);
#else
    m_Logger->set_level(spdlog::level::info);
#endif
}