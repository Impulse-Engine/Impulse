#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>

// Assertions like the two provided here are globally available and not tied
// to the namespace Impulse. They just are... use them carefully and remember
// to use soft_assert above plain assert where possible!

inline std::string getTimestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm tm{};

    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm, &now);
    #else
        localtime_r(&now, &tm);
    #endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

inline void assert_fail(const char* expr, const char* msg, const char* file, int line, const char* func, bool fatal)
{
    std::cerr << "[ASSERT " << (fatal ? "HARD" : "SOFT") << "] "
              << getTimestamp() << "\n"
              << "  Expr : " << expr << "\n"
              << "  Msg  : " << (msg ? msg : "(none)") << "\n"
              << "  File : " << file << ":" << line << "\n"
              << "  Func : " << func << "\n";

    if (fatal)
    {
        std::cerr << "  Action: abort()\n";
        std::abort();
    }
}

// A soft assertion will check the expression and push a stderr message if the expr fails.
// This will not stop execution however, if you need it to stop, use `hard_assert()`
#define soft_assert(expr, message) \
    ((expr) ? (void)0 : assert_fail(#expr, message, __FILE__, __LINE__, __func__, false))

// This is a hard assertion, if the expression fails, the entire process aborts.
// If you only want a soft check, use `soft_assert()`
#define hard_assert(expr, message) \
    ((expr) ? (void)0 : assert_fail(#expr, message, __FILE__, __LINE__, __func__, true))