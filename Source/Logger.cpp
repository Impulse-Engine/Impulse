#include <Logger.hpp>

#include <atomic>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined(_WIN32)
	#define NOMINMAX
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

namespace impulse::log
{
	namespace
	{
		using clock_t = std::chrono::system_clock;

		struct Sink
		{
			std::ofstream file;
			bool file_enabled = false;
		};

		std::once_flag g_init_once;
		std::mutex g_mutex;

		// Global filter level
		std::atomic<Level> g_min_level{ Level::Debug };

		Sink g_sink;

		static constexpr const char *level_to_string(Level level) noexcept
		{
			switch (level)
			{
				case Level::Trace: return "TRACE";
				case Level::Debug: return "DEBUG";
				case Level::Info:  return "INFO";
				case Level::Warn:  return "WARN";
				case Level::Error: return "ERROR";
				default:           return "UNKNOWN";
			}
		}

		static int level_rank(Level level) noexcept
		{
			switch (level)
			{
				case Level::Trace: return 10;
				case Level::Debug: return 20;
				case Level::Info:  return 30;
				case Level::Warn:  return 40;
				case Level::Error: return 50;
				default:           return 0;
			}
		}

		static Level parse_level(const char* s) noexcept
		{
			if (!s || !*s) return Level::Info;

			auto eq = [](const char* a, const char* b)
			{
#if defined(_WIN32)
					return _stricmp(a, b) == 0;
#else
					return strcasecmp(a, b) == 0;
#endif
			};

			if (eq(s, "debug")) return Level::Debug;
			if (eq(s, "info"))  return Level::Info;
			if (eq(s, "warn") || eq(s, "warning")) return Level::Warn;
			if (eq(s, "error")) return Level::Error;
			if (eq(s, "fatal")) return Level::Fatal;

			return Level::Info;
		}

		static void init()
		{
			if (const char* lvl = std::getenv("IMPULSE_LOG_LEVEL"))
			{
				g_min_level.store(parse_level(lvl), std::memory_order_relaxed);
			}

			if (const char* path = std::getenv("IMPULSE_LOG_FILE"))
			{
				std::lock_guard<std::mutex> lock(g_mutex);
				g_sink.file.open(path, std::ios::out | std::ios::app);
				g_sink.file_enabled = g_sink.file.is_open();
			}
		}

		static void format_timestamp(char* out, size_t out_sz, clock_t::time_point tp)
		{
			using namespace std::chrono;
			const auto t = clock_t::to_time_t(tp);
			std::tm tm{};
#if defined(_WIN32)
			localtime_s(&tm, &t);
#else
			localtime_r(&t, &tm);
#endif
			const auto ms = duration_cast<milliseconds>(tp.time_since_epoch()) % 1000;

			std::snprintf(out, out_sz, "%02d:%02d:%02d.%03d",
				tm.tm_hour, tm.tm_min, tm.tm_sec, static_cast<int>(ms.count()));
		}

		static uint32_t thread_id_u32()
		{
			const auto id = std::this_thread::get_id();
			return static_cast<uint32_t>(std::hash<std::thread::id>{}(id));
		}

		static bool enabled_impl(Level lvl, std::string_view category)  noexcept
		{
			const Level global_min = g_min_level.load(std::memory_order_relaxed);
			if (level_rank(lvl) < level_rank(global_min)) return false;

			(void)category;
			return true;
		}

		static void write_console_line(const char* line, bool is_error_stream)
		{
#if defined(_WIN32)
			FILE* out = is_error_stream ? stderr : stdout;
			std::fputs(line, out);
			std::fflush(out);
#else
			FILE* out = is_error_stream ? stderr : stdout;
			std::fputs(line, out);
			std::fflush(out);
#endif
		}
	} // anon namespace

	bool enabled(Level lvl, std::string_view category) noexcept
	{
		return enabled_impl(lvl, category);
	}

	void write(Level lvl, std::string_view cat, std::string_view msg, std::source_location where) noexcept
	{
		std::call_once(g_init_once, init);
		char ts[32]{};
		format_timestamp(ts, sizeof(ts), clock_t::now());
		const uint32_t tid = thread_id_u32();

		const bool err_stream = (lvl == Level::Warn || lvl == Level::Error || lvl == Level::Fatal);

		// NOTE: We lock around output to avoid interleaved lines across threads.
		std::lock_guard<std::mutex> lock(g_mutex);

		// Compose the final line
		std::string line;
		line.reserve(msg.size() + 256);

		line.append(ts);
		line.append(" [");
		line.append(level_to_string(lvl));
		line.append("] [");
		line.append(cat.data(), cat.size());
		line.append("] (T:");
		line.append(std::to_string(tid));
		line.append(") ");

		line.append(msg.data(), msg.size());

		line.append(" (");
		line.append(where.file_name());
		line.append(":");
		line.append(std::to_string(where.line()));
		line.append(")\n");

		// Console sink
		write_console_line(line.c_str(), err_stream);

		if (g_sink.file_enabled)
		{
			g_sink.file << line;
			g_sink.file.flush();
		}
	}

} // namespace impulse::log