cmake_minimum_required(VERSION 3.16)
project(utest)

set(CMAKE_CXX_STANDARD 14)

add_executable(testUTest
        ../include/utest.h
        testUTest.cpp)

# Add include directory
target_include_directories(testUTest PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

ADD_TEST(testUTest, testUTest)
