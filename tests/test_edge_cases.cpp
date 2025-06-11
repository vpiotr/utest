#include "utest.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>

// Non-streamable class for testing type info fallback
class NonStreamableClass {
public:
    NonStreamableClass(int val) : value(val) {}
    
    bool operator==(const NonStreamableClass& other) const {
        return value == other.value;
    }
    
    bool operator!=(const NonStreamableClass& other) const {
        return !(*this == other);
    }
    
private:
    int value;
};

// Test edge cases with different data types
void test_data_type_edge_cases() {
    // Test with very large numbers
    long long bigInt = 9223372036854775807LL; // Max long long
    UTEST_ASSERT_EQUALS(bigInt, 9223372036854775807LL);
    
    // Test with very small numbers
    long long smallInt = -9223372036854775807LL;
    UTEST_ASSERT_EQUALS(smallInt, -9223372036854775807LL);
    
    // Test with floating point precision
    double pi = 3.141592653589793;
    UTEST_ASSERT_EQUALS(pi, 3.141592653589793);
    
    // Test with zero values
    int zero = 0;
    double zeroDouble = 0.0;
    UTEST_ASSERT_EQUALS(zero, 0);
    UTEST_ASSERT_EQUALS(zeroDouble, 0.0);
    
    // Test with negative values
    int negative = -42;
    UTEST_ASSERT_EQUALS(negative, -42);
    UTEST_ASSERT_LT(negative, 0);
}

// Test string edge cases
void test_string_edge_cases() {
    // Empty strings
    std::string empty1 = "";
    std::string empty2 = "";
    const char* emptyC = "";
    
    UTEST_ASSERT_STR_EQUALS(empty1, empty2);
    UTEST_ASSERT_STR_EQUALS(empty1, emptyC);
    
    // Strings with special characters
    std::string special1 = "Hello\nWorld\t!";
    std::string special2 = "Hello\nWorld\t!";
    UTEST_ASSERT_STR_EQUALS(special1, special2);
    
    // Unicode strings (basic)
    std::string unicode1 = "Héllo Wörld";
    std::string unicode2 = "Héllo Wörld";
    UTEST_ASSERT_STR_EQUALS(unicode1, unicode2);
    
    // Very long strings
    std::string long1(1000, 'A');
    std::string long2(1000, 'A');
    UTEST_ASSERT_STR_EQUALS(long1, long2);
}

// Test pointer edge cases
void test_pointer_edge_cases() {
    // Null pointers of different types
    int* nullInt = nullptr;
    char* nullChar = nullptr;
    void* nullVoid = nullptr;
    
    UTEST_ASSERT_NULL(nullInt);
    UTEST_ASSERT_NULL(nullChar);
    UTEST_ASSERT_NULL(nullVoid);
    
    // Valid pointers
    int value = 42;
    int* validPtr = &value;
    UTEST_ASSERT_NOT_NULL(validPtr);
    
    // Smart pointers
    std::unique_ptr<int> smartPtr(new int(42));
    UTEST_ASSERT_NOT_NULL(smartPtr.get());
    
    std::unique_ptr<int> nullSmartPtr;
    UTEST_ASSERT_NULL(nullSmartPtr.get());
}

// Test comparison edge cases
void test_comparison_edge_cases() {
    // Floating point comparisons (be careful with precision)
    double a = 1.0;
    double b = 1.0;
    UTEST_ASSERT_EQUALS(a, b);
    UTEST_ASSERT_GTE(a, b);
    UTEST_ASSERT_LTE(a, b);
    
    // Mixed type comparisons (compiler should handle implicit conversions)
    int intVal = 5;
    double doubleVal = 5.0;
    UTEST_ASSERT_EQUALS(intVal, static_cast<int>(doubleVal));
    
    // Boundary values
    int maxInt = 2147483647;
    int minInt = -2147483648;
    UTEST_ASSERT_GT(maxInt, minInt);
    UTEST_ASSERT_LT(minInt, maxInt);
}

// Test non-streamable types (should fall back to type info)
void test_non_streamable_types() {
    NonStreamableClass obj1(42);
    NonStreamableClass obj2(42);
    NonStreamableClass obj3(100);
    
    // These should work but will show type info in error messages
    UTEST_ASSERT_EQUALS(obj1, obj2);
    UTEST_ASSERT_NOT_EQUALS(obj1, obj3);
}

// Test exception edge cases
void test_exception_edge_cases() {
    // Test nested exceptions
    auto nestedThrow = []() {
        try {
            throw std::runtime_error("Inner exception");
        } catch (...) {
            throw std::logic_error("Outer exception");
        }
    };
    UTEST_ASSERT_THROWS(nestedThrow);
    
    // Test exception with empty message
    auto emptyMessageThrow = []() {
        throw std::runtime_error("");
    };
    UTEST_ASSERT_THROWS(emptyMessageThrow);
    
    // Test exception in destructor scenario (should not throw in destructor normally)
    auto destructorTest = []() {
        struct TestClass {
            ~TestClass() noexcept {
                // Destructor should not throw
            }
        };
        TestClass obj;
        // Object will be destroyed normally
    };
    UTEST_ASSERT_DOES_NOT_THROW(destructorTest);
}

// Test boundary conditions
void test_boundary_conditions() {
    // Test with containers
    std::vector<int> vec;
    UTEST_ASSERT_EQUALS(vec.size(), 0);
    
    vec.push_back(1);
    UTEST_ASSERT_EQUALS(vec.size(), 1);
    UTEST_ASSERT_EQUALS(vec[0], 1);
    
    // Test with maps
    std::map<std::string, int> testMap;
    UTEST_ASSERT_EQUALS(testMap.size(), 0);
    
    testMap["key"] = 42;
    UTEST_ASSERT_EQUALS(testMap.size(), 1);
    UTEST_ASSERT_EQUALS(testMap["key"], 42);
}

// Test macro edge cases
void test_macro_edge_cases() {
    // Test with complex expressions
    int a = 5, b = 3, c = 2;
    UTEST_ASSERT_EQUALS(a + b, 8);
    UTEST_ASSERT_GT(a * b, c * c);
    UTEST_ASSERT_LTE(a - b, c + 1);
    
    // Test with function calls
    auto getValue = []() { return 42; };
    UTEST_ASSERT_EQUALS(getValue(), 42);
    
    // Test with ternary operator
    int x = 10;
    UTEST_ASSERT_EQUALS(x > 5 ? 1 : 0, 1);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Edge Cases and Error Scenarios Test\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    std::cout << "Testing data type edge cases...\n";
    UTEST_FUNC(data_type_edge_cases);
    
    std::cout << "\nTesting string edge cases...\n";
    UTEST_FUNC(string_edge_cases);
    
    std::cout << "\nTesting pointer edge cases...\n";
    UTEST_FUNC(pointer_edge_cases);
    
    std::cout << "\nTesting comparison edge cases...\n";
    UTEST_FUNC(comparison_edge_cases);
    
    std::cout << "\nTesting non-streamable types...\n";
    UTEST_FUNC(non_streamable_types);
    
    std::cout << "\nTesting exception edge cases...\n";
    UTEST_FUNC(exception_edge_cases);
    
    std::cout << "\nTesting boundary conditions...\n";
    UTEST_FUNC(boundary_conditions);
    
    std::cout << "\nTesting macro edge cases...\n";
    UTEST_FUNC(macro_edge_cases);
    
    UTEST_EPILOG();
}
