# Overview
Micro (header-only) library for unit testing.

Library author: Piotr Likus

Created: 31/08/2020

Initial code based on blog post by Bastian Rieck, 
see: https://bastian.rieck.me/blog/posts/2017/simple_unit_tests/

# Requirements
This library requires at least C++11 and has been tested on Linux Mint 20.
 
# Project home
https://github.com/vpiotr/utest

# How to run example

Included test runner "testUTest" is both a sample for and a test of this library.
In order to execute it:

    cd ~/tmp
    git clone https://github.com/vpiotr/utest.git
    cd utest
    mkdir _build
    cd _build
    cmake ..
    # to build test runner executables
    make all
    # to execute all test runners 
    make test
    # to execute specific runner
    tests/testUTest

# How to use library
This library is header-only solution, so you just need to include single header file `utest.h` in your test runner code.

Create a runner application executing single function which executes all tests plus adds prolog (UTEST_PROLOG) and epilog (UTEST_EPILOG) code.
Each test must be prepared as a function called `test_aaa` and must be registered using `UTEST_FUNC` macro.

Example complete test runner:

    #include "utest.h"
    
    void test_assert_equals() {
        int a{42};
        UTEST_ASSERT_EQUALS(a, 42);
    }
    
    int run_all_tests() {
        UTEST_PROLOG();
        UTEST_FUNC(assert_equals);
        UTEST_EPILOG();
    }
    
    int main() {
        return run_all_tests();
    }
 
See tests in "tests" subdirectory for details.

There are two ways of runner execution:
1) As standard application - from IDE or console, this will show which tests are failing, example:

        OK: Test [assert_true] succeeded
        OK: Test [assert_false] succeeded
        OK: Test [assert_true_msg] succeeded
        OK: Test [assert_false_msg] succeeded
        Error: Test [assert_equals] failed!, error: Assertion failed,  at /home/piotr/tmp/utest/tests/testUTest.cpp:33 in void test_assert_equals(): 42 != 43
        OK: Test [assert_equals_msg] succeeded
        OK: Test [assert_throws] succeeded
        OK: Test [assert_throws_msg] succeeded
        Failures!

2) As `make test` - this will only show which runners are failing 
 
        piotr@piotr-Prec-M47:~/tmp/utest/_build$ make test
        Running tests...
        Test project /home/piotr/tmp/utest/_build
            Start 1: testUTest,
        1/1 Test #1: testUTest, .......................***Failed    0.00 sec
        
        0% tests passed, 1 tests failed out of 1
        
        Total Test time (real) =   0.00 sec
        
        The following tests FAILED:
              1 - testUTest, (Failed)
        Errors while running CTest
        make: *** [Makefile:84: test] Error 8
 
This second type of execution requires two things in CMakeList.txt:
* in runner directory in CMakeList.txt add runner as a test:

        ADD_TEST(testUTest, testUTest)

* in main project directory (from where you want to execute tests) in CMakeList.txt add:

        ENABLE_TESTING()
        ADD_SUBDIRECTORY( tests )
 
 where `tests` is a directory containing runner code.
 
# License
See LICENSE.txt