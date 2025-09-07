cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine)
include(FetchContent)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
## Flecs ECS github tag
## Can be a branch, a commit or a release
message(STATUS "${Yellow}[SRC] Fetching lib 'Flecs' at tag '$ENV{FLECS_GIT_TAG}'. This might take a while...${ColourReset}")
FetchContent_Declare(flecs
  GIT_REPOSITORY git@github.com:SanderMertens/flecs.git
  GIT_TAG $ENV{FLECS_GIT_TAG}
)
FetchContent_MakeAvailable(flecs)

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
    flecs
)
target_link_libraries(hylozoa_engine
  flecs::flecs_static
)

## Main
add_executable(hylozoa.exe
    main.cpp
)
target_link_libraries(hylozoa.exe
    hylozoa_engine
)

message(STATUS "${Yellow}Compiling files:\n${LIB_FILES}${ColourReset}")
