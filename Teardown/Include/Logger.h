#pragma once

#include <spdlog/spdlog.h>
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

class Logger {
public:
    static void Initialize();
    static Ref<spdlog::logger>& GetLogger() { return m_Logger; }

    template<typename... Args>
    static void Error(fmt::format_string<Args...> fmt, Args &&...args) {
        #ifdef _DEBUG
                m_Logger->error(fmt, std::forward<Args>(args)...);
        #else
                auto msg = fmt::format(fmt, std::forward<Args>(args)...);
                MessageBoxA(nullptr, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
        #endif
    }

    template<typename... Args>
    static void Critical(fmt::format_string<Args...> fmt, Args &&...args) {
        #ifdef _DEBUG
                m_Logger->critical(fmt, std::forward<Args>(args)...);
        #else
                auto msg = fmt::format(fmt, std::forward<Args>(args)...);
                MessageBoxA(nullptr, msg.c_str(), "Fatal", MB_OK | MB_ICONERROR);
        #endif
    }
    
private:
    static Ref<spdlog::logger> m_Logger;
};

#define LOG_DEBUG(...) ::Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) ::Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Logger::Error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Logger::Critical(__VA_ARGS__)