cmake_minimum_required(VERSION 3.25)
project(TSML)

set(CMAKE_CXX_STANDARD 17)
set(TEST_STATUS ON)

add_subdirectory(include)

if(TEST_STATUS)
    enable_testing()
    message("Tests for TSML is starting.")
    add_subdirectory(tests)
endif()
