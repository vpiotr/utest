cmake_minimum_required(VERSION 3.16)
project(utest)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

ENABLE_TESTING()
ADD_SUBDIRECTORY( tests )
