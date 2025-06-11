add_executable(utest_demo
    demo_main.cpp
)

target_link_libraries(utest_demo
    PRIVATE utest
)

# Add include directory
target_include_directories(utest_demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Set output directory
set_target_properties(utest_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Demo with intentional failure
add_executable(demo_with_failure
    demo_with_failure.cpp
)

target_link_libraries(demo_with_failure
    PRIVATE utest
)

target_include_directories(demo_with_failure PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(demo_with_failure PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Demo with no tests
add_executable(demo_no_tests
    demo_no_tests.cpp
)

target_link_libraries(demo_no_tests
    PRIVATE utest
)

target_include_directories(demo_no_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set_target_properties(demo_no_tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)