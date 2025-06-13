#include "utest.h"
#include <iostream>
#include <string>
#include <functional>

// Test basic string contains and not contains functionality
UTEST_FUNC_DEF(StringContains) {
    std::string text = "The quick brown fox jumps over the lazy dog";
    
    // Test contains with various types
    UTEST_ASSERT_STR_CONTAINS(text, "brown fox");
    UTEST_ASSERT_STR_CONTAINS(text, "quick");
    UTEST_ASSERT_STR_CONTAINS(text, "lazy dog");
    
    // Test contains with different string types
    const char* ctext = "The quick brown fox jumps over the lazy dog";
    UTEST_ASSERT_STR_CONTAINS(ctext, "brown");
    UTEST_ASSERT_STR_CONTAINS(text, std::string("fox"));
    UTEST_ASSERT_STR_CONTAINS(std::string("Testing string"), "string");
    
    // Test STR_NOT_CONTAINS assertions
    UTEST_ASSERT_STR_NOT_CONTAINS(text, "elephant");
    UTEST_ASSERT_STR_NOT_CONTAINS(text, "zebra");
    UTEST_ASSERT_STR_NOT_CONTAINS("Simple test", "complex");
    
    // Test short alias forms
    UTEST_ASSERT_SC(text, "jumps");
    UTEST_ASSERT_SNC(text, "giraffe");
}

// Test string contains with custom messages
UTEST_FUNC_DEF(StringContainsWithMessage) {
    std::string response = "Status: OK, Results: 5 items found";
    
    // Test with custom messages
    UTEST_ASSERT_STR_CONTAINS_MSG(response, "OK", "Response should indicate success");
    UTEST_ASSERT_STR_CONTAINS_MSG(response, "Results", "Response should include results section");
    UTEST_ASSERT_STR_CONTAINS_MSG(response, "5 items", "Response should show correct item count");
    
    // Test not contains with custom messages
    UTEST_ASSERT_STR_NOT_CONTAINS_MSG(response, "Error", "Response should not contain errors");
    UTEST_ASSERT_STR_NOT_CONTAINS_MSG(response, "Failed", "Response should not indicate failure");
    UTEST_ASSERT_STR_NOT_CONTAINS_MSG(response, "0 items", "Response should not show zero items");
}

// Test edge cases for string contains assertions
UTEST_FUNC_DEF(StringContainsEdgeCases) {
    // Empty string tests
    std::string emptyStr = "";
    std::string nonEmptyStr = "This is a test";
    
    // Empty string should contain empty substring
    UTEST_ASSERT_STR_CONTAINS(emptyStr, "");
    
    // Non-empty string should contain empty substring
    UTEST_ASSERT_STR_CONTAINS(nonEmptyStr, "");
    
    // Empty string should not contain any non-empty substring
    UTEST_ASSERT_STR_NOT_CONTAINS(emptyStr, "test");
    
    // Case sensitivity tests
    std::string mixedCaseStr = "The Quick Brown Fox";
    UTEST_ASSERT_STR_CONTAINS(mixedCaseStr, "Quick");
    UTEST_ASSERT_STR_NOT_CONTAINS(mixedCaseStr, "quick"); // Should fail if case-insensitive
    
    // Boundary tests
    std::string boundaryStr = "TestString";
    UTEST_ASSERT_STR_CONTAINS(boundaryStr, "Test");        // Start of string
    UTEST_ASSERT_STR_CONTAINS(boundaryStr, "String");      // End of string
    UTEST_ASSERT_STR_CONTAINS(boundaryStr, "TestString");  // Full string
    
    // Numeric and special character tests
    std::string specialStr = "Special chars: !@#$%^&*() and numbers: 1234567890";
    UTEST_ASSERT_STR_CONTAINS(specialStr, "!@#$%^&*()");
    UTEST_ASSERT_STR_CONTAINS(specialStr, "1234567890");
    
    // Unicode character tests if supported
    std::string unicodeStr = "Unicode symbols: € £ ¥ © ®";
    UTEST_ASSERT_STR_CONTAINS(unicodeStr, "€ £ ¥");
    UTEST_ASSERT_STR_CONTAINS(unicodeStr, "©");
}

// Test string contains with different data types
UTEST_FUNC_DEF(StringContainsDifferentTypes) {
    // Standard string types
    std::string stdStr = "Testing with std::string";
    const char* cStr = "Testing with const char*";
    char mutableStr[] = "Testing with mutable char array";
    
    // String/string combinations
    UTEST_ASSERT_STR_CONTAINS(stdStr, std::string("std::string"));
    UTEST_ASSERT_STR_CONTAINS(stdStr, "string");
    
    // Const char*/string combinations
    UTEST_ASSERT_STR_CONTAINS(cStr, std::string("const char*"));
    UTEST_ASSERT_STR_CONTAINS(cStr, "Testing");
    
    // Mutable char array/string combinations
    UTEST_ASSERT_STR_CONTAINS(mutableStr, std::string("mutable"));
    UTEST_ASSERT_STR_CONTAINS(mutableStr, "array");
    
    // Numeric to string conversions
    int num = 12345;
    std::string numStr = "Number: 12345";
    UTEST_ASSERT_STR_CONTAINS(numStr, std::to_string(num));
    
    // Special handling for single characters
    char singleChar = 'X';
    std::string charStr = "Testing X character";
    UTEST_ASSERT_STR_CONTAINS(charStr, std::string(1, singleChar));
}

// Function to verify expected exceptions from contains assertions
UTEST_FUNC_DEF(StringContainsExpectedFailures) {
    std::string testStr = "This is a test string";
    bool exceptionCaught = false;
    
    // Test for expected failure of UTEST_ASSERT_STR_CONTAINS
    try {
        UTEST_ASSERT_STR_CONTAINS(testStr, "not present");
    } catch (const utest::AssertionException& e) {
        exceptionCaught = true;
        std::string errorMsg = e.what();
        UTEST_ASSERT_STR_CONTAINS(errorMsg, "does not contain");
    }
    UTEST_ASSERT_TRUE(exceptionCaught);
    
    // Reset exception flag and test for expected failure of UTEST_ASSERT_STR_NOT_CONTAINS
    exceptionCaught = false;
    try {
        UTEST_ASSERT_STR_NOT_CONTAINS(testStr, "test");
    } catch (const utest::AssertionException& e) {
        exceptionCaught = true;
        std::string errorMsg = e.what();
        UTEST_ASSERT_STR_CONTAINS(errorMsg, "contains");
    }
    UTEST_ASSERT_TRUE(exceptionCaught);
    
    // Reset exception flag and test for expected failure of message variant
    exceptionCaught = false;
    try {
        UTEST_ASSERT_STR_CONTAINS_MSG(testStr, "missing", "Custom error message");
    } catch (const utest::AssertionException& e) {
        exceptionCaught = true;
        std::string errorMsg = e.what();
        UTEST_ASSERT_STR_CONTAINS(errorMsg, "Custom error message");
    }
    UTEST_ASSERT_TRUE(exceptionCaught);
}

int main() {
    std::cout << "======================================\n";
    std::cout << "String Contains/Not Contains Tests\n";
    std::cout << "======================================\n\n";
    
    UTEST_PROLOG();
    
    std::cout << "Testing string contains/not contains assertions...\n";
    UTEST_FUNC(StringContains);
    
    std::cout << "\nTesting string contains with custom messages...\n";
    UTEST_FUNC(StringContainsWithMessage);
    
    std::cout << "\nTesting string contains edge cases...\n";
    UTEST_FUNC(StringContainsEdgeCases);
    
    std::cout << "\nTesting string contains with different data types...\n";
    UTEST_FUNC(StringContainsDifferentTypes);
    
    std::cout << "\nTesting string contains expected failures...\n";
    UTEST_FUNC(StringContainsExpectedFailures);
    
    UTEST_EPILOG();
}
