#include "utest.h"
#include <iostream>
#include <vector>
#include <string>

// Test basic assertions
void test_basic_assertions() {
    int a = 5;
    int b = 5;
    int c = 10;
    
    UTEST_ASSERT_EQUALS(a, b);
    UTEST_ASSERT_EQ(a, b);  // Alias
    
    UTEST_ASSERT_NOT_EQUALS(a, c);
    UTEST_ASSERT_NEQ(a, c);  // Alias
    
    UTEST_ASSERT_TRUE(a == b);
    UTEST_ASSERT_FALSE(a != b);
}

// Test string assertions
void test_string_assertions() {
    const char* str1 = "hello";
    const char* str2 = "hello";
    const char* str3 = "world";
    std::string s1 = "hello";
    std::string s2 = "hello";
    std::string s3 = "world";
    
    UTEST_ASSERT_STR_EQUALS(str1, str2);
    UTEST_ASSERT_SEQ(str1, str2);  // Alias
    
    UTEST_ASSERT_STR_NOT_EQUALS(str1, str3);
    UTEST_ASSERT_SNEQ(str1, str3);  // Alias
    
    UTEST_ASSERT_STR_EQUALS(s1, s2);
    UTEST_ASSERT_STR_NOT_EQUALS(s1, s3);
}

// Test comparison assertions
void test_comparison_assertions() {
    int a = 5;
    int b = 10;
    int c = 5;
    
    UTEST_ASSERT_GT(b, a);      // 10 > 5
    UTEST_ASSERT_GTE(b, a);     // 10 >= 5
    UTEST_ASSERT_GTE(a, c);     // 5 >= 5
    
    UTEST_ASSERT_LT(a, b);      // 5 < 10
    UTEST_ASSERT_LTE(a, b);     // 5 <= 10
    UTEST_ASSERT_LTE(a, c);     // 5 <= 5
    
    double x = 3.14;
    double y = 2.71;
    UTEST_ASSERT_GT(x, y);
    UTEST_ASSERT_LT(y, x);
}

// Test exception handling
void test_exception_handling() {
    UTEST_ASSERT_THROWS([]() { throw std::runtime_error("Expected exception"); });
    
    auto throwFunc = []() { throw std::out_of_range("Index out of range"); };
    UTEST_ASSERT_THROWS_MSG(throwFunc, "Function should throw an exception");
    
    auto indexOutOfRangeFunc = []() {
        std::vector<int> v{1, 2, 3};
        return v.at(10); // This will throw std::out_of_range
    };
    UTEST_ASSERT_THROWS(indexOutOfRangeFunc);
}

// Test pointer assertions
void test_pointer_assertions() {
    int* null_ptr = nullptr;
    UTEST_ASSERT_NULL(null_ptr);
    
    int x = 42;
    int* ptr = &x;
    UTEST_ASSERT_NOT_NULL(ptr);
    
    std::string* dynamic_string = new std::string("hello");
    UTEST_ASSERT_NOT_NULL(dynamic_string);
    
    delete dynamic_string;
    dynamic_string = nullptr;
    UTEST_ASSERT_NULL(dynamic_string);
}

// Demonstrating the new function definition macro
UTEST_FUNC_DEF(LevelManagement, TimestampUtilities) {
    // This function is named test_LevelManagement_TimestampUtilities
    std::string level = "INFO";
    std::string expected = "INFO";
    UTEST_ASSERT_STR_EQUALS(level, expected);
    
    // Simulate timestamp utilities
    long long timestamp = 1234567890;
    UTEST_ASSERT_GT(timestamp, 0);
    UTEST_ASSERT_LT(timestamp, 9999999999LL);
}

UTEST_FUNC_DEF(DataProcessing, ValidationEngine) {
    // This function is named test_DataProcessing_ValidationEngine
    std::vector<int> data{1, 2, 3, 4, 5};
    UTEST_ASSERT_EQUALS(data.size(), 5);
    
    for (size_t i = 0; i < data.size(); ++i) {
        UTEST_ASSERT_EQUALS(data[i], static_cast<int>(i + 1));
    }
}

// Test that will fail to demonstrate error reporting
void test_intentional_failure() {
    int a = 5;
    int b = 6;
    UTEST_ASSERT_EQUALS(a, b);  // This will fail
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Comprehensive utest library demo\n";
    std::cout << "======================================\n\n";
    
    // Initialize test tracking
    UTEST_PROLOG();
    
    // Run test cases
    std::cout << "Running basic assertions test...\n";
    UTEST_FUNC(basic_assertions);
    
    std::cout << "\nRunning string assertions test...\n";
    UTEST_FUNC(string_assertions);
    
    std::cout << "\nRunning comparison assertions test...\n";
    UTEST_FUNC(comparison_assertions);
    
    std::cout << "\nRunning exception handling test...\n";
    UTEST_FUNC(exception_handling);
    
    std::cout << "\nRunning pointer assertions test...\n";
    UTEST_FUNC(pointer_assertions);
    
    std::cout << "\nRunning LevelManagement TimestampUtilities test...\n";
    UTEST_FUNC2(LevelManagement, TimestampUtilities);
    
    std::cout << "\nRunning DataProcessing ValidationEngine test...\n";
    UTEST_FUNC2(DataProcessing, ValidationEngine);
    
    std::cout << "\nRunning intentional failure test...\n";
    UTEST_FUNC(intentional_failure);
    
    // Report overall result with summary
    UTEST_EPILOG();
}
