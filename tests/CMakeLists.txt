cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.17.0
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googletest)
enable_testing()
  
## Tests
message(STATUS "Compiling tests with source files: ${SRC_FILES}") 
file(GLOB_RECURSE SRC_FILES
  src/HeadlessEngine/Time.cpp
  src/HeadlessEngine/Input.cpp
) # Tip: CMake won't complain if a file cannot be found. Make sure the path is correct
add_executable( testSuite
    ${SRC_FILES}
)
target_link_libraries(testSuite
    GTest::gtest_main
    hylozoa_engine
    glm
)

include(GoogleTest)
gtest_discover_tests(testSuite)