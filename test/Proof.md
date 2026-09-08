# Zero-Overhead Verification

This document is the evidence behind the "near-zero overhead when disabled"
claim in the README. It was produced by running
[`verification/verify_zero_overhead.sh`](verification/verify_zero_overhead.sh)
against a Release build (`-O2`, no debug/coverage flags).

## Method

1. Build Release: `cmake -DCMAKE_BUILD_TYPE=Release -DDO_UT=OFF -B build && cmake --build build`
2. Disassemble the driver binary: `objdump -d --demangle build/bin/template_bin`
3. Isolate `high_frequency_loop()` and confirm the disabled path (`log_enabled == false`)
   reaches a `ret` before any `call` to `logger()` is executed.

## Result

```asm
00000000000012a0 <high_frequency_loop()>:
    12a0: endbr64
    12a4: cmpb   $0x0,0x2d65(%rip)        # log_enabled
    12ab: jne    12b0 <+0x10>
    12ad: ret                              ; <-- log_enabled == false exits here
    ...
    12b0: sub    $0x8,%rsp
    12b4: mov    $0x4,%esi
    12b9: mov    $0x1,%edi
    12be: lea    0xd3f(%rip),%rdx
    12c5: call   10c0 <logger@plt>         ; <-- only reached when log_enabled == true
```

## Interpretation

When `log_enabled` is `false`, the compiled function is one byte-compare
(`cmpb`), one conditional jump (`jne`), and `ret`. Execution never reaches
the `call logger@plt` instruction, so none of the following happen on the
disabled path:

- No function call (no `call` instruction executed)
- No argument setup (no `mov`/`lea` populating registers for `logger()`'s args)
- No stack allocation for the call (`sub $0x8,%rsp` is skipped)
- No I/O (`logger()` itself is never entered, so `printf` is never reached)

`logger()` is compiled into a separate shared object (`libtemplate.so`) and
resolved dynamically via `logger@plt` — the skip is a genuine **runtime**
short-circuit on the `&&` operator, not a compile-time optimization. This
confirms the mechanism described in `main.cpp`:

```cpp
log_enabled && logger(LOG_NAV, LOG_LEVEL_INFO, "Position updated");
```

Reproduce this yourself: `./verification/verify_zero_overhead.sh` from the
repo root (requires `cmake`, a C++14 compiler, and `objdump`).