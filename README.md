# Dynamic Embedded Logger

A lightweight, zero-overhead C++ logging library for embedded and real-time systems, with per-subsystem log level filtering, CMake build configuration, Google Test unit testing, Robot Framework integration testing, and automated CI/CD pipelines for both GitHub Actions and Jenkins.

> **License:** GNU General Public License v3.0 — see [LICENSE.txt](LICENSE.txt)

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build Instructions](#build-instructions)
- [Running the Program](#running-the-program)
- [Running Unit Tests](#running-unit-tests)
- [Running Integration Tests](#running-integration-tests)
- [Logging API](#logging-api)
- [CI/CD Pipelines](#cicd-pipelines)
- [License](#license)

---

## Overview

**Dynamic Embedded Logger** is a C/C++ logging library designed for systems where logging overhead matters. It provides per-subsystem verbosity control through a `static inline` hot path that the compiler can eliminate entirely when a given log level is disabled — making it safe to leave log calls in high-frequency loops without a performance penalty. The repository also serves as a complete project scaffold with build, test, and CI/CD infrastructure ready to go.

---

## Features

- **CMake build system** with support for `Debug`, `Release`, `RelWithDebInfo`, and `MinSizeRel` build types
- **Shared component library** compiled separately from the driver binary
- **Inline subsystem logger** — per-subsystem log level filtering with zero call overhead when a log level is disabled
- **Google Test unit tests** with `gcovr` coverage reporting via a `make check` target
- **Robot Framework integration tests** that exercise the compiled binary end-to-end
- **GitHub Actions** workflow for automated build, test, and artifact upload on push/PR
- **Jenkinsfile** pipeline for build, unit test, and deploy stages with Bitbucket status notifications
- **Automatic CD tagging** — pushes incremental `cd<N>` and floating `latest_passed` git tags on every successful master merge

---

## Project Structure

```
dynamic-embedded-logger/
├── CMakeLists.txt                   # Top-level CMake configuration
├── Jenkinsfile                      # Jenkins CI/CD pipeline
├── LICENSE.txt
├── .github/
│   └── workflows/
│       └── ut_and_integration.yml   # GitHub Actions workflow
├── src/
│   ├── main.cpp                     # Driver binary entry point
│   ├── include/
│   │   ├── template.h               # Public API — logger types and inline function
│   │   └── template_config.h.in     # CMake-generated version header template
│   └── template/
│       ├── CMakeLists.txt           # Component library CMake config
│       ├── template.c               # C implementation (stub)
│       └── template.cpp             # C++ implementation — logger runtime
├── test/
│   ├── template_unittest.cpp        # Google Test unit tests
│   └── template_test.cpp            # Manual logger system test
└── integration/
    ├── requirements.txt             # Python dependencies (Robot Framework)
    └── main.robot                   # Robot Framework integration test suite
```

---

## Prerequisites

| Tool | Minimum Version | Notes |
|---|---|---|
| CMake | 3.24 | |
| GCC / Clang | C++14 support | |
| libgtest-dev | Any | `sudo apt-get install libgtest-dev` |
| gcovr | Any | `sudo apt-get install gcovr` |
| Python | 3.13 | Required for integration tests only |
| Robot Framework | 7.2.2 | Installed via `requirements.txt` |

### 👤 Who built this

## Build Instructions

All build output goes to a `build/` directory (excluded from version control).

**1. Configure**

```bash
# Debug build (includes coverage instrumentation)
cmake -DCMAKE_BUILD_TYPE=Debug -B build

# Release build
cmake -DCMAKE_BUILD_TYPE=Release -B build

# To disable unit test compilation
cmake -DDO_UT=OFF -B build
```

**2. Compile the driver binary**

```bash
cd build
make
```

**3. Compile the unit test binary**

```bash
cd build
make check   # Compiles and immediately runs the UT suite + coverage report
```

Binaries are placed in `build/bin/` and shared libraries in `build/lib/`.

log_enabled && logger(LOG_NAV, LOG_LEVEL_INFO, "Position updated");
// -> System: NAV    INFO: Position updated

## Running the Program

```bash
./build/bin/template_bin
```

Expected output demonstrates the logger's silent-by-default behavior, followed by enabling log output for the `NAV` and `MOTOR` subsystems:

```
Hello, world!
Version: 1.0.0
--- Project ---
Default Silent Mode
DONE.

Silent Mode Disabled for NAV and MOTOR
System: NAV    DEBUG: GPS satellites locked: 5
System: NAV    INFO: Current speed: 12.34 m/s
System: MOTOR    WARNING: Motor temperature high: 85.5 C
System: MOTOR    ERROR: Motor failure detected!
DONE.
```

---

## Running Unit Tests

Unit tests use the [Google Test](https://github.com/google/googletest) framework and are compiled and run together via the `check` make target:

```bash
cd build
make check
```

This will:
1. Compile `template_unittest_bin`
2. Execute the test binary and print pass/fail results
3. Run `gcovr` to generate a source-level coverage report for `src/`

### Test Coverage

| Test | What it validates |
|---|---|
| `DefaultLogLevel` | All subsystems start silent (`LOG_LEVEL_NONE`); nothing is printed |
| `EnableLogLevel` | Setting `LOG_LEVEL_INFO` correctly enables output at that level |
| `Filter` | Setting `LOG_LEVEL_WARNING` suppresses `INFO` and passes `WARNING` messages |

---

## Running Integration Tests

Integration tests use [Robot Framework](https://robotframework.org/) to execute the compiled binary as a subprocess and assert on its stdout/stderr.

**1. Install dependencies**

```bash
pip install -r integration/requirements.txt
```

**2. Build the binary first** (see [Build Instructions](#build-instructions))

**3. Run the suite**

```bash
# From repository root
robot -T --outputdir results integration/main.robot
```

Results (HTML report, log, output XML) are written to `results/`.

---

## Logging API

The logger is defined in `src/include/template.h` and implemented in `src/template/template.cpp`.

### Log Subsystems

```c
typedef enum {
    LOG_SYSTEM = 0,
    LOG_NAV    = 1,
    LOG_MOTOR  = 2,
    LOG_MAX
} LogSubsystem;
```

### Log Levels

```c
typedef enum {
    LOG_LEVEL_NONE     = 0,  // Silent (default)
    LOG_LEVEL_CRITICAL = 1,
    LOG_LEVEL_ERROR    = 2,
    LOG_LEVEL_WARNING  = 3,
    LOG_LEVEL_INFO     = 4,
    LOG_LEVEL_DEBUG    = 5,
} LogLevel;
```

### Functions

```c
// Set the minimum level for a subsystem. Messages below this level are suppressed.
void set_system_log_level(LogSubsystem subsystem, LogLevel level);

// Log a formatted message. Inlined — zero overhead when the level is below the threshold.
void log_info_inline(LogSubsystem subsystem, LogLevel level, const char* format, ...);
```

### Usage Example

```c
// Enable INFO and above for the NAV subsystem
set_system_log_level(LOG_NAV, LOG_LEVEL_INFO);

// Printed: "System: NAV    INFO: Speed: 12.34 m/s"
log_info_inline(LOG_NAV, LOG_LEVEL_INFO, "Speed: %.2f m/s", 12.34);

// Not printed (DEBUG < INFO threshold)
log_info_inline(LOG_NAV, LOG_LEVEL_DEBUG, "This is suppressed");
```

Because `log_info_inline` is declared `static inline`, the compiler eliminates the function call entirely when the log level check fails at compile time or can be constant-folded — making it suitable for high-frequency loops.

---

## CI/CD Pipelines

### GitHub Actions (`.github/workflows/ut_and_integration.yml`)

Triggers on:
- Push to `master` or `dev`
- Pull requests targeting `master`

Stages:
1. Install Google Test and gcovr
2. `cmake` configure
3. `make` (driver binary)
4. `make check` (unit tests + coverage)
5. Set up Python 3.13
6. Install Robot Framework
7. Run integration tests
8. Upload test results as a workflow artifact (retained 3 days)

After a successful push to `master`, a second job (`tag_master`) runs automatically to push an incremental `cd<N>` tag and update the floating `latest_passed` tag.

### Jenkins (`Jenkinsfile`)

| Stage | Description |
|---|---|
| **Build** | Cleans workspace, checks out source, configures with CMake |
| **Unit Test** | Runs `make check` in the `build/` directory |
| **Deploy** | Runs only on `master`; pushes `cd<BUILD_NUMBER>` and `latest_passed` tags via SSH |

Build status is reported back to Bitbucket via `bitbucketStatusNotify` at the start and end of each run.

---

## License

This project is licensed under the **GNU General Public License v3.0**.
See [LICENSE.txt](LICENSE.txt) for the full license text.

Copyright (C) 2025 Scott Carrion, BTL Technologies, Inc.
