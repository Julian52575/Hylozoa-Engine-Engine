cmake_minimum_required(VERSION 3.14)
project(Hylozoa-Engine)
include(FetchContent)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

find_package(EnTT REQUIRED)

FetchContent_Declare(
  box2d
  GIT_REPOSITORY https://github.com/erincatto/box2d.git
  GIT_TAG v3.1.0 # or the version you want
)
FetchContent_MakeAvailable(box2d)
FetchContent_GetProperties(box2d)
if(box2d_POPULATED)
  set(BOX2D_INCLUDE_DIR "${box2d_SOURCE_DIR}/include")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## Shared library 
## Edit the list of file here:
set(LIB_FILES
  Placeholder/Placeholder.cpp
  Components/Rendering/Renderable.cpp
  Core/Engine.cpp
  Core/Entity.cpp
  Systems/Transform/Transform.cpp
  Systems/Physics/Collision.cpp
  Components/Transform/Transform.cpp
  SDL/SDL.cpp
)
# Add Hylozoa-Engine/ to file path
list(TRANSFORM LIB_FILES PREPEND "Hylozoa-Engine/")
add_library(hylozoa_engine STATIC
  ${LIB_FILES}
)
# Add include directories for the library
target_include_directories(hylozoa_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOX2D_INCLUDE_DIR}
    SDL3
    glm
)
target_link_libraries(hylozoa_engine PRIVATE
  SDL3
  EnTT::EnTT
  box2d
)

## Main
add_executable(hylozoa.exe
    main.cpp
)
target_link_libraries(hylozoa.exe
    hylozoa_engine
)

message(STATUS "${Yellow}Compiling files:\n${LIB_FILES}${ColourReset}")