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

# Build and Installation

## Building the demo and tests

```bash
mkdir _build && cd _build
cmake ..
cmake --build .

# Run the demo
./bin/utest_demo

# Run the tests
ctest
# or
make test
```

## Installing the library

```bash
mkdir _build && cd _build
cmake ..
cmake --build .
sudo cmake --install .
```

## CMake Options

- `UTEST_BUILD_DEMO`: Build the demo project (ON by default)
- `UTEST_BUILD_TESTS`: Build the tests (ON by default)

Example with options:
```bash
cmake -DUTEST_BUILD_DEMO=OFF -DUTEST_BUILD_TESTS=ON ..
```

# Integration

## Method 1: Direct inclusion

Simply copy `include/utest.h` into your project and include it.

## Method 2: CMake integration

Add utest as a subdirectory in your CMake project:

```cmake
add_subdirectory(path/to/utest)
target_link_libraries(your_project PRIVATE utest)
```

Or, if installed:

```cmake
find_package(utest REQUIRED)
target_link_libraries(your_project PRIVATE utest::utest)
```

# How to use library
This library is header-only solution, so you just need to include single header file `utest.h` in your test runner code.

Create a runner application executing single function which executes all tests plus adds prolog (UTEST_PROLOG) and epilog (UTEST_EPILOG) code.
Each test must be prepared as a function called `test_aaa` and must be registered using `UTEST_FUNC` macro.

Example complete test runner:

```cpp
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
```
 
See tests in "tests" subdirectory and the "demo" directory for more examples.

## Demo Project

The demo project demonstrates various aspects of the library:
- Basic assertions (equality, true/false)
- Assertions with custom error messages
- Testing exceptions
- Complex test scenarios with collections

To run the demo:
```bash
mkdir _build && cd _build
cmake ..
cmake --build .
./bin/utest_demo
```

## Test Output

There are two ways of viewing test results:

1) As standard application - from IDE or console, this will show which tests are failing, example:

```
OK: Test [assert_true] succeeded
OK: Test [assert_false] succeeded
OK: Test [assert_true_msg] succeeded
OK: Test [assert_false_msg] succeeded
Error: Test [assert_equals] failed!, error: Assertion failed,  at /home/piotr/tmp/utest/tests/testUTest.cpp:33 in void test_assert_equals(): 42 != 43
OK: Test [assert_equals_msg] succeeded
OK: Test [assert_throws] succeeded
OK: Test [assert_throws_msg] succeeded
Failures!
```

2) As `make test` or `ctest` - this will only show which runners are failing 
 
```
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
```

## Available macros

- `UTEST_PROLOG()` - Initialize the test framework
- `UTEST_EPILOG()` - Finalize and report test results
- `UTEST_FUNC(name)` - Run a test function (function should be named `test_name`)
- `UTEST_ALLOW_EMPTY_TESTS()` - Allow test runner to succeed even when no tests are run
- `UTEST_ASSERT_TRUE(condition)` - Assert that condition is true
- `UTEST_ASSERT_TRUE_MSG(condition, msg)` - Assert that condition is true, with custom message
- `UTEST_ASSERT_FALSE(condition)` - Assert that condition is false
- `UTEST_ASSERT_FALSE_MSG(condition, msg)` - Assert that condition is false, with custom message
- `UTEST_ASSERT_EQUALS(a, b)` - Assert that a equals b
- `UTEST_ASSERT_EQUALS_MSG(a, b, msg)` - Assert that a equals b, with custom message
- `UTEST_ASSERT_THROWS(F)` - Assert that function F throws an exception
- `UTEST_ASSERT_THROWS_MSG(F, MSG)` - Assert that function F throws an exception, with custom message
- `UTEST_ASSERT_DOES_NOT_THROW(F)` - Assert that function F does not throw an exception
- `UTEST_ASSERT_DOES_NOT_THROW_MSG(F, MSG)` - Assert that function F does not throw an exception, with custom message
- `UTEST_ASSERT_NULL(ptr)` - Assert that pointer ptr is null
- `UTEST_ASSERT_NOT_NULL(ptr)` - Assert that pointer ptr is not null

## Build Organization

The build system now organizes binaries into dedicated subdirectories:
- Main demo: `bin/utest_demo`
- Additional demos: `bin/demo_with_failure`, `bin/demo_no_tests`
- Tests: `bin/tests/test_*`

## Handling No Tests Scenario

By default, if no tests are run, the test framework will exit with failure. This can be changed using:

```cpp
UTEST_PROLOG();
UTEST_ALLOW_EMPTY_TESTS(); // Allow success even with no tests
// No test functions called here
UTEST_EPILOG(); // Will return SUCCESS instead of FAILURE
```

This is useful for:
- Conditional test execution
- Test suites that may be empty under certain conditions
- Template-based test generation where some configurations may produce no tests

# License
See LICENSE.txt