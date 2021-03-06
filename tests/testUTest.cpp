//----------------------------------------------------------------------------------
// Name:        testUTest.cpp
// Purpose:     Tests unit test lib
// Author:      Piotr Likus
// Created:     31/08/2020
// Last change:
//----------------------------------------------------------------------------------

#include "utest.h"

void test_assert_true() {
    int a{42};
    UTEST_ASSERT_TRUE(a == 42);
}

void test_assert_false() {
    int a{42};
    UTEST_ASSERT_FALSE(a == 41);
}

void test_assert_true_msg() {
    int a{42};
    UTEST_ASSERT_TRUE_MSG("a should be equal to 42", a == 42);
}

void test_assert_false_msg() {
    int a{42};
    UTEST_ASSERT_FALSE_MSG("a should be different than 35", a == 35);
}

void test_assert_equals() {
    int a{42};
    UTEST_ASSERT_EQUALS(a, 42);
}

void test_assert_equals_msg() {
    int a{42};
    UTEST_ASSERT_EQUALS_MSG("a should be equal to 42", a, 42);
}

void test_assert_throws() {
    UTEST_ASSERT_THROWS([]() { throw std::exception(); });
}

void test_assert_throws_msg() {
    auto lambdaX = []() { throw std::exception(); };
    UTEST_ASSERT_THROWS_MSG("Lambda execution throws", lambdaX);
}

int run_all_tests() {
    UTEST_PROLOG();
    UTEST_FUNC(assert_true);
    UTEST_FUNC(assert_false);
    UTEST_FUNC(assert_true_msg);
    UTEST_FUNC(assert_false_msg);
    UTEST_FUNC(assert_equals);
    UTEST_FUNC(assert_equals_msg);
    UTEST_FUNC(assert_throws);
    UTEST_FUNC(assert_throws_msg);
    UTEST_EPILOG();
}

int main() {
    return run_all_tests();
}

