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
message(STATUS "Fetching googlebenchmark...")
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.4
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(googlebenchmark)
set(GOOGLEBENCHMARK_INCLUDE_DIR "${googlebenchmark_SOURCE_DIR}/include")

## Benchmarks
file(GLOB_RECURSE SRC_FILES
  src/*.cpp
) # Tip: CMake won't complain if a file cannot be found. Make sure the path is correct
message(STATUS "Compiling benchmarks with source files: ${SRC_FILES}") 
add_executable( benchmarkSuite
    ${SRC_FILES}
)
message(STATUS "DEBUG: ${HE_LIBRARY_NAME}") 
target_link_libraries(benchmarkSuite PUBLIC
    benchmark::benchmark #google benchmark
    ${HE_LIBRARY_NAME}
)
target_include_directories(benchmarkSuite PUBLIC
    ${GOOGLEBENCHMARK_INCLUDE_DIR}
)