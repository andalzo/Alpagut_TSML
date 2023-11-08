include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(TSML_TEST
        ThreadNodes/ThreadNodeA.cpp
        ThreadNodes/ThreadNodeA.h
        ThreadNodes/ThreadNodeB.cpp
        ThreadNodes/ThreadNodeB.h
        ThreadNodes/TestThreadMessages.h
        ThreadNodes/CustomException.cpp
        ThreadNodes/CustomException.h
        test_main.cpp
        ThreadNodes/MainNodeTest.cpp
        ThreadNodes/MainNodeTest.h)

target_link_libraries(TSML_TEST
        PRIVATE
        GTest::GTest
        TSML)

target_include_directories(TSML_TEST PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_test(TSML_TESTS TSML_TEST)