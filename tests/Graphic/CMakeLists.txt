cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Graphic-Tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest
#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
#FetchContent_MakeAvailable(googletest)
#enable_testing()
  
## Tests
file(GLOB_RECURSE SRC_FILES
  ./main.cpp
) # Tip: CMake won't complain if a file cannot be found. Make sure the path is correct
add_executable( graphicTest
    ${SRC_FILES}
)
target_include_directories(graphicTest PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    hylozoa_engine
    SDL3
    glm
)
target_link_libraries(graphicTest
    hylozoa_engine
    SDL3
)

#include(GoogleTest)
#gtest_discover_tests(graphicTest)