// Demo showcasing verbose mode in utest library
#include "utest.h"
#include <string>

UTEST_FUNC_DEF(VerboseBasicTest) {
    UTEST_ASSERT_TRUE(true);
    UTEST_ASSERT_EQUALS(2 + 2, 4);
    UTEST_ASSERT_STR_EQUALS("hello", "hello");
}

UTEST_FUNC_DEF2(VerboseDemo, PassingTest) {
    std::string greeting = "Hello, World!";
    UTEST_ASSERT_EQUALS(greeting.length(), 13);
    UTEST_ASSERT_GT(greeting.find("World"), 0);
}

UTEST_FUNC_DEF2(VerboseDemo, AnotherPassingTest) {
    int numbers[] = {1, 2, 3, 4, 5};
    UTEST_ASSERT_EQUALS(numbers[0], 1);
    UTEST_ASSERT_EQUALS(numbers[4], 5);
}

UTEST_FUNC_DEF2(VerboseDemo, MathTest) {
    UTEST_ASSERT_LT(3.14, 4.0);
    UTEST_ASSERT_GTE(10, 10);
    UTEST_ASSERT_NEQ(1, 2);
}

int main() {
    UTEST_PROLOG();
    
    // Enable verbose mode for prettier output
    UTEST_ENABLE_VERBOSE_MODE();
    
    // Enable Unicode checkmarks for prettier output
    UTEST_USE_UNICODE_CHECKMARKS();
    
    // Keep performance timing enabled (default)
    UTEST_SHOW_PERFORMANCE();
    
    std::cout << "=== Verbose Mode Demo ===\n";
    std::cout << "This demo shows test names before execution\n\n";
    
    UTEST_FUNC(VerboseBasicTest);
    UTEST_FUNC2(VerboseDemo, PassingTest);
    UTEST_FUNC2(VerboseDemo, AnotherPassingTest);
    UTEST_FUNC2(VerboseDemo, MathTest);
    
    UTEST_EPILOG();
}
