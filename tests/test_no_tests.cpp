#include "utest.h"
#include <iostream>

int main() {
    std::cout << "Testing no tests scenario (default behavior - should fail)...\n";
    
    UTEST_PROLOG();
    UTEST_ALLOW_EMPTY_TESTS(); // Allow empty tests to succeed
    
    // No tests run here - this should not fail 
    
    UTEST_EPILOG();
}
