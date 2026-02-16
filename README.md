```
      :::::::::::    | Impulse Engine
         :+:         |
        +:+          | A powerful and modular C++ engine.
       +#+           | 
      +#+            | Runtime-first, highly hackable, open-source.
     #+#             | Universally usable, broadly compatible.
###########          |
```

<!-- We don't need dozens of epic flair badges, just a couple to show the license, approximate code size and CI build tests. -->
![GitHub top language](https://img.shields.io/github/languages/top/Impulse-Engine/Impulse?style=flat-square)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/Impulse-Engine/Impulse?style=flat-square)

This is the canonical repository for Impulse. Impulse is a powerful, lightweight, runtime-first 3D engine written in C++. Impulse is focused on portability, performance, and rapid iteration. No bloat, no film-length build times.

It is developed primarily to ship **real games,** with the long-term goal of being useful and approchable for the wider community. \
Licensed under **Apache-2.0.**

## Goals

Impulse aims to answer a simple idea:

> What if a powerful C++ engine existed that didn't require a workstation to build, and could work across most hardware?

**Core Priorities:**
 - Fast builds & iteration
 - Runtime-first architecture (headless by default)
 - Modular, service-oriented design
 - ECS-driven world model
 - Cross-platform from day one
     * _Windows 10/11_ (x86_64)
     * _Linux_ (x86_64)
     * _macOS_ (arm64)

## Project Status

Impulse is in **early development.**

The main focus is on the "spine" of the engine:
 - Runtime loop/life
 - ECS system
 - Job system
 - Platform abstraction
 - Headless execution

Rendering is intentionally deferred to solidify the core first.

## Philosophy

Impulse sits firmly between:
 - **Game-first** systems - built to ship titles
 - **Community engines** - designed for reuse and extension

Key principles:
 - The runtime owns the process
 - Systems are modular and replaceable
 - Engine core stays small and focused
 - Services (rendering, audio, etc.) are optional and pluggable
 - Headless execution is a first-class mode

## Build System

Impulse uses **CMake** for cross-platform builds. \
Requirements are kept intentionally minimal and documented as the project evolves.

**Current targets:**
 - Windows (MSVC)
 - Linux (Clang)
 - macOS (AppleClang)

## Non-Goals

To keep the early scope under control, Impulse intentionally avoids early investment into:
 - A full renderer
 - Visual scripting systems
 - An editor
 - Physics implementation
 - Networking

Initial focus is on correctness, stability and clean architecture.

## License

Impulse is licensed under the **Apache License 2.0.** \
This allows:
 - Commercial use
 - Modification
 - Distribution
 - Private use

See the `[LICENSE](./LICENSE)` for full details.

## Contributing

Contributions are welcome. The full guide, standards and setup instructions can be found in `[CONTRIBUTING.md](./.github/CONTRIBUTING.md)`

## Security

Security issues and the reporting procedure is outlined in `[SECURITY.md](./.github/SECURITY.md)`

## Disclaimer

Impulse is an experimental and ever-evolving project. \
APIs and arcitecture are subject to frequent change during early development.
