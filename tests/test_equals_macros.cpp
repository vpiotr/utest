#include "utest.h"
#include <iostream>
#include <string>

// ===================================================================
// TESTS FOR PROPER USAGE OF ALL EQUALS MACROS
// ===================================================================

void test_utest_assert_equals_valid_types() {
    // Valid: numeric types
    int a = 42, b = 42;
    UTEST_ASSERT_EQUALS(a, b);
    
    double x = 3.14, y = 3.14;
    UTEST_ASSERT_EQUALS(x, y);
    
    // Valid: same object types
    std::string str1 = "hello", str2 = "hello";
    UTEST_ASSERT_EQUALS(str1, str2);
    
    // Valid: bool
    bool flag1 = true, flag2 = true;
    UTEST_ASSERT_EQUALS(flag1, flag2);
    
    // Valid: characters
    char c1 = 'A', c2 = 'A';
    UTEST_ASSERT_EQUALS(c1, c2);
}

void test_utest_assert_equals_msg_valid_types() {
    int a = 100, b = 100;
    UTEST_ASSERT_EQUALS_MSG(a, b, "Integers should be equal");
    
    std::string str1 = "test", str2 = "test";
    UTEST_ASSERT_EQUALS_MSG(str1, str2, "Strings should be equal");
}

void test_utest_assert_not_equals_valid_types() {
    int a = 42, b = 24;
    UTEST_ASSERT_NOT_EQUALS(a, b);
    
    std::string str1 = "hello", str2 = "world";
    UTEST_ASSERT_NOT_EQUALS(str1, str2);
}

void test_utest_assert_ptr_equals_valid_types() {
    int value = 42;
    int* ptr1 = &value;
    int* ptr2 = &value;  // Same address
    UTEST_ASSERT_PTR_EQUALS(ptr1, ptr2);
    
    // Test with nullptr
    int* null_ptr1 = nullptr;
    int* null_ptr2 = nullptr;
    UTEST_ASSERT_PTR_EQUALS(null_ptr1, null_ptr2);
    
    // Test with function pointers
    void (*func_ptr1)() = nullptr;
    void (*func_ptr2)() = nullptr;
    UTEST_ASSERT_PTR_EQUALS(func_ptr1, func_ptr2);
}

void test_utest_assert_ptr_equals_msg_valid_types() {
    int value = 10;
    int* ptr1 = &value;
    int* ptr2 = &value;
    UTEST_ASSERT_PTR_EQUALS_MSG(ptr1, ptr2, "Pointers should point to same object");
}

void test_utest_assert_ptr_not_equals_valid_types() {
    int value1 = 42, value2 = 24;
    int* ptr1 = &value1;
    int* ptr2 = &value2;
    UTEST_ASSERT_PTR_NOT_EQUALS(ptr1, ptr2);
    
    int* null_ptr = nullptr;
    UTEST_ASSERT_PTR_NOT_EQUALS(ptr1, null_ptr);
}

void test_utest_assert_ptr_not_equals_msg_valid_types() {
    int value1 = 1, value2 = 2;
    int* ptr1 = &value1;
    int* ptr2 = &value2;
    UTEST_ASSERT_PTR_NOT_EQUALS_MSG(ptr1, ptr2, "Pointers should be different");
}

void test_utest_assert_str_equals_valid_types() {
    // C-style strings
    const char* cstr1 = "hello";
    const char* cstr2 = "hello";
    UTEST_ASSERT_STR_EQUALS(cstr1, cstr2);
    
    // String literals
    UTEST_ASSERT_STR_EQUALS("literal1", "literal1");
    
    // std::string objects
    std::string str1 = "world";
    std::string str2 = "world";
    UTEST_ASSERT_STR_EQUALS(str1, str2);
    
    // Mixed types
    std::string stdstr = "test";
    UTEST_ASSERT_STR_EQUALS(stdstr, "test");
}

void test_utest_assert_str_equals_msg_valid_types() {
    const char* msg1 = "success";
    const char* msg2 = "success";
    UTEST_ASSERT_STR_EQUALS_MSG(msg1, msg2, "Status messages should match");
}

void test_utest_assert_str_not_equals_valid_types() {
    UTEST_ASSERT_STR_NOT_EQUALS("hello", "world");
    
    std::string str1 = "foo";
    std::string str2 = "bar";
    UTEST_ASSERT_STR_NOT_EQUALS(str1, str2);
}

void test_utest_assert_str_not_equals_msg_valid_types() {
    UTEST_ASSERT_STR_NOT_EQUALS_MSG("pass", "fail", "Different outcomes");
}

void test_edge_cases_for_equals_macros() {
    // Test with const pointers
    const int value = 42;
    const int* const_ptr1 = &value;
    const int* const_ptr2 = &value;
    UTEST_ASSERT_PTR_EQUALS(const_ptr1, const_ptr2);
    
    // Test with void pointers
    int data = 100;
    void* void_ptr1 = &data;
    void* void_ptr2 = &data;
    UTEST_ASSERT_PTR_EQUALS(void_ptr1, void_ptr2);
    
    // Test with different numeric types that should work with EQUALS
    int int_val = 5;
    long long_val = 5;
    UTEST_ASSERT_EQUALS(int_val, long_val);
    
    // Test string objects with temporary
    std::string temp_str = "temporary";
    UTEST_ASSERT_STR_EQUALS(temp_str, std::string("temporary"));
}

// ===================================================================
// DOCUMENTATION OF INVALID USAGE (THESE WOULD CAUSE COMPILE ERRORS)
// ===================================================================

void document_invalid_usage_examples() {
    // The following examples would cause compile-time errors if uncommented:
    
    /*
    // ❌ INVALID: Using UTEST_ASSERT_EQUALS with pointers
    int value = 42;
    int* ptr1 = &value;
    int* ptr2 = &value;
    UTEST_ASSERT_EQUALS(ptr1, ptr2);  // ERROR: should use UTEST_ASSERT_PTR_EQUALS
    
    // ❌ INVALID: Using UTEST_ASSERT_EQUALS with string literals  
    UTEST_ASSERT_EQUALS("str1", "str2");  // ERROR: should use UTEST_ASSERT_STR_EQUALS
    
    // ❌ INVALID: Using UTEST_ASSERT_EQUALS with C-style strings
    const char* cstr1 = "hello";
    const char* cstr2 = "world"; 
    UTEST_ASSERT_EQUALS(cstr1, cstr2);  // ERROR: should use UTEST_ASSERT_STR_EQUALS
    
    // ❌ INVALID: Using UTEST_ASSERT_PTR_EQUALS with non-pointers
    int a = 5, b = 10;
    UTEST_ASSERT_PTR_EQUALS(a, b);  // ERROR: should use UTEST_ASSERT_EQUALS
    
    // ❌ INVALID: Using UTEST_ASSERT_PTR_EQUALS with strings
    std::string str1 = "test";
    std::string str2 = "test";
    UTEST_ASSERT_PTR_EQUALS(str1, str2);  // ERROR: should use UTEST_ASSERT_STR_EQUALS
    
    // All the above would generate static_assert errors with helpful messages:
    // - "UTEST_ASSERT_EQUALS should not be used with pointers or string literals..."
    // - "UTEST_ASSERT_PTR_EQUALS can only be used with pointer types..."
    */
}

int main() {
    std::cout << "======================================\n";
    std::cout << "Testing All Equals Macros\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    // Test all valid usage patterns
    UTEST_FUNC(utest_assert_equals_valid_types);
    UTEST_FUNC(utest_assert_equals_msg_valid_types);
    UTEST_FUNC(utest_assert_not_equals_valid_types);
    UTEST_FUNC(utest_assert_ptr_equals_valid_types);
    UTEST_FUNC(utest_assert_ptr_equals_msg_valid_types);
    UTEST_FUNC(utest_assert_ptr_not_equals_valid_types);
    UTEST_FUNC(utest_assert_ptr_not_equals_msg_valid_types);
    UTEST_FUNC(utest_assert_str_equals_valid_types);
    UTEST_FUNC(utest_assert_str_equals_msg_valid_types);
    UTEST_FUNC(utest_assert_str_not_equals_valid_types);
    UTEST_FUNC(utest_assert_str_not_equals_msg_valid_types);
    UTEST_FUNC(edge_cases_for_equals_macros);
    
    // Note: Invalid usage examples are documented in comments within 
    // document_invalid_usage_examples() function, but not executed
    
    UTEST_EPILOG();
}
