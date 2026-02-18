#include <Impulse/Platform.hpp>
#include <Impulse/Assert.hpp>
#include <limits>
#include <mutex>

#if defined(_WIN32) || defined(_WIN64)
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <Windows.h>
    #include <winternl.h>
    #include <string>
    #include <sstream>
    #include <cstdint>
    #include <algorithm>
#endif
#ifdef __APPLE__
    #include <sys/types.h>
    #include <sys/sysctl.h>
    #include <cctype>
    #include <string>
    #include <vector>
    #include <algorithm>
#endif
#ifdef __linux__
#endif

namespace Impulse::Platform
{
    #if defined(_WIN32) || defined(_WIN64)
        static std::string reqQueryString(HKEY root, const wchar_t* subkey, const wchar_t* value)
        {
            HKEY h = nullptr;
            if (RegOpenKeyExW(root, subkey, 0, KEY_READ, &h) != ERROR_SUCCESS)
                return {};

            DWORD type = 0, bytes = 0;
            LONG rc = RegQueryValueExW(h, value, nullptr, &type, nullptr, &bytes);
            if (rc != ERROR_SUCCESS)
            {
                RegCloseKey(h);
                return {};
            }

	    	std::wstring wbuf(bytes / sizeof(wchar_t), L'\0');
            rc = RegQueryValueExW(h, value, nullptr, &type, reinterpret_cast<LPBYTE>(&wbuf[0]), &bytes);
            RegCloseKey(h);

            if (rc != ERROR_SUCCESS) return {};

            // Trim at NUL
	    	size_t nul = wbuf.find(L'\0');
            if (nul != std::wstring::npos) wbuf.resize(nul);

            // wide -> utf8
            int len = WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, nullptr, 0, nullptr, nullptr);
            if (len <= 1) return {};
            std::string out(static_cast<size_t>(len - 1), '\0');
            WideCharToMultiByte(CP_UTF8, 0, wbuf.c_str(), -1, &out[0], len, nullptr, nullptr);
            return out;
        }

        static bool rtlGetVersion(uint32_t& maj, uint32_t& min, uint32_t& bld)
        {
            maj = min = bld = 0;
            HMODULE ntdll = GetModuleHandleW(L"ntdll.dll");
            if (!ntdll) return false;

            typedef LONG(WINAPI* RtlGetVersionFn)(PRTL_OSVERSIONINFOW);
            auto fn = reinterpret_cast<RtlGetVersionFn>(GetProcAddress(ntdll, "RtlGetVersion"));
            if (!fn) return false;

            RTL_OSVERSIONINFOW vi = {};
            vi.dwOSVersionInfoSize = sizeof(vi);
            if (fn(&vi) != 0) return false; // STATUS_SUCCESS == 0

            maj = (uint32_t)vi.dwMajorVersion;
            min = (uint32_t)vi.dwMinorVersion;
            bld = (uint32_t)vi.dwBuildNumber;
            return true;
        }

        PlatformInfo queryPlatformInfo_Impl()
        {
            PlatformInfo info{};
            info.SysFamily = OSFamily::Windows;

            SYSTEM_INFO si{};
            GetNativeSystemInfo(&si);

            switch (si.wProcessorArchitecture)
            {
                case PROCESSOR_ARCHITECTURE_AMD64: info.arch = Arch::X86_64; break;
                case PROCESSOR_ARCHITECTURE_ARM64: info.arch = Arch::ARM64;  break;
                case PROCESSOR_ARCHITECTURE_INTEL: info.arch = Arch::X86;    break;
                case PROCESSOR_ARCHITECTURE_ARM:   info.arch = Arch::ARM;    break;
                default:                           info.arch = Arch::Unknown;break;
            }

            info.PageSizeBytes = (uint32_t)si.dwPageSize;

            uint32_t maj=0, min=0, bld=0;
            if (rtlGetVersion(maj, min, bld))
            {
                std::ostringstream oss;
                oss << maj << "." << min << "." << bld;
                info.OSVersion = oss.str();
            }
            else
            {
                info.OSVersion = "Unknown";
            }

            //TODO: Get CPU name from Windows registry
            info.CPUPrettyName = reqQueryString(
                HKEY_LOCAL_MACHINE,
                L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                L"ProcessorNameString"
            );
            if (info.CPUPrettyName.empty())
            {
                info.CPUPrettyName = "Unknown";
            }
            
            // sysinfo 2 for logical cores/procs
            SYSTEM_INFO si2{};
            GetSystemInfo(&si2);
            info.CPULogicalCores = (uint32_t)si2.dwNumberOfProcessors;

            // get total physical RAM
            MEMORYSTATUSEX ms{};
            ms.dwLength = sizeof(ms);
            if (GlobalMemoryStatusEx(&ms))
            {
                // Your struct uses uint32_t; clamp
                uint64_t mem = (uint64_t)ms.ullTotalPhys;
                uint64_t maxu32 = (uint64_t)std::numeric_limits<uint32_t>::max();
                info.TotalPhysicalMemBytes = (uint32_t)std::min(mem, maxu32);
            }
            else
            {
                info.TotalPhysicalMemBytes = 0;
            }

            return info;

        }

    #endif
    #if defined(__linux__)
        PlatformInfo queryPlatformInfo_Impl()
        {

        }
    #endif
    #if defined(__APPLE__)

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

            // Ensure null-termination...cut at first '\0'
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

            // OS version: prefer product version, fall back to Darwin kernel release
            info.OSVersion = sysctl_string("kern.osproductversion");
            if (info.OSVersion.empty())
                info.OSVersion = sysctl_string("kern.osrelease");

            // CPU name: prefer brand string, fall back to model identifier
            info.CPUPrettyName = sysctl_string("machdep.cpu.brand_string");
            if (info.CPUPrettyName.empty())
                info.CPUPrettyName = sysctl_string("hw.model");

            // Logical cores, RAM, page size, important stuff
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