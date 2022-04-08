#include "Logger.h"
#include "Teardown/Teardown.h"
#include "Launcher.h"
#include "Globals.h"

#include <spdlog/async_logger.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <filesystem>
#include <iomanip>
#include <sstream>

Ref<spdlog::logger> Logger::m_Logger;

void Logger::Initialize() {
    // Create log directory if it doesn't exist

    // Set log path to g_LauncherPath/logs
    std::string sLogPath = Launcher::sPath + std::string("\\Logs");

    if (!std::filesystem::exists(sLogPath)) {
        std::filesystem::create_directory(sLogPath);
    } else {
        // Clear log directory if the filecount >= 6
        int fileCount = 0;

        for (auto& p : std::filesystem::directory_iterator(sLogPath)) {
            fileCount++;
            if (fileCount >= 6) {
                std::filesystem::remove_all(sLogPath);
                break;
            }
        }
    }

    // Get date and time for filename
    auto time = std::time(nullptr);
    auto timeTm = *std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(&timeTm, "%Y-%m-%d_%H-%M-%S");

    auto sFilename = sLogPath + "\\" + ss.str() + ".log";

    // Set the sinks
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt >();
    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(sFilename, true);

#ifdef _DEBUG
    consoleSink->set_level(spdlog::level::debug);
#else
    consoleSink->set_level(spdlog::level::info);
#endif

    consoleSink->set_pattern("[%X] [%^%l%$]: %v");

    fileSink->set_level(spdlog::level::info);
    fileSink->set_pattern("[%X] [%l]: %v");

    std::vector<spdlog::sink_ptr> sinks {consoleSink, fileSink};
    m_Logger = std::make_shared<spdlog::logger>("TDMLogger", sinks.begin(), sinks.end());
#ifdef _DEBUG
    m_Logger->flush_on(spdlog::level::debug);
    m_Logger->set_level(spdlog::level::debug);
#else
    m_Logger->flush_on(spdlog::level::info);
    m_Logger->set_level(spdlog::level::info);
#endif
}