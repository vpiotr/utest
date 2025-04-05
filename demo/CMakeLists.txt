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