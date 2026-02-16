# Contributing to Impulse

First off, thanks for taking an interest in contributing.

This document serves as both:
 - A contribution guide
 - The coding standard reference

Some of these rules may feel strict, but they exist for a reason: readability and long-term maintenence.

> [!TIP]
> "80% of the lifetime cost of a piece of software goes to maintenance." - [Unreal Engine C++ Standards](https://dev.epicgames.com/documentation/en-us/unreal-engine/epic-cplusplus-coding-standard-for-unreal-engine)

Impulse is designed to be a long-lived, production-grade engine. Clean, consistence source matters.

## Table of Contents

 - [Cloning the Project](#cloning-the-project)
 - [Bootstrapping](#bootstrapping)
 - [Building](#building)
 - [Project Structure](#project-structure)
 - [Opening a Pull Request](#opening-a-pull-request)
 - [C++ Guidelines for Impulse]()

## Cloning the Project

You can clone this repository however you prefer:

```sh
# Generic shell
git clone https://github.com/Impulse-Engine/Impulse.git
cd Impulse
```

```sh
# GitHub CLI
gh repo clone Impulse-Engine/Impulse
cd Impulse
```

For users of an IDE like Visual Studio, CLion, or VSCode; you should be able to clone the repository directly.

## Bootstrapping

At the moment, the bootstrap process is kept intentionally minimal.

### On Windows
 - Make sure you have the x64 Developer Tools Powershell installed.
 - Make sure you also have CMake installed.
 - Make sure you have Ninja installed and available on your PATH.
 - Run the `Setup/Bootstrap.ps1` script in an x64 Development Powershell terminal.

### On Linux
 - Make sure you have the `clang++` compilers installed
    * You can test compile on GCC, we highly prefer clang/llvm however.
 - Make sure you have `cmake` and `ninja` installed.
 - Run the `Setup/Bootstrap.sh` script in your shell.

### On macOS
 - Make sure you have `brew` installed.
 - Make sure you have `cmake` and `ninja` installed.
 - Run the `Setup/BootstrapX.command` script in your terminal.

The bootstrap scripts will review your hardware specs, available storage, usable RAM and logical cores, etc.. as well as fetch dependencies that the engine may rely on. The bootstrap process may take a couple minutes, depending on internet connection speeds.

## Building

Impulse uses **CMake** as the one true build system.

**Basic workflow:**
```sh
cmake -S . -B build -G Ninja # added flags as needed...
cmake --build build --parallel
```

**Tested Platforms:**
 - Windows (MSVC)
 - Linux (Clang)
 - macOS (Clang)

## Project Structure

Current layout as of **February 2026**:
```
    /docs/      -> Architecture and Design Docs
    /Setup/     -> Setup Scripts
    /Source/    -> Engine Implementation
    /Include/   -> Headers
    /examples/  -> Sample Programs (TBD)
    /tests/     -> Unit Tests (TBD)
```

## Opening a Pull Request

Before opening a PR, please ensure:
 - The project builds cleanly
 - Code follows the C++ guidelines laid out
 - No unnecessary warnings are introduced
 - Changes are focused and minimal

Good PRs:
 - Solve one problem
 - Are easy to review
 - Include a clear description

Avoid:
 - Massive refactors without discussion
 - Style-only changes across many files
 - Mixing unrelated changes

 ## C++ Guidelines for Impulse

 Due to the size of this section, it has been moved to its own document; [`CXX_STANDARDS.md`](./CXX_STANDARDS.md).

## Final Notes

Impulse is still early in development. Expect:
 - Rapid iteration
 - Occasional breaking changes
 - Evolving architecture

The goal right now is to build a solid foundation. Clean, understandable, and stable. \
Thanks for helping make that happen.