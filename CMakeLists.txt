cmake_minimum_required(VERSION 3.28)
project(Hylozoa-Engine-Engine
  LANGUAGES CXX)

option(HE_ENGINE_BUILD_TESTS "Build the tests" OFF)
option(HE_ENGINE_BUILD_MAIN_EXECUTABLE "Build the main executable" OFF)
option(HE_ENGINE_BUILD_BENCHMARKS "Build the benchmarks" OFF)
option(HE_ENGINE_HIDE_SYMBOLS "Hide symbols of the lib" OFF)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
    COMMAND rustc --print host-tuple
    OUTPUT_VARIABLE RUST_TARGET_TRIPLE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(HE_LIBRARY_NAME "hylozoa_engine-${RUST_TARGET_TRIPLE}")
message(STATUS "Building library ${HE_LIBRARY_NAME}")

add_subdirectory(src)
# Enable testing only if requested
if (HE_ENGINE_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if (HE_ENGINE_BUILD_BENCHMARKS)
  add_subdirectory(benchmarks)
endif()