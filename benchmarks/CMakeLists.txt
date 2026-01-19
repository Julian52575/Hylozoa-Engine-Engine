cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine-Benchmarks
    LANGUAGES CXX
)
include(FetchContent)

# Google Benchmark
set(FLAGS "-O3 \
    -fno-omit-frame-pointer \
    -fsanitize=address \
    -fsanitize=thread \
    -fno-sanitize-recover=all \
    -pedantic \
    -pthread
")

set(CMAKE_C_FLAGS="")
set(CMAKE_CXX_FLAGS="-g -O2 -fno-omit-frame-pointer -fsanitize=address -fsanitize=thread -fno-sanitize-recover=all")

set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googlebenchmark)
set(GOOGLEBENCHMARK_INCLUDE_DIR "${googlebenchmark_SOURCE_DIR}/include")

## Benchmarks
message(STATUS "Compiling benchmarks with source files: ${SRC_FILES}") 
file(GLOB_RECURSE SRC_FILES
  src/helloworld.cpp
) # Tip: CMake won't complain if a file cannot be found. Make sure the path is correct
add_executable( benchmarkSuite
    ${SRC_FILES}
)
target_link_libraries(benchmarkSuite
    benchmark::benchmark #google benchmark
    hylozoa_engine
)
target_include_directories(benchmarkSuite PUBLIC
    hylozoa_engine
    ${GOOGLEBENCHMARK_INCLUDE_DIR}
)