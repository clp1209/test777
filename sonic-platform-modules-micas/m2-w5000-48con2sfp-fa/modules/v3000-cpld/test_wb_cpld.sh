#!/bin/bash
# SPDX-License-Identifier: GPL-2.0
#
# test_wb_cpld.sh - WB CPLD driver test script
#
# Usage:
#   sudo ./test_wb_cpld.sh
#

set -e

DRIVER_NAME="wb-cpld"
DEV_CPU="/dev/cpld_cpu"
DEV_CTRL="/dev/cpld_ctrl"

echo "================================"
echo "WB CPLD Driver Test Script"
echo "================================"

# Check if driver is loaded
if ! lsmod | grep -q wb_cpld; then
    echo "ERROR: Driver $DRIVER_NAME is not loaded"
    echo "Please run: sudo insmod wb-cpld.ko"
    exit 1
fi

echo "[PASS] Driver is loaded"

# Check device nodes
if [ ! -c "$DEV_CPU" ]; then
    echo "ERROR: Device node not found: $DEV_CPU"
    exit 1
fi

echo "[PASS] cpld_cpu device node exists"

if [ ! -c "$DEV_CTRL" ]; then
    echo "ERROR: Device node not found: $DEV_CTRL"
    exit 1
fi

echo "[PASS] cpld_ctrl device node exists"

# Test read operation (registers 0x00~0x05)
echo ""
echo "Testing read operations..."
echo "Reading CPU CPLD ID registers (0x00~0x05):"
dd if="$DEV_CPU" bs=1 skip=0 count=6 2>/dev/null | hexdump -C

echo ""
echo "Reading CTRL CPLD ID registers (0x00~0x05):"
dd if="$DEV_CTRL" bs=1 skip=0 count=6 2>/dev/null | hexdump -C

# Test write operation (CAUTION: only write to safe registers!)
# echo ""
# echo "Testing write operations..."
# echo "SKIPPED: Write test requires knowledge of safe register addresses"

echo ""
echo "================================"
echo "Basic tests PASSED"
echo "================================"
echo ""
echo "Next steps:"
echo "1. Verify CPLD version numbers match expected values"
echo "2. Test specific register read/write for your platform"
echo "3. Check dmesg for any warnings: dmesg | grep wb"
echo ""
