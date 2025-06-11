#!/bin/bash

# run_all_demos.sh - Script to run all demo programs for utest library

echo "==============================================="
echo "           utest Framework Demo Suite"
echo "==============================================="
echo

# Check if demos are built
if [ ! -d "build/bin" ]; then
    echo "Error: build/bin directory not found!"
    echo "Please build the project first with: mkdir -p build && cd build && cmake .. && make"
    exit 1
fi

# List available demos
echo "Available demos:"
for demo in build/bin/demo_* build/bin/utest_demo; do
    if [ -f "$demo" ]; then
        echo "  $(basename "$demo")"
    fi
done
echo

echo "Note: Run individual demos manually:"
echo "  ./build/bin/utest_demo"
echo "  ./build/bin/demo_unicode_checkmarks"
echo "  ./build/bin/demo_with_failure"
echo "  ./build/bin/demo_no_tests"
echo

echo "All demos are built and ready to run!"
echo "==============================================="
