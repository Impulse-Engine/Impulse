# Impulse Engine Bootstrap Script (Windows)
# (translated from bash by AI!)

$ErrorActionPreference = "Stop"

$GIT_STATUS = "NO"
$CMAKE_STATUS = "NO"
$CLANG_STATUS = "NO"

function Test-Command($name) {
    return $null -ne (Get-Command $name -ErrorAction SilentlyContinue)
}

if (Test-Command "git") {
    Write-Host "git found."
    $GIT_STATUS = "YES"
} else {
    Write-Host "git not found, please install git."
}

if (Test-Command "cmake") {
    Write-Host "cmake found."
    $CMAKE_STATUS = "YES"
} else {
    Write-Host "cmake not found, please install cmake."
    Write-Host "if you have cmake already, run this script"
    Write-Host "from the Powershell Developer Prompt from VS."
}

$SYSTEM = "Windows"
$osInfo = Get-CimInstance Win32_OperatingSystem
$OS_NAME = $osInfo.Caption
$OS_VERSION = $osInfo.Version
$cpuInfo = Get-CimInstance Win32_Processor | Select-Object -First 1
$CPU = $cpuInfo.Name
$LOGICAL_CORES = $cpuInfo.NumberOfLogicalProcessors
$MEMORY_BYTES = [int64]$osInfo.TotalVisibleMemorySize * 1024
$MEMORY_READABLE = [math]::Round($MEMORY_BYTES / 1GB)

Write-Host "----------------------------------------------"
Write-Host " System Information | Pre-bootstrap Section "
Write-Host "----------------------------------------------"
Write-Host "OS: $OS_NAME | Ver: $OS_VERSION"
Write-Host ""
Write-Host "Has git: $GIT_STATUS"
Write-Host "Has cmake: $CMAKE_STATUS"
Write-Host "Has clang++: $CLANG_STATUS"
Write-Host ""
Write-Host "CPU: $CPU"
Write-Host "Cores: $LOGICAL_CORES"
Write-Host "RAM: ${MEMORY_READABLE}GiB"
Write-Host "----------------------------------------------"
Write-Host ""

Start-Sleep -Seconds 1

Write-Host "Would you like to start download dependencies?"
Write-Host "This will require a network and may take a few minutes."
$CHOICE = Read-Host "(y/n)"

if ($CHOICE -match '^[Yy]$') {
    Write-Host ""
    Write-Host "Collecting libraries..."
    exit 0

}
else {
    Write-Host ""
    Write-Host "Skipping collecting libraries..."
    exit 0
}