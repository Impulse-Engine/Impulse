#include <string>

namespace Impulse::Platform
{

    enum class OSFamily : std::uint8_t
    {
        Windows,
        macOS,
        Linux,
        Unknown
    };

    enum class Arch
    {
        X86_64,
        ARM64,
        X86,
        ARM,
        Unknown
    };

    struct PlatformInfo
    {
        OSFamily SysFamily = OSFamily::Unknown;
        Arch arch = Arch::Unknown;

        std::string OSVersion;
        std::string CPUPrettyName;
        std::uint32_t CPULogicalCores;
        std::uint64_t TotalPhysicalMemBytes;
        std::uint64_t PageSizeBytes;
    };

    const PlatformInfo& GetPlatformInfo();

} // namespace Impulse::Platform