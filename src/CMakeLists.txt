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
## Edit the list of file here:
set(LIB_FILES
  Placeholder/Placeholder.cpp
)
# Add Hylozoa-Engine/ to file path
list(TRANSFORM LIB_FILES PREPEND "Hylozoa-Engine/")
add_library(hylozoa_engine STATIC
  ${LIB_FILES}
)
# Add include directories for the library
target_include_directories(hylozoa_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

## Main
add_executable(hylozoa.exe
    main.cpp
)
target_link_libraries(hylozoa.exe
    hylozoa_engine
)

message(STATUS "${Yellow}Compiling files:\n${LIB_FILES}${ColourReset}")
