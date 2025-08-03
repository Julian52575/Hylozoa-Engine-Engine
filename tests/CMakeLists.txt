cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Tests)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()

add_executable( testSuite
    ${SRC_FILES}
)
target_link_libraries(testSuite
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testSuite)
