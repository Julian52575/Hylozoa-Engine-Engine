cmake_minimum_required(VERSION 3.28)
project(Hylozoa-Engine-Engine
  LANGUAGES CXX)

option(HE_ENGINE_BUILD_TESTS "Build the tests" OFF)
option(HE_ENGINE_BUILD_TESTS_GRAPHIC "Build the graphical test" OFF)
option(HE_ENGINE_BUILD_MAIN_EXECUTABLE "Build the main executable" OFF)
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

if (BUILD_TESTS_GRAPHIC)
  add_subdirectory(tests/Graphic)
endif()