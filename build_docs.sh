#!/bin/bash

# build_docs.sh - Script to generate Doxygen documentation for utest library

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== utest Documentation Builder ===${NC}"
echo

# Check if doxygen is installed
if ! command -v doxygen &> /dev/null; then
    echo -e "${RED}Error: Doxygen is not installed!${NC}"
    echo -e "${YELLOW}Please install doxygen:${NC}"
    echo "  Ubuntu/Debian: sudo apt-get install doxygen"
    echo "  CentOS/RHEL:   sudo yum install doxygen"
    echo "  macOS:         brew install doxygen"
    echo "  Arch Linux:    sudo pacman -S doxygen"
    exit 1
fi

# Check if we're in the right directory
if [ ! -f "Doxyfile" ]; then
    echo -e "${RED}Error: Doxyfile not found!${NC}"
    echo "Please run this script from the utest project root directory."
    exit 1
fi

if [ ! -f "include/utest.h" ]; then
    echo -e "${RED}Error: utest.h not found!${NC}"
    echo "Please run this script from the utest project root directory."
    exit 1
fi

echo -e "${YELLOW}Cleaning previous documentation...${NC}"
if [ -d "docs" ]; then
    rm -rf docs
    echo "Removed existing docs/ directory"
fi

echo -e "${YELLOW}Generating documentation with Doxygen...${NC}"
doxygen Doxyfile

if [ $? -eq 0 ]; then
    echo
    echo -e "${GREEN}✓ Documentation generated successfully!${NC}"
    echo
    echo -e "${BLUE}Documentation location:${NC}"
    echo "  HTML: $(pwd)/docs/html/index.html"
    echo
    echo -e "${BLUE}To view the documentation:${NC}"
    echo "  Open in browser: file://$(pwd)/docs/html/index.html"
    echo "  Or run:          xdg-open docs/html/index.html"
    echo
    
    # Check if we can open the browser
    if command -v xdg-open &> /dev/null; then
        read -p "Open documentation in browser now? [y/N]: " -n 1 -r
        echo
        if [[ $REPLY =~ ^[Yy]$ ]]; then
            xdg-open docs/html/index.html
        fi
    fi
else
    echo -e "${RED}✗ Documentation generation failed!${NC}"
    echo "Check the Doxygen output above for error details."
    exit 1
fi
