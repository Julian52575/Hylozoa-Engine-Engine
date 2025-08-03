cmake_minimum_required(VERSION 3.14)

option(BUILD_TESTS "Build the tests" OFF)
# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add tests

add_subdirectory(src)
# Enable testing only if requested
if (BUILD_TESTS)
  add_subdirectory(tests)
endif()