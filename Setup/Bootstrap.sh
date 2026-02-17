#!/usr/bin/env bash
# Impulse Engine Bootstrap Script (Linux & macOS)

set -euo pipefail

GIT_STATUS="NO"
CMAKE_STATUS="NO"
CLANG_STATUS="NO"

if command -v git -v &>/dev/null; then
    echo "git found."
    GIT_STATUS="YES"
else
    echo "git not found, please install git."
fi

if command -v cmake --version &>/dev/null; then
    echo "cmake found."
    CMAKE_STATUS="YES"
else
    echo "cmake not found, please install cmake."
fi

# We do not check for Ninja, while it's a good builder,
# some may choose to avoid it in favour of a Makefile..

if ! command -v clang++ -v &>/dev/null; then
    echo "clang++ not found, we highly recommend compiling with clang."
else
    echo "clang++ found."
    CLANG_STATUS="YES"
fi

# Hardware detection section (very important)
SYSTEM=$(uname -s)

echo "Detected: $SYSTEM"

case "$SYSTEM" in
    Darwin)
        echo "Running on macOS..."

        OS_NAME=$(sw_vers -productName)
        OS_VERSION=$(sw_vers -productVersion)
        KERNEL=$(uname -r)
        CPU=$(sysctl -n machdep.cpu.brand_string)
        LOGICAL_CORES=$(sysctl -n hw.ncpu)
        MEMORY=$(sysctl -n hw.memsize)
        MEMORY_READABLE=$((MEMORY / 1024 / 1024 / 1024))

        ;;
    Linux)
        echo "Running on Linux (generic)..."

        OS_NAME=$(grep '^PRETTY_NAME=' /etc/os-release | cut -d= -f2 | tr -d '"')
        OS_VERSION=$(uname -r)
        CPU=$(lscpu | grep 'Model name' | sed 's/.*: //')
        MEMORY=$(free -h | awk '/Mem:/ {print $2}')
        MEMORY_READABLE=$MEMORY

        ;;
    *)
        echo "Unknown system: \"$SYSTEM\""
        exit 1

        ;;
esac

# Display system info
echo "----------------------------------------------"
echo "  System Information | Pre-bootstrap Section  "
echo "----------------------------------------------"
echo "OS: $OS_NAME | Ver: $OS_VERSION"
echo
echo "Has git: $GIT_STATUS"
echo "Has cmake: $CMAKE_STATUS"
echo "Has clang++: $CLANG_STATUS"
echo
echo "CPU: $CPU"
echo "Cores: $LOGICAL_CORES"
echo "RAM: $MEMORY_READABLE\GiB"
echo "----------------------------------------------"
echo
sleep 1
echo "Would you like to start download dependencies?"
echo "This will require a network and may take a few minutes."
read -r -p "(y/n): " CHOICE

if [[ "$CHOICE" =~ ^[Yy]$ ]]; then
    echo "Collecting libraries..."

    # todo, when we have deps.

    exit 0
else
    echo "Skipping collecting libraries..."
    exit 0
fi
