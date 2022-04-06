#pragma once

#include <spdlog/spdlog.h>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Logger {
public:
    static void Initialize();
    static Ref<spdlog::logger>& GetLogger() { return m_Logger; }
    
private:
    static Ref<spdlog::logger> m_Logger;
};

#define LOG_TRACE(...) ::Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...) ::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) ::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...) ::Logger::GetLogger()->fatal(__VA_ARGS__)