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

# Use generator expression to handle .exe extension on Windows
add_test(NAME test_core_features 
         COMMAND $<TARGET_FILE:test_core_features>)

# Comprehensive features test
add_executable(test_comprehensive_features
        ../include/utest.h
        test_comprehensive_features.cpp)

target_include_directories(test_comprehensive_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_comprehensive_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

add_test(NAME test_comprehensive_features 
         COMMAND $<TARGET_FILE:test_comprehensive_features>)

# Edge cases test
add_executable(test_edge_cases
        ../include/utest.h
        test_edge_cases.cpp)

target_include_directories(test_edge_cases PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_edge_cases PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

add_test(NAME test_edge_cases 
         COMMAND $<TARGET_FILE:test_edge_cases>)

# Enhanced features test
add_executable(test_enhanced_features
        ../include/utest.h
        test_enhanced_features.cpp)

target_include_directories(test_enhanced_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_enhanced_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

add_test(NAME test_enhanced_features 
         COMMAND $<TARGET_FILE:test_enhanced_features>)

# New features comprehensive test
add_executable(test_new_features
        ../include/utest.h
        test_new_features.cpp)

target_include_directories(test_new_features PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_new_features PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Use generator expression to handle .exe extension on Windows
add_test(NAME test_new_features 
         COMMAND $<TARGET_FILE:test_new_features>)

# No tests scenario test (should fail by design)
add_executable(test_no_tests
        ../include/utest.h
        test_no_tests.cpp)

target_include_directories(test_no_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_no_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Note: test_no_tests is expected to fail, so we don't add it to CTest

# String contains test
add_executable(test_string_contains
        ../include/utest.h
        test_string_contains.cpp)

target_include_directories(test_string_contains PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_string_contains PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Use generator expression to handle .exe extension on Windows
add_test(NAME test_string_contains 
         COMMAND $<TARGET_FILE:test_string_contains>)

# Equals macros test
add_executable(test_equals_macros
        ../include/utest.h
        test_equals_macros.cpp)

target_include_directories(test_equals_macros PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(test_equals_macros PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests"
)

# Use generator expression to handle .exe extension on Windows
add_test(NAME test_equals_macros 
         COMMAND $<TARGET_FILE:test_equals_macros>)
