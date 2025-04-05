add_executable(utest_demo
    demo_main.cpp
)

target_link_libraries(utest_demo
    PRIVATE utest
)

# Set output directory
set_target_properties(utest_demo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)