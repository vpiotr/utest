cmake_minimum_required(VERSION 3.16)
project(utest VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Set specific output directories for different configurations
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach()

# Options
option(UTEST_BUILD_DEMO "Build the utest demo project" ON)
option(UTEST_BUILD_TESTS "Build the utest tests" ON)

# Header-only library target
add_library(utest INTERFACE)
target_include_directories(utest
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Tests
if(UTEST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Demo project
if(UTEST_BUILD_DEMO)
    add_subdirectory(demo)
endif()

# Installation
include(GNUInstallDirs)
install(FILES include/utest.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export package for find_package
install(TARGETS utest
    EXPORT utestTargets
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT utestTargets
    FILE utestTargets.cmake
    NAMESPACE utest::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utest
)

# Generate package config files
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/utestConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/utestConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/utestConfig.cmake"
    @ONLY
)

# Install config files
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/utestConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/utestConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/utest
)
