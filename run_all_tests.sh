#!/bin/bash

# Test runner script for utest library
# Runs all pre-built test binaries from the build directory

echo "========================================"
echo "Running all utest library tests"
echo "========================================"

# Set up variables
BUILD_DIR="build/bin/tests"
FAILED_TESTS=0
TOTAL_TESTS=0
PASS_COUNT=0

# Check if tests are built
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: $BUILD_DIR directory not found!"
    echo "Please build the project first with: ./rebuild.sh"
    exit 1
fi

# Function to run a single test
run_test() {
    local test_exec="$1"
    local test_name=$(basename "$test_exec")
    
    echo ""
    echo "========================================="
    echo "Running: $test_name"
    echo "========================================="
    
    if [ -f "$test_exec" ]; then
        # Run the test and capture exit code
        if "$test_exec" > /dev/null 2>&1; then
            local exit_code=$?
            echo "✓ Test execution successful (exit code: $exit_code)"
            ((PASS_COUNT++))
        else
            local exit_code=$?
            echo "✗ Test execution failed (exit code: $exit_code)"
            # For tests designed to fail (like test_no_tests), this is expected
            if [[ "$test_name" == "test_no_tests" ]] || [[ "$test_name" == *"failure"* ]]; then
                echo "  (This test is designed to fail - expected behavior)"
                ((PASS_COUNT++))
            else
                ((FAILED_TESTS++))
            fi
        fi
    else
        echo "✗ Test executable not found: $test_exec"
        ((FAILED_TESTS++))
    fi
    
    ((TOTAL_TESTS++))
}

# Function to run detailed test output
run_test_detailed() {
    local test_exec="$1"
    local test_name=$(basename "$test_exec")
    
    echo ""
    echo "========================================="
    echo "Running detailed output for: $test_name"
    echo "========================================="
    
    if [ -f "$test_exec" ]; then
        # Run the test with full output
        "$test_exec" || true
    else
        echo "✗ Test executable not found: $test_exec"
    fi
}

# Change to project directory
cd "$(dirname "$0")"

# Check if detailed mode is requested
if [[ "$1" == "--detailed" ]] || [[ "$1" == "-d" ]]; then
    echo "Running in detailed mode..."
    
    # Find and run all test executables with detailed output
    for test_exec in "$BUILD_DIR"/test*; do
        if [ -f "$test_exec" ]; then
            run_test_detailed "$test_exec"
        fi
    done
else
    # Find and run all test executables
    for test_exec in "$BUILD_DIR"/test*; do
        if [ -f "$test_exec" ]; then
            run_test "$test_exec"
        fi
    done

    # Summary
    echo ""
    echo "========================================="
    echo "Test Summary"
    echo "========================================="
    echo "Total tests run: $TOTAL_TESTS"
    echo "Failed tests: $FAILED_TESTS"
    echo "Passed tests: $PASS_COUNT"

    if [ $FAILED_TESTS -eq 0 ]; then
        echo "✓ All tests passed!"
        exit 0
    else
        echo "✗ Some tests failed!"
        echo ""
        echo "Run with --detailed flag to see full test output"
        exit 1
    fi
fi
