#include "utest.h"
#include <iostream>
#include <string>
#include <functional>

// Test UTEST_ASSERT_EQUALS with std::string and char* string literals
void test_string_equality() {
    // Test with std::string
    std::string str1 = "hello";
    std::string str2 = "hello";
    UTEST_ASSERT_EQUALS(str1, str2);
    
    // Test with const char* - use STR_EQUALS for string content comparison
    const char* cstr1 = "world";
    const char* cstr2 = "world";
    UTEST_ASSERT_STR_EQUALS(cstr1, cstr2);
    
    // Test mixed std::string and const char* - use STR_EQUALS
    std::string str3 = "test";
    const char* cstr3 = "test";
    UTEST_ASSERT_STR_EQUALS(str3, cstr3);
    
    // Test with string literals - use STR_EQUALS for proper string comparison
    UTEST_ASSERT_STR_EQUALS("literal1", "literal1");
}

// Test throwing with functors and lambdas
void test_throwing_functors_lambdas() {
    // Test with lambda that throws
    auto throwing_lambda = []() { 
        throw std::runtime_error("Lambda exception"); 
    };
    UTEST_ASSERT_THROWS(throwing_lambda);
    
    // Test with lambda that doesn't throw
    auto safe_lambda = []() { 
        return 42; 
    };
    UTEST_ASSERT_DOES_NOT_THROW(safe_lambda);
    
    // Test with functor that throws
    std::function<void()> throwing_functor = []() {
        throw std::invalid_argument("Functor exception");
    };
    UTEST_ASSERT_THROWS(throwing_functor);
    
    // Test with functor that doesn't throw
    std::function<int()> safe_functor = []() {
        return 100;
    };
    UTEST_ASSERT_DOES_NOT_THROW(safe_functor);
    
    // Test complex lambda with capture
    int value = 10;
    auto complex_lambda = [&value]() {
        if (value < 5) {
            throw std::out_of_range("Value too small");
        }
        return value * 2;
    };
    UTEST_ASSERT_DOES_NOT_THROW(complex_lambda);
}

// Test single parameter UTEST_FUNC_DEF
UTEST_FUNC_DEF(SingleParamTest) {
    UTEST_ASSERT_TRUE(true);
    UTEST_ASSERT_EQUALS(2 + 2, 4);
}

UTEST_FUNC_DEF(AnotherSingleTest) {
    std::string result = "success";
    UTEST_ASSERT_STR_EQUALS(result, "success");
}

// Test two parameter UTEST_FUNC_DEF2
UTEST_FUNC_DEF2(ModuleA, Feature1) {
    UTEST_ASSERT_GT(10, 5);
}

UTEST_FUNC_DEF2(ModuleA, Feature2) {
    UTEST_ASSERT_LT(3, 7);
}

UTEST_FUNC_DEF2(ModuleB, Feature1) {
    UTEST_ASSERT_NOT_NULL(&test_string_equality);
}

// Test performance with a deliberately slow test
void test_performance_timing() {
    // Simulate some work
    volatile int sum = 0;
    for (int i = 0; i < 1000000; ++i) {
        sum += i;
    }
    UTEST_ASSERT_GT(sum, 0);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "New Features Comprehensive Test\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    // Enable new features for demonstration
    UTEST_SHOW_PERFORMANCE();
    // Uncomment next line to test ASCII checkmarks
    // UTEST_USE_ASCII_CHECKMARKS();
    
    std::cout << "Testing string equality assertions...\n";
    UTEST_FUNC(string_equality);
    
    std::cout << "\nTesting throwing with functors and lambdas...\n";
    UTEST_FUNC(throwing_functors_lambdas);
    
    std::cout << "\nTesting single parameter UTEST_FUNC_DEF...\n";
    UTEST_FUNC(SingleParamTest);
    UTEST_FUNC(AnotherSingleTest);
    
    std::cout << "\nTesting two parameter UTEST_FUNC_DEF2 with grouping...\n";
    // Tests with same first parameter (ModuleA) should be grouped together
    UTEST_FUNC2(ModuleA, Feature1);
    UTEST_FUNC2(ModuleA, Feature2);
    UTEST_FUNC2(ModuleB, Feature1);
    
    std::cout << "\nTesting performance timing...\n";
    UTEST_FUNC(performance_timing);
    
    UTEST_EPILOG();
}
