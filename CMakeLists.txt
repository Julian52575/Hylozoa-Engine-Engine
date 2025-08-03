cmake_minimum_required(VERSION 3.14)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Shared library
project(Hylozoa-Engine)
file(GLOB_RECURSE LIB_FILES
  src/Placeholder.cpp
)

add_library(hylozoa_engine.so
  ${LIB_FILES}
)

message(STATUS "${Yellow}Compiling files:\n${SRC_FILES}${ColourReset}")

## Main
add_executable( hylozoa.exe
    src/main.cpp
)
target_link_libraries(hylozoa.exe
  hylozoa_engine.so
)