//----------------------------------------------------------------------------------
// Name:        demo_ascii_checkmarks.cpp
// Purpose:     Demo application that demonstrates ASCII checkmarks feature
// Author:      utest library demonstration
// Created:     2024
//----------------------------------------------------------------------------------

#include "utest.h"
#include <iostream>

void test_basic_operations() {
    UTEST_ASSERT_TRUE(1 + 1 == 2);
    UTEST_ASSERT_EQUALS(5 * 6, 30);
}

void test_string_operations() {
    std::string greeting = "Hello";
    std::string world = " World";
    std::string result = greeting + world;
    UTEST_ASSERT_EQUALS(result, "Hello World");
}

void test_performance_demo() {
    // Simulate some computation
    volatile int result = 0;
    for (int i = 0; i < 100000; ++i) {
        result += i;
    }
    UTEST_ASSERT_GT(result, 0);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "ASCII Checkmarks and Performance Demo\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    // Enable ASCII checkmarks instead of Unicode
    UTEST_USE_ASCII_CHECKMARKS();
    
    // Enable performance timing
    UTEST_SHOW_PERFORMANCE();
    
    std::cout << "Note: Using ASCII checkmarks [OK]/[FAIL] instead of Unicode ✓/✗\n";
    std::cout << "Performance timing is enabled to show test execution times.\n\n";
    
    std::cout << "Running basic operations test...\n";
    UTEST_FUNC(basic_operations);
    
    std::cout << "\nRunning string operations test...\n";
    UTEST_FUNC(string_operations);
    
    std::cout << "\nRunning performance demo test...\n";
    UTEST_FUNC(performance_demo);
    
    std::cout << "\nTest summary:\n";
    UTEST_EPILOG();
}
