#pragma once

#include <cstdint>
#include <string_view>
#include <source_location>
#include <format>
#include <utility>

namespace impulse::log
{
	enum class Level : uint8_t
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

	bool enabled(Level lvl, std::string_view category) noexcept;

	// Non-templated entrypoint
	void write(Level lvl, std::string_view cat, std::string_view msg, std::source_location where = std::source_location::current()) noexcept;

	// Templated convenience func with comp-time format checking
	template<Level L, typename... Args>
	inline void logf(std::string_view cat, std::format_string<Args...> f, Args&&... a, std::source_location w = std::source_location::current())
	{
		if (!enabled(L, cat)) return;
		write(L, cat, std::format(f, std::forward<Args>(a)...), w);
	}

	template<typename... Args>
	inline void info(std::string_view cat, std::format_string<Args...> f, Args&&... a, std::source_location w = std::source_location::current())
	{
		logf(Level::Info, cat, f, std::forward<Args>(a)..., w);
	}
}
