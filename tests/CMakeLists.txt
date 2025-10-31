cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)
enable_testing()
  
## Tests
file(GLOB_RECURSE SRC_FILES
  src/helloWorld_test.cpp
  src/Placeholder/placeholder_test.cpp
) # Tip: CMake won't complain if a file cannot be found. Make sure the path is correct
add_executable( testSuite
    ${SRC_FILES}
)
target_link_libraries(testSuite
    GTest::gtest_main
    hylozoa_engine
)

include(GoogleTest)
gtest_discover_tests(testSuite)