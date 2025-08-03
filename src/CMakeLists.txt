cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Shared library
file(GLOB_RECURSE LIB_FILES
  Placeholder.cpp
)
add_library(hylozoa_engine STATIC
  ${LIB_FILES}
)
# Add public include directories for the library
target_include_directories(hylozoa_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/Placeholder
)

## Main
add_executable(hylozoa.exe
    main.cpp
)
target_link_libraries(hylozoa.exe
    hylozoa_engine
)

message(STATUS "${Yellow}Compiling files:\n${SRC_FILES}${ColourReset}")