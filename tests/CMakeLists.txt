cmake_minimum_required(VERSION 3.16)
project(utest)

set(CMAKE_CXX_STANDARD 14)

add_executable(testUTest
        ../include/utest.h
        testUTest.cpp)

ADD_TEST(testUTest, testUTest)
