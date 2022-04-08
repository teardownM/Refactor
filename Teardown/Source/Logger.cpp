#include "Logger.h"

#include <spdlog/async_logger.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/rotating_file_sink.h"

Ref<spdlog::logger> Logger::m_Logger;

void Logger::Initialize() {
    spdlog::init_thread_pool(8192, 1);
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("TDMLogs/Log.txt", 1024*1024*10, 3);

#ifdef _DEBUG
    consoleSink->set_level(spdlog::level::debug);
#else
    consoleSink->set_level(spdlog::level::info);
#endif

    fileSink->set_level(spdlog::level::info);

    std::vector<spdlog::sink_ptr> sinks {consoleSink, fileSink};
    m_Logger = std::make_shared<spdlog::async_logger>("TDM", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

#ifdef _DEBUG
    m_Logger->flush_on(spdlog::level::debug);
    m_Logger->set_level(spdlog::level::debug);
#else
    m_Logger->flush_on(spdlog::level::info);
    m_Logger->set_level(spdlog::level::info);
#endif
}