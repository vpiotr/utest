cmake_minimum_required(VERSION 3.16)
project(utest)

set(CMAKE_CXX_STANDARD 14)

# Core features test
add_executable(test_core_features
        ../include/utest.h
        test_core_features.cpp)

target_include_directories(test_core_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_core_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

ADD_TEST(test_core_features, ${CMAKE_BINARY_DIR}/bin/tests/test_core_features)

# Comprehensive features test
add_executable(test_comprehensive_features
        ../include/utest.h
        test_comprehensive_features.cpp)

target_include_directories(test_comprehensive_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_comprehensive_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

ADD_TEST(test_comprehensive_features, ${CMAKE_BINARY_DIR}/bin/tests/test_comprehensive_features)

# Edge cases test
add_executable(test_edge_cases
        ../include/utest.h
        test_edge_cases.cpp)

target_include_directories(test_edge_cases PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_edge_cases PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

ADD_TEST(test_edge_cases, ${CMAKE_BINARY_DIR}/bin/tests/test_edge_cases)

# Enhanced features test
add_executable(test_enhanced_features
        ../include/utest.h
        test_enhanced_features.cpp)

target_include_directories(test_enhanced_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_enhanced_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

ADD_TEST(test_enhanced_features, ${CMAKE_BINARY_DIR}/bin/tests/test_enhanced_features)

# New features comprehensive test
add_executable(test_new_features
        ../include/utest.h
        test_new_features.cpp)

target_include_directories(test_new_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_new_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

ADD_TEST(test_new_features, ${CMAKE_BINARY_DIR}/bin/tests/test_new_features)

# No tests scenario test (should fail by design)
add_executable(test_no_tests
        ../include/utest.h
        test_no_tests.cpp)

target_include_directories(test_no_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_no_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Note: test_no_tests is expected to fail, so we don't add it to CTest
