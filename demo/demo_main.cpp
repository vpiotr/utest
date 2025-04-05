#include "utest.h"
#include <iostream>
#include <vector>
#include <string>

// Simple test case function
void test_basic_assertions() {
    // Test equality
    int a = 5;
    int b = 5;
    UTEST_ASSERT_EQUALS(a, b);
    
    // Test true/false
    UTEST_ASSERT_TRUE(a == b);
    UTEST_ASSERT_FALSE(a != b);
    
    // Test with messages
    UTEST_ASSERT_TRUE_MSG(a == 5, "a should equal 5");
    UTEST_ASSERT_FALSE_MSG(a == 6, "a should not equal 6");
    
    std::string s1 = "hello";
    std::string s2 = "hello";
    UTEST_ASSERT_EQUALS_MSG(s1, s2, "Strings should match");
}

// Test case with failure
void test_failing_assertion() {
    int a = 5;
    int b = 6;
    // This will fail
    UTEST_ASSERT_EQUALS(a, b);
}

// Test case with exception handling
void test_exception_assertions() {
    // Test that an exception is thrown
    UTEST_ASSERT_THROWS([]() { throw std::runtime_error("Expected exception"); });
    
    // Test with message
    auto throwFunc = []() { throw std::out_of_range("Index out of range"); };
    UTEST_ASSERT_THROWS_MSG(throwFunc, "Function should throw an exception");
    
    // This would fail if the function didn't throw
    UTEST_ASSERT_THROWS([]() {
        std::vector<int> v{1, 2, 3};
        return v.at(10); // This will throw std::out_of_range
    });
}

// Demonstration of a complex test
void test_complex_scenario() {
    std::vector<int> numbers{10, 20, 30, 40, 50};
    
    // Test the size
    UTEST_ASSERT_EQUALS(numbers.size(), 5);
    
    // Test individual elements
    UTEST_ASSERT_EQUALS(numbers[0], 10);
    UTEST_ASSERT_EQUALS(numbers[4], 50);
    
    // Test a computation
    int sum = 0;
    for (int num : numbers) {
        sum += num;
    }
    UTEST_ASSERT_EQUALS(sum, 150);
    UTEST_ASSERT_TRUE(sum > 100);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "utest library demonstration\n";
    std::cout << "======================================\n\n";
    
    // Initialize test tracking
    UTEST_PROLOG();
    
    // Run test cases
    std::cout << "\nRunning basic assertions test...\n";
    UTEST_FUNC(basic_assertions);
    
    std::cout << "\nRunning complex scenario test...\n";
    UTEST_FUNC(complex_scenario);
    
    std::cout << "\nRunning exception assertions test...\n";
    UTEST_FUNC(exception_assertions);
    
    std::cout << "\nRunning intentionally failing test...\n";
    UTEST_FUNC(failing_assertion);
    
    // Report overall result
    std::cout << "\nTest summary:\n";
    UTEST_EPILOG();
}