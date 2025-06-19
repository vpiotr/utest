#!/bin/bash

# run_all_demos.sh - Script to run all demo programs for utest library

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${BLUE}===============================================${NC}"
echo -e "${BLUE}           utest Framework Demo Suite${NC}"
echo -e "${BLUE}===============================================${NC}"
echo

# Check if demos are built
if [ ! -d "build/bin" ]; then
    echo -e "${RED}Error: build/bin directory not found!${NC}"
    echo "Please build the project first with: ./rebuild.sh"
    exit 1
fi

# Function to run a demo with proper formatting
run_demo() {
    local demo_name="$1"
    local demo_description="$2"
    local demo_path="build/bin/$demo_name"
    
    echo -e "${CYAN}=== $demo_description ===${NC}"
    echo
    
    if [ -f "$demo_path" ]; then
        echo -e "${YELLOW}Running: $demo_path${NC}"
        echo
        
        # Run the demo and capture exit code
        "$demo_path"
        local exit_code=$?
        echo
        
        if [ "$demo_name" = "demo_with_failure" ]; then
            # This demo is expected to fail
            if [ $exit_code -ne 0 ]; then
                echo -e "${YELLOW}⚠ Demo completed with expected failure (exit code: $exit_code)${NC}"
                return 0
            else
                echo -e "${RED}✗ Demo was expected to fail but succeeded (exit code: $exit_code)${NC}"
                return 1
            fi
        elif [ "$demo_name" = "demo_no_tests" ]; then
            # This demo might succeed or fail depending on configuration
            echo -e "${GREEN}✓ Demo completed (exit code: $exit_code)${NC}"
            return 0
        else
            # Normal demos should succeed
            if [ $exit_code -eq 0 ]; then
                echo -e "${GREEN}✓ Demo completed successfully${NC}"
                return 0
            else
                echo -e "${RED}✗ Demo failed unexpectedly (exit code: $exit_code)${NC}"
                return 1
            fi
        fi
    else
        echo -e "${RED}✗ Demo executable not found: $demo_path${NC}"
        return 1
    fi
    
    echo
    echo -e "${BLUE}===============================================${NC}"
    echo
}

# Track demo results
TOTAL_DEMOS=0
FAILED_DEMOS=0

echo -e "${YELLOW}Running all utest framework demos...${NC}"
echo

# Main demo - core functionality
echo -e "${BLUE}>>> Demo 1/4: Core Features${NC}"
((TOTAL_DEMOS++))
run_demo "utest_demo" "Core Features Demo" || ((FAILED_DEMOS++))

# Unicode checkmarks demo
echo -e "${BLUE}>>> Demo 2/4: Unicode Checkmarks${NC}"
((TOTAL_DEMOS++))
run_demo "demo_unicode_checkmarks" "Unicode Checkmarks Demo" || ((FAILED_DEMOS++))

# Failure handling demo
echo -e "${BLUE}>>> Demo 3/4: Failure Handling${NC}"
((TOTAL_DEMOS++))
run_demo "demo_with_failure" "Failure Handling Demo" || ((FAILED_DEMOS++))

# No tests scenario demo
echo -e "${BLUE}>>> Demo 4/4: No Tests Scenario${NC}"
((TOTAL_DEMOS++))
run_demo "demo_no_tests" "No Tests Scenario Demo" || ((FAILED_DEMOS++))

# Final summary
echo -e "${BLUE}===============================================${NC}"
echo -e "${BLUE}                Demo Summary${NC}"
echo -e "${BLUE}===============================================${NC}"

if [ $FAILED_DEMOS -eq 0 ]; then
    echo -e "${GREEN}✓ All $TOTAL_DEMOS demos completed successfully!${NC}"
    echo
    echo -e "${CYAN}The utest framework is working correctly and ready for use.${NC}"
    echo
    echo -e "${YELLOW}Key features demonstrated:${NC}"
    echo "  • Basic assertion macros (TRUE, FALSE, EQUALS, etc.)"
    echo "  • Exception testing (THROWS, DOES_NOT_THROW)"
    echo "  • String comparisons and mixed type support"
    echo "  • Performance timing with microsecond precision"
    echo "  • Test grouping and organization"
    echo "  • Configurable output formats (ASCII/Unicode)"
    echo "  • Comprehensive error reporting"
    echo "  • Robust failure handling"
    echo
    echo -e "${GREEN}All demos completed successfully! 🎉${NC}"
else
    echo -e "${RED}✗ $FAILED_DEMOS out of $TOTAL_DEMOS demos failed${NC}"
    echo
    echo -e "${YELLOW}Please check the output above for details.${NC}"
    exit 1
fi
