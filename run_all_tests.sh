#!/bin/bash

# Test runner script for utest library
# Compiles and runs all test*.cpp files in tests/ directory

echo "========================================"
echo "Running all utest library tests"
echo "========================================"

# Set up variables
TEST_DIR="tests"
INCLUDE_DIR="include"
FAILED_TESTS=0
TOTAL_TESTS=0
PASS_COUNT=0

# Function to run a single test
run_test() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .cpp)
    local test_exec="$TEST_DIR/$test_name"
    
    echo ""
    echo "========================================="
    echo "Compiling and running: $test_name"
    echo "========================================="
    
    # Compile the test
    if g++ -std=c++11 -I "$INCLUDE_DIR" "$test_file" -o "$test_exec" 2>/dev/null; then
        echo "✓ Compilation successful"
        
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
        echo "✗ Compilation failed"
        ((FAILED_TESTS++))
    fi
    
    ((TOTAL_TESTS++))
    
    # Clean up executable
    if [ -f "$test_exec" ]; then
        rm "$test_exec"
    fi
}

# Function to run detailed test output
run_test_detailed() {
    local test_file="$1"
    local test_name=$(basename "$test_file" .cpp)
    local test_exec="$TEST_DIR/$test_name"
    
    echo ""
    echo "========================================="
    echo "Running detailed output for: $test_name"
    echo "========================================="
    
    # Compile the test
    if g++ -std=c++11 -I "$INCLUDE_DIR" "$test_file" -o "$test_exec" 2>/dev/null; then
        # Run the test with full output
        "$test_exec" || true
    fi
    
    # Clean up executable
    if [ -f "$test_exec" ]; then
        rm "$test_exec"
    fi
}

# Change to project directory
cd "$(dirname "$0")"

# Check if detailed mode is requested
if [[ "$1" == "--detailed" ]] || [[ "$1" == "-d" ]]; then
    echo "Running in detailed mode..."
    
    # Find and run all test*.cpp files with detailed output
    for test_file in "$TEST_DIR"/test*.cpp; do
        if [ -f "$test_file" ]; then
            run_test_detailed "$test_file"
        fi
    done
else
    # Find and run all test*.cpp files
    for test_file in "$TEST_DIR"/test*.cpp; do
        if [ -f "$test_file" ]; then
            run_test "$test_file"
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
