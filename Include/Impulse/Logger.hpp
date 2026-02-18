#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdint>

namespace Impulse::Logger
{
    enum class LogLevel : uint8_t
    {
        DEBUG = 0,
        INFO = 1,
        WARN = 2,
        ERROR = 3,
        FATAL = 4,
    };

    inline std::string getTimeNow()
    {
        std::time_t ts = std::time(nullptr);
        std::tm tm{};

        #ifdef _WIN32
            localtime_s(&tm, &ts);
        #else
            localtime_r(&ts, &tm);
        #endif

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

    inline std::string getLevelAsString(LogLevel l)
    {
        switch (l)
        {
            case LogLevel::DEBUG: return "Debug";
            case LogLevel::INFO: return "Info";
            case LogLevel::WARN: return "Warn";
            case LogLevel::ERROR: return "Error";
            case LogLevel::FATAL: return "Fatal";
            default: return "Unknown";
        }
    }

    inline void logf(LogLevel lvl, std::string_view msg, const char* file, const char* func)
    {
        std::ostringstream line;
        line << "[" << getTimeNow() << "] "
             << "[" << getLevelAsString(lvl) << "] "
             << "[" << func << "] "
             << "[" << file << "]: "
             << msg
             << '\n';

        if (lvl > LogLevel::WARN) std::cerr << line.str();
        else                     std::cout << line.str();
    }
}

#define LOG(lvl, msg) Impulse::Logger::logf(lvl, msg, __FILE__, __func__)