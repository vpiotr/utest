//----------------------------------------------------------------------------------
// Name:        demo_no_tests.cpp
// Purpose:     Demo application that demonstrates the behavior when no tests are run
// Author:      utest library demonstration
// Created:     2024
//----------------------------------------------------------------------------------

#include "utest.h"
#include <iostream>

int main() {
    std::cout << "======================================\n";
    std::cout << "utest library - No Tests Demo\n";
    std::cout << "======================================\n\n";
    
    std::cout << "This demo shows what happens when no tests are registered.\n";
    
    // Initialize test tracking
    UTEST_PROLOG();
    
    // Note: No UTEST_FUNC calls here - this demonstrates running with zero tests
    std::cout << "No tests registered or executed.\n\n";
    
    std::cout << "Test summary:\n";
    // This should now fail
    UTEST_EPILOG();
}
