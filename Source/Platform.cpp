#include <Impulse/Platform.hpp>
#include <Impulse/Assert.hpp>

namespace Impulse::Platform
{

    #if defined(_WIN32) || defined(_WIN64)
        #define WIN32_LEAN_AND_MEAN
        #include <Windows.h>

        PlatformInfo queryPlatformInfo_Impl()
        {

        }

    #endif
    #if defined(__linux__)
        PlatformInfo queryPlatformInfo_Impl()
        {

        }
    #endif
    #if defined(__APPLE__)
        #include <sys/types.h>
        #include <sys/sysctl.h>
        #include <cctype>
        #include <string>
        #include <vector>
        #include <algorithm>

        static inline void trimInPlace(std::string& s)
        {
            auto not_space = [](unsigned char c) { return !std::isspace(c); };
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), not_space));
            s.erase(std::find_if(s.rbegin(), s.rend(), not_space).base(), s.end());
        }

        static std::string sysctl_string(const char* name)
        {
            size_t size = 0;
            if (sysctlbyname(name, nullptr, &size, nullptr, 0) != 0 || size == 0)
                return {};

            std::string out;
            out.resize(size); // includes trailing '\0' typically
            if (sysctlbyname(name, out.data(), &size, nullptr, 0) != 0)
                return {};

            // Ensure null-termination...x`cut at first '\0'
            auto nul = out.find('\0');
            if (nul != std::string::npos) out.resize(nul);

            trimInPlace(out);
            return out;
        }

        template <typename T>
        static bool sysctl_value(const char* name, T& out)
        {
            size_t size = sizeof(T);
            return sysctlbyname(name, &out, &size, nullptr, 0) == 0 && size == sizeof(T);
        }

        PlatformInfo queryPlatformInfo_Impl()
        {  
            PlatformInfo info{};
            info.SysFamily = OSFamily::macOS;

            // Arch: compile-time is the most reliable.
            #if defined(__aarch64__) || defined(__arm64__) || defined(__ARM64__)
                info.arch = Arch::ARM64;
            #elif defined(__x86_64__) || defined(_M_X64)
                info.arch = Arch::X86_64;
            #elif defined(__i386__) || defined(_M_IX86)
                info.arch = Arch::X86;
            #elif defined(__arm__)
                info.arch = Arch::ARM;
            #else
                info.arch = Arch::Unknown;
            #endif

            // OS version: prefer product version, fall back to Darwin kernel release.
            info.OSVersion = sysctl_string("kern.osproductversion");
            if (info.OSVersion.empty())
                info.OSVersion = sysctl_string("kern.osrelease");

            // CPU name: prefer brand string, fall back to model identifier.
            info.CPUPrettyName = sysctl_string("machdep.cpu.brand_string");
            if (info.CPUPrettyName.empty())
                info.CPUPrettyName = sysctl_string("hw.model");

            // Logical cores, RAM, page size.
            {
                std::uint32_t logical = 0;
                if (sysctl_value("hw.logicalcpu", logical))
                    info.CPULogicalCores = logical;
            }
            {
                std::uint64_t mem = 0;
                if (sysctl_value("hw.memsize", mem))
                    info.TotalPhysicalMemBytes = mem;
            }
            {
                std::uint32_t ps = 0;
                if (sysctl_value("hw.pagesize", ps))
                    info.PageSizeBytes = ps;
            }

            return info;
        }
    #endif

    // Called and performed once
    const PlatformInfo& GetPlatformInfo()
    {
        static PlatformInfo cached{};
        static std::once_flag once;
        std::call_once(once, [] { cached = queryPlatformInfo_Impl(); });
        return cached;
    }

} // namespace Impulse::Platform