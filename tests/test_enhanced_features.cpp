#include "utest.h"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <memory>

// Custom class to test convertToString functionality
class TestClass {
public:
    int value;
    TestClass(int v) : value(v) {}
};

// Custom streamable class
class StreamableClass {
public:
    std::string name;
    StreamableClass(const std::string& n) : name(n) {}
    
    friend std::ostream& operator<<(std::ostream& os, const StreamableClass& obj) {
        os << "StreamableClass(" << obj.name << ")";
        return os;
    }
};

// Test realistic UTEST_ASSERT_THROWS scenarios
void test_realistic_throws() {
    // Test 1: Division by zero simulation
    auto divByZero = []() {
        int a = 10;
        int b = 0;
        if (b == 0) throw std::invalid_argument("Division by zero");
        int result = a / b;
        (void)result;
    };
    UTEST_ASSERT_THROWS(divByZero);
    
    // Test 2: Vector out of bounds
    auto vecOutOfBounds = []() {
        std::vector<int> vec{1, 2, 3};
        int val = vec.at(10); // Should throw std::out_of_range
        (void)val;
    };
    UTEST_ASSERT_THROWS(vecOutOfBounds);
    
    // Test 3: Smart pointer dereferencing null
    auto nullPtrDeref = []() {
        std::unique_ptr<int> ptr;
        if (!ptr) throw std::runtime_error("Null pointer dereference");
        int val = *ptr;
        (void)val;
    };
    UTEST_ASSERT_THROWS(nullPtrDeref);
    
    // Test 4: String to number conversion failure
    auto stringConvFail = []() {
        std::string invalid = "not_a_number";
        int num = std::stoi(invalid); // Should throw std::invalid_argument
        (void)num;
    };
    UTEST_ASSERT_THROWS(stringConvFail);
}

// Test UTEST_ASSERT_DOES_NOT_THROW
void test_does_not_throw() {
    // Test 1: Normal vector access
    auto normalVecAccess = []() {
        std::vector<int> vec{1, 2, 3, 4, 5};
        int val = vec[2]; // Should not throw
        UTEST_ASSERT_EQUALS(val, 3);
    };
    UTEST_ASSERT_DOES_NOT_THROW(normalVecAccess);
    
    // Test 2: Valid string operations
    auto validStringOps = []() {
        std::string str = "hello";
        str += " world";
        UTEST_ASSERT_STR_EQUALS(str, "hello world");
    };
    UTEST_ASSERT_DOES_NOT_THROW(validStringOps);
    
    // Test 3: Valid arithmetic operations
    auto validArithmetic = []() {
        int a = 10;
        int b = 2;
        int result = a / b;
        UTEST_ASSERT_EQUALS(result, 5);
    };
    UTEST_ASSERT_DOES_NOT_THROW(validArithmetic);
}

// Test MSG versions of all assertions
void test_msg_versions() {
    // Basic assertions with messages
    UTEST_ASSERT_TRUE_MSG(5 > 3, "Five should be greater than three");
    UTEST_ASSERT_FALSE_MSG(5 < 3, "Five should not be less than three");
    
    // Equality assertions with messages
    UTEST_ASSERT_EQUALS_MSG(2 + 2, 4, "Basic arithmetic should work");
    UTEST_ASSERT_NOT_EQUALS_MSG(5, 3, "Different values should not be equal");
    
    // String assertions with messages
    UTEST_ASSERT_STR_EQUALS_MSG("hello", "hello", "Identical strings should be equal");
    UTEST_ASSERT_STR_NOT_EQUALS_MSG("hello", "world", "Different strings should not be equal");
    
    // Comparison assertions with messages
    UTEST_ASSERT_GT_MSG(10, 5, "Ten should be greater than five");
    UTEST_ASSERT_GTE_MSG(10, 10, "Ten should be greater than or equal to ten");
    UTEST_ASSERT_LT_MSG(5, 10, "Five should be less than ten");
    UTEST_ASSERT_LTE_MSG(5, 5, "Five should be less than or equal to five");
    
    // Pointer assertions with messages
    int* ptr = nullptr;
    UTEST_ASSERT_NULL_MSG(ptr, "Null pointer should be null");
    
    int value = 42;
    ptr = &value;
    UTEST_ASSERT_NOT_NULL_MSG(ptr, "Valid pointer should not be null");
    
    // Exception assertions with messages
    auto throwTestException = []() { throw std::runtime_error("test"); };
    UTEST_ASSERT_THROWS_MSG(throwTestException, "Function should throw an exception");
    
    auto simpleArithmetic = []() { int x = 1 + 1; (void)x; };
    UTEST_ASSERT_DOES_NOT_THROW_MSG(simpleArithmetic, "Simple arithmetic should not throw");
}

// Test improved convertToString functionality
void test_convert_to_string() {
    // Test numeric types (should use std::to_string)
    int intVal = 42;
    double doubleVal = 3.14159;
    float floatVal = 2.71f;
    
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(intVal), "42");
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(doubleVal), "3.141590");
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(floatVal), "2.710000");
    
    // Test bool (should use custom conversion)
    bool trueVal = true;
    bool falseVal = false;
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(trueVal), "true");
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(falseVal), "false");
    
    // Test char (should use custom conversion)
    char charVal = 'A';
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(charVal), "A");
    
    // Test string types
    std::string stdStr = "hello";
    const char* cStr = "world";
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(stdStr), "hello");
    UTEST_ASSERT_STR_EQUALS(UTEST_TO_STRING(cStr), "world");
    
    // Test streamable custom class
    StreamableClass streamable("test");
    std::string streamableStr = UTEST_TO_STRING(streamable);
    UTEST_ASSERT_TRUE(streamableStr.find("StreamableClass(test)") != std::string::npos);
    
    // Test non-streamable custom class (should show type info)
    TestClass nonStreamable(123);
    std::string nonStreamableStr = UTEST_TO_STRING(nonStreamable);
    UTEST_ASSERT_TRUE(nonStreamableStr.find("[") == 0); // Should start with [
    UTEST_ASSERT_TRUE(nonStreamableStr.find("at") != std::string::npos); // Should contain address
}

// Test vector with custom types to verify convertToString robustness
void test_complex_types() {
    std::vector<int> vec{1, 2, 3};
    UTEST_ASSERT_EQUALS_MSG(vec.size(), 3, "Vector should have 3 elements");
    
    // Test that assertion failures with complex types work
    StreamableClass obj1("first");
    StreamableClass obj2("second");
    UTEST_ASSERT_STR_NOT_EQUALS_MSG(UTEST_TO_STRING(obj1), UTEST_TO_STRING(obj2), 
                                   "Different streamable objects should not be equal");
}

// Test intentional failure to see improved error messages
void test_intentional_failure() {
    UTEST_ASSERT_EQUALS_MSG(5, 6, "This test should fail with a clear message");
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Enhanced utest library test\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    std::cout << "Testing realistic THROWS scenarios...\n";
    UTEST_FUNC(realistic_throws);
    
    std::cout << "\nTesting DOES_NOT_THROW assertions...\n";
    UTEST_FUNC(does_not_throw);
    
    std::cout << "\nTesting MSG versions of all assertions...\n";
    UTEST_FUNC(msg_versions);
    
    std::cout << "\nTesting improved convertToString...\n";
    UTEST_FUNC(convert_to_string);
    
    std::cout << "\nTesting complex types...\n";
    UTEST_FUNC(complex_types);
    
    // Note: intentional_failure test removed from production test suite
    // It was only meant for error message demonstration
    
    UTEST_EPILOG();
}
