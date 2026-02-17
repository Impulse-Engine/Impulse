#include <string>

#if defined(__linux__)
    #include <cstdint>
#endif

namespace Impulse::Runtime
{
    struct PlatformInfo
    {
        std::string operatingSystem;
        std::string targetArchitecture;
        std::string cpuNamePretty;
        uint8_t cpuCoresLogical;
        uint64_t totalSystemRAM;
    }; // PlatformInfo

    bool Initialize();
    void Run();
    void Shutdown();

    PlatformInfo getPlatformInfo();
} // namespace Impulse::Runtime