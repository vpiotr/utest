#include "utest.h"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string>

// Custom exception for testing
class CustomException : public std::exception {
public:
    explicit CustomException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
private:
    std::string message;
};

// Function that throws std::runtime_error
void throwRuntimeError() {
    throw std::runtime_error("This is a runtime error");
}

// Function that throws std::out_of_range
void throwOutOfRange() {
    std::vector<int> v{1, 2, 3};
    v.at(10); // Will throw std::out_of_range
}

// Function that throws custom exception
void throwCustomException() {
    throw CustomException("Custom exception message");
}

// Function that throws bad_alloc
void throwBadAlloc() {
    throw std::bad_alloc();
}

// Function that doesn't throw
void normalFunction() {
    int x = 5 + 5;
    (void)x; // Suppress unused variable warning
}

// Function that returns a value
int computeValue() {
    return 42;
}

// Test realistic exception scenarios
void test_exception_throws() {
    // Test different exception types
    UTEST_ASSERT_THROWS(throwRuntimeError);
    UTEST_ASSERT_THROWS(throwOutOfRange);
    UTEST_ASSERT_THROWS(throwCustomException);
    UTEST_ASSERT_THROWS(throwBadAlloc);
    
    // Test with lambdas
    auto outOfRangeAccess = []() { std::string s; s.at(100); };
    UTEST_ASSERT_THROWS(outOfRangeAccess);
    
    // Test with function objects
    auto divideByZero = []() {
        int a = 5;
        int b = 0;
        if (b == 0) throw std::runtime_error("Division by zero");
        return a / b;
    };
    UTEST_ASSERT_THROWS(divideByZero);
}

// Test exception throws with messages
void test_exception_throws_msg() {
    UTEST_ASSERT_THROWS_MSG(throwRuntimeError, "Testing runtime error");
    UTEST_ASSERT_THROWS_MSG(throwOutOfRange, "Testing out of range");
    UTEST_ASSERT_THROWS_MSG(throwCustomException, "Testing custom exception");
    
    auto invalidArgThrow = []() { throw std::invalid_argument("Invalid argument"); };
    UTEST_ASSERT_THROWS_MSG(invalidArgThrow, "Lambda should throw invalid_argument");
}

// Test functions that should not throw
void test_no_exception_throws() {
    UTEST_ASSERT_DOES_NOT_THROW(normalFunction);
    UTEST_ASSERT_DOES_NOT_THROW(computeValue);
    
    // Test with lambdas that don't throw
    auto simpleCalc = []() { int x = 5; int y = 10; int z = x + y; (void)z; };
    UTEST_ASSERT_DOES_NOT_THROW(simpleCalc);
    
    // Test with function that returns value
    auto vectorSize = []() { std::vector<int> v{1, 2, 3, 4, 5}; return v.size(); };
    UTEST_ASSERT_DOES_NOT_THROW(vectorSize);
}

// Test no exception with messages
void test_no_exception_throws_msg() {
    UTEST_ASSERT_DOES_NOT_THROW_MSG(normalFunction, "Normal function should not throw");
    UTEST_ASSERT_DOES_NOT_THROW_MSG(computeValue, "Compute value should not throw");
    
    auto stringOps = []() { std::string s = "hello"; s += " world"; return s.length(); };
    UTEST_ASSERT_DOES_NOT_THROW_MSG(stringOps, "String operations should not throw");
}

// Test all MSG versions of assertions
void test_msg_versions() {
    // Basic assertions with messages
    UTEST_ASSERT_TRUE_MSG(5 > 3, "5 should be greater than 3");
    UTEST_ASSERT_FALSE_MSG(3 > 5, "3 should not be greater than 5");
    
    // Equality assertions with messages
    UTEST_ASSERT_EQUALS_MSG(10, 10, "Ten should equal ten");
    UTEST_ASSERT_NOT_EQUALS_MSG(5, 7, "Five should not equal seven");
    
    // String assertions with messages
    UTEST_ASSERT_STR_EQUALS_MSG("hello", "hello", "Strings should match");
    UTEST_ASSERT_STR_NOT_EQUALS_MSG("hello", "world", "Different strings should not match");
    
    // Comparison assertions with messages
    UTEST_ASSERT_GT_MSG(10, 5, "10 should be greater than 5");
    UTEST_ASSERT_GTE_MSG(10, 10, "10 should be greater than or equal to 10");
    UTEST_ASSERT_LT_MSG(5, 10, "5 should be less than 10");
    UTEST_ASSERT_LTE_MSG(5, 5, "5 should be less than or equal to 5");
    
    // Pointer assertions with messages
    int* nullPtr = nullptr;
    int x = 42;
    int* validPtr = &x;
    
    UTEST_ASSERT_NULL_MSG(nullPtr, "Null pointer should be null");
    UTEST_ASSERT_NOT_NULL_MSG(validPtr, "Valid pointer should not be null");
}

// Test improved convertToString functionality
void test_convert_to_string() {
    // Test numeric types (should use std::to_string)
    int intVal = 42;
    double doubleVal = 3.14159;
    float floatVal = 2.71f;
    long longVal = 1234567890L;
    
    UTEST_ASSERT_EQUALS(intVal, 42);
    UTEST_ASSERT_EQUALS(doubleVal, 3.14159);
    UTEST_ASSERT_EQUALS(floatVal, 2.71f);
    UTEST_ASSERT_EQUALS(longVal, 1234567890L);
    
    // Test bool (should use custom specialization)
    bool trueVal = true;
    bool falseVal = false;
    UTEST_ASSERT_EQUALS(trueVal, true);
    UTEST_ASSERT_EQUALS(falseVal, false);
    
    // Test char (should use custom specialization)
    char charVal = 'A';
    UTEST_ASSERT_EQUALS(charVal, 'A');
    
    // Test string types (should use custom specializations)
    std::string stdStr = "hello";
    const char* cStr = "world";
    UTEST_ASSERT_STR_EQUALS(stdStr, "hello");
    UTEST_ASSERT_STR_EQUALS(cStr, "world");
    
    // Test streamable types (should use ostringstream)
    std::vector<int> vec{1, 2, 3}; // This will likely show type info since vector isn't streamable by default
    // We can't easily test this without custom streamable types, but the framework is in place
}

// Test with custom streamable type
class StreamableClass {
public:
    StreamableClass(int val) : value(val) {}
    
    friend std::ostream& operator<<(std::ostream& os, const StreamableClass& obj) {
        os << "StreamableClass(" << obj.value << ")";
        return os;
    }
    
    bool operator==(const StreamableClass& other) const {
        return value == other.value;
    }
    
    bool operator!=(const StreamableClass& other) const {
        return !(*this == other);
    }
    
private:
    int value;
};

void test_streamable_types() {
    StreamableClass obj1(42);
    StreamableClass obj2(42);
    StreamableClass obj3(100);
    
    UTEST_ASSERT_EQUALS(obj1, obj2); // Should work with streamable output
    UTEST_ASSERT_NOT_EQUALS(obj1, obj3);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Comprehensive Exception & Features Test\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    std::cout << "Testing exception throws...\n";
    UTEST_FUNC(exception_throws);
    
    std::cout << "\nTesting exception throws with messages...\n";
    UTEST_FUNC(exception_throws_msg);
    
    std::cout << "\nTesting functions that should not throw...\n";
    UTEST_FUNC(no_exception_throws);
    
    std::cout << "\nTesting no exception with messages...\n";
    UTEST_FUNC(no_exception_throws_msg);
    
    std::cout << "\nTesting all MSG versions of assertions...\n";
    UTEST_FUNC(msg_versions);
    
    std::cout << "\nTesting improved convertToString functionality...\n";
    UTEST_FUNC(convert_to_string);
    
    std::cout << "\nTesting streamable types...\n";
    UTEST_FUNC(streamable_types);
    
    UTEST_EPILOG();
}
