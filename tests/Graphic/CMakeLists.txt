cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Graphic-Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
  
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
)
target_link_libraries(graphicTest
    hylozoa_engine
)

#include(GoogleTest)
#gtest_discover_tests(graphicTest)