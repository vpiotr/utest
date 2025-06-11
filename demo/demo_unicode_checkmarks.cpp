// Demo showcasing Unicode checkmarks in utest library
#include "utest.h"
#include <string>

UTEST_FUNC_DEF(UnicodeBasicTest) {
    UTEST_ASSERT_TRUE(true);
    UTEST_ASSERT_EQUALS(2 + 2, 4);
    UTEST_ASSERT_STR_EQUALS("hello", "hello");
}

UTEST_FUNC_DEF2(UnicodeDemo, PassingTest) {
    std::string greeting = "Hello, World!";
    UTEST_ASSERT_EQUALS(greeting.length(), 13);
    UTEST_ASSERT_GT(greeting.find("World"), 0);
}

UTEST_FUNC_DEF2(UnicodeDemo, AnotherPassingTest) {
    int numbers[] = {1, 2, 3, 4, 5};
    UTEST_ASSERT_EQUALS(numbers[0], 1);
    UTEST_ASSERT_EQUALS(numbers[4], 5);
}

UTEST_FUNC_DEF2(UnicodeDemo, MathTest) {
    UTEST_ASSERT_LT(3.14, 4.0);
    UTEST_ASSERT_GTE(10, 10);
    UTEST_ASSERT_NEQ(1, 2);
}

int main() {
    UTEST_PROLOG();
    
    // Enable Unicode checkmarks for prettier output
    UTEST_USE_UNICODE_CHECKMARKS();
    
    // Keep performance timing enabled (default)
    UTEST_SHOW_PERFORMANCE();
    
    std::cout << "=== Unicode Checkmarks Demo ===\n";
    std::cout << "This demo shows ✓/✗ instead of [OK]/[FAIL]\n\n";
    
    UTEST_FUNC(UnicodeBasicTest);
    UTEST_FUNC2(UnicodeDemo, PassingTest);
    UTEST_FUNC2(UnicodeDemo, AnotherPassingTest);
    UTEST_FUNC2(UnicodeDemo, MathTest);
    
    UTEST_EPILOG();
}
