cmake_minimum_required(VERSION 3.14)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

project(Hylozoa-Engine)
file(GLOB_RECURSE SRC_FILES src/*.cpp)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "${Yellow}Compiling files:\n${SRC_FILES}${ColourReset}")

add_executable( hylozoa.exe
    ${SRC_FILES}
)
target_link_libraries(hylozoa.exe
)
