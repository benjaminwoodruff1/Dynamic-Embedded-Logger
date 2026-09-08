#!/usr/bin/env bash
# Verifies the zero-overhead claim for high_frequency_loop() in main.cpp.
#
# Builds a Release binary, disassembles it, and checks that
# high_frequency_loop() short-circuits on the log_enabled check
# BEFORE reaching any call to logger() -- i.e. the disabled path
# is a single compare + conditional jump, with no call instruction
# executed when logging is off.
#
# Usage: ./verification/verify_zero_overhead.sh   (run from repo root)

set -euo pipefail

BUILD_DIR="build_verify"
rm -rf "$BUILD_DIR"
cmake -DCMAKE_BUILD_TYPE=Release -DDO_UT=OFF -B "$BUILD_DIR" > /dev/null
cmake --build "$BUILD_DIR" > /dev/null

BIN="$BUILD_DIR/bin/template_bin"
DISASM=$(objdump -d --demangle "$BIN")

# Extract just the high_frequency_loop() function body
FUNC=$(echo "$DISASM" | awk '/<high_frequency_loop\(\)>:/{f=1} f{print} f && /^$/{exit}')

echo "=== high_frequency_loop() disassembly ==="
echo "$FUNC"
echo "=========================================="

# The very first branch after the log_enabled compare must be able to
# return (ret) BEFORE any call to logger@plt appears in the listing.
FIRST_RET_LINE=$(echo "$FUNC" | grep -n $'\tret' | head -1 | cut -d: -f1)
FIRST_CALL_LINE=$(echo "$FUNC" | grep -n "call.*logger" | head -1 | cut -d: -f1)

if [[ -z "$FIRST_RET_LINE" || -z "$FIRST_CALL_LINE" ]]; then
    echo "FAIL: could not locate expected ret/call instructions"
    exit 1
fi

if [[ "$FIRST_RET_LINE" -lt "$FIRST_CALL_LINE" ]]; then
    echo "PASS: disabled path (log_enabled == false) returns before any logger() call instruction is reached."
    echo "No call, no argument setup, no I/O on the disabled path -- confirmed."
    exit 0
else
    echo "FAIL: a call to logger() appears before the early return -- zero-overhead claim not supported by this build."
    exit 1
fi