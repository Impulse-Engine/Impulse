#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

#if defined(__linux__)
    #include <cstdint>
#endif

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
        }
    }

    inline void logf(LogLevel lvl, std::string msg)
    {
        std::string current = getTimeNow();
        std::string lvl_text = getLevelAsString(lvl);

        if (lvl > LogLevel::WARN)
        {
            std::cerr << "[" << current << "] "
                    << "[" << lvl_text << "] "
                    << "[" << __func__  << "] "
                    << "[" << __FILE__ << "]: "
                    << msg;
        }
        else
        {
            std::cout << "[" << current << "] "
                    << "[" << lvl_text << "] "
                    << "[" << __func__  << "] "
                    << "[" << __FILE__ << "]: "
                    << msg;
        }
        
    };
}