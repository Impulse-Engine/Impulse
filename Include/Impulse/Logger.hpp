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
        std::tm tm = *std::localtime(&ts);
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

    inline void logf(LogLevel lvl, const std::string& msg, const char *file, const char *func)
    {
        std::string current = getTimeNow();
        std::string lvl_text = getLevelAsString(lvl);

        if (lvl > LogLevel::WARN)
        {
            std::cerr << "[" << current << "] "
                    << "[" << lvl_text << "] "
                    << "[" << func << "] "
                    << "[" << file << "]: "
                    << msg;
        }
        else
        {
            std::cout << "[" << current << "] "
                    << "[" << lvl_text << "] "
                    << "[" << func  << "] "
                    << "[" << file << "]: "
                    << msg;
        }
        
    };
}

#define LOG(lvl, msg) Impulse::Logger::logf(lvl, &msg, __FILE__, __func__)