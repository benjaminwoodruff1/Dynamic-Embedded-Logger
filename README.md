# Dynamic Embedded Logger

A lightweight C++ logging subsystem built for resource-constrained, real-time embedded environments — designed to keep high-frequency execution paths as close to zero-cost as possible when logging is off.

---

## ✨ Highlights

-  **Near-zero overhead when disabled** - a single short-circuited boolean check gates every log call; no function call, no formatting, no I/O when logging is off.
-  **Per-subsystem, per-severity thresholds** - filter `NAV`, `MOTOR`, `SYSTEM`, and future subsystems independently, at runtime, without recompiling.
-  **Tested with GoogleTest** - automated unit tests assert exact output for every threshold boundary, alongside a manual integration smoke test.
-  **Minimal surface area** - one header, one source file, no external dependencies beyond the C++ standard library and (optionally) GTest for testing.
-  **CMake-driven build** - a single `cmake --build .` produces the library, driver binary, and test suite together.

---

## 📖 Overview

Embedded main loops often run thousands of iterations per second, and it's tempting to gate logging with a plain `if (logging_enabled)` check scattered through the hot path. That works, but it means paying a comparison — and often argument evaluation — on every single iteration, even when logging is off 99% of the time.

This project takes a different approach: it leans on C++'s short-circuit evaluation of `&&` to skip the log call entirely when logging is disabled, rather than branching into it and immediately returning. Each log statement looks like this:

```cpp
log_enabled && logger(LOG_NAV, LOG_LEVEL_INFO, "Position updated");
```

When `log_enabled` is `false`, the right-hand side is never evaluated — no function call, no argument setup, nothing. When logging is on, `logger()` runs a second, cheap check against a per-subsystem severity threshold (`system_thresholds[LOG_NAV]`, for example) before it ever touches `printf`. That gives you two independent knobs — a global kill switch and fine-grained per-subsystem verbosity — without adding meaningful cost to the paths that matter most.

### 🧭 Where this fits

Most embedded projects reach for one of a few existing options: full-featured logging libraries like `spdlog` or `glog` (powerful, but often heavier than a microcontroller budget allows), preprocessor-based logging macros that strip calls entirely at compile time (fast, but inflexible — you can't turn logging on in the field), or a bare `printf` sprinkled through the code (flexible, but with no structure, no severity levels, and no way to silence it cheaply). This project sits in between: it keeps runtime flexibility — you can flip logging and adjust verbosity while the system is running — while keeping the disabled-state cost close to what a compile-time-stripped macro would give you.

### 👤 Who built this

This project is a personal learning exercise in low-level performance-conscious C++ design, built to explore how far you can push runtime logging controls before they start costing real CPU cycles on hot paths. See the [Author](#-author) section below for more.

---

## 🚀 Usage

Set a severity threshold per subsystem, enable logging, and call `logger()` from anywhere in your code:

```cpp
#include "template.h"

set_system_log_level(LOG_NAV, LOG_LEVEL_INFO); // NAV logs INFO and higher
setting();                                     // toggle log_enabled on

log_enabled && logger(LOG_NAV, LOG_LEVEL_INFO, "Position updated");
// -> System: NAV    INFO: Position updated

log_enabled && logger(LOG_NAV, LOG_LEVEL_DEBUG, "Raw sensor dump");
// -> filtered out; DEBUG is below the NAV threshold
```

Available severities, low to high: `LOG_LEVEL_NONE`, `LOG_LEVEL_CRITICAL`, `LOG_LEVEL_ERROR`, `LOG_LEVEL_WARNING`, `LOG_LEVEL_INFO`, `LOG_LEVEL_DEBUG`.

Available subsystems: `LOG_SYSTEM`, `LOG_NAV`, `LOG_MOTOR` (extend `LogSubsystem` in `template.h` to add more).

---

## 🧰 Installation

### Prerequisites

- GCC/G++ (C++14 or later)
- CMake (v3.24+)
- GoogleTest (`libgtest-dev`) — only required if building the test suite

### Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Run

```bash
./bin/template_bin              # driver binary
./bin/template_unittest_bin     # GoogleTest suite
cmake --build . --target check  # run tests + generate coverage report
```

---

## ✍️ Author

Built by **Benjamin Woodruff** as a hands-on exploration of low-overhead runtime logging design for embedded systems — covering short-circuit evaluation, per-subsystem verbosity control, and validating both behavior with GoogleTest.
