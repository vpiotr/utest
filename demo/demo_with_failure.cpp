#include "utest.h"
#include <iostream>
#include <vector>
#include <string>

// Test that should pass
void test_basic_operations() {
    int a = 5;
    int b = 5;
    UTEST_ASSERT_EQUALS(a, b);
    
    std::string str1 = "hello";
    std::string str2 = "hello";
    UTEST_ASSERT_STR_EQUALS(str1, str2);
    
    UTEST_ASSERT_TRUE(10 > 5);
    UTEST_ASSERT_GT(10, 5);
}

// Test that INTENTIONALLY FAILS - for demonstration
void test_intentional_failure() {
    std::cout << "  [INFO] This test is designed to fail for demonstration purposes\n";
    
    int expected = 42;
    int actual = 24;
    
    // This assertion will fail on purpose
    UTEST_ASSERT_EQUALS_MSG(actual, expected, 
        "This is an intentional failure to demonstrate error reporting");
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Demo with Intentional Failure\n";
    std::cout << "======================================\n";
    std::cout << "This demo shows how utest handles both passing and failing tests.\n";
    std::cout << "One test will intentionally fail to demonstrate error reporting.\n\n";
    
    UTEST_PROLOG();
    
    std::cout << "Running basic operations test...\n";
    UTEST_FUNC(basic_operations);
    
    std::cout << "\nRunning intentional failure test...\n";
    UTEST_FUNC(intentional_failure);
    
    std::cout << "\n[INFO] The failure above is expected and demonstrates error reporting.\n";
    
    UTEST_EPILOG();
}
