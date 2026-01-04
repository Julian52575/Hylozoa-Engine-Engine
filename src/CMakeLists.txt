cmake_minimum_required(VERSION 3.28) #4.21)
project(Hylozoa-Engine)
include(FetchContent)

Set(FETCHCONTENT_QUIET FALSE)

# Colors output
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(Yellow      "${Esc}[33m")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## glm
message(STATUS "Fetching gLm...")
FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.2
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glm)
FetchContent_GetProperties(glm)
set(glm_INCLUDE_DIR "${glm_SOURCE_DIR}")

## sdl3
message(STATUS "Fetching SDL3...")
FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG preview-3.3.2
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3)
FetchContent_GetProperties(SDL3)
set(SDL3_INCLUDE_DIR "${SDL3_SOURCE_DIR}/include")

## EnTT
message(STATUS "Fetching EnTT...")
FetchContent_Declare(
        EnTT
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.12.2 # or the version you want
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(EnTT)
FetchContent_GetProperties(EnTT)
set(EnTT_INCLUDE_DIR "${EnTT_SOURCE_DIR}/src")

## box2d
message(STATUS "Fetching box2d...")
FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.0 # or the version you want
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)
FetchContent_GetProperties(box2d)
if(box2d_POPULATED)
  set(BOX2D_INCLUDE_DIR "${box2d_SOURCE_DIR}/include")
endif()

## bgfx
FetchContent_Declare(
        bgfx
        GIT_REPOSITORY https://github.com/bkaradzic/bgfx.cmake.git
        GIT_TAG v1.135.9062-502
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(bgfx)
FetchContent_GetProperties(bgfx)
set(bgfx_INCLUDE_DIR "${bgfx_SOURCE_DIR}")

## Shared library
## Edit the list of file here:
set(LIB_FILES
        Placeholder/Placeholder.cpp
        Components/Rendering/Renderable.cpp
        Core/Engine.cpp
        Core/Entity.cpp
        Core/Input.cpp
        Core/Time.cpp
        Core/Scene.cpp
        Systems/Transform/Transform.cpp
        Systems/Physics/Collision.cpp
        Components/Transform/Transform.cpp
        Components/Scene/UUID.cpp
        SDL/SDL_Manager.cpp
        Systems/Renderer/Renderer.cpp
        Systems/Movement/Movement.cpp
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
        ${SDL3_INCLUDE_DIR}
        ${glm_INCLUDE_DIR}
        ${EnTT_INCLUDE_DIR}
        ${bgfx_INCLUDE_DIR}
)
target_link_libraries(hylozoa_engine PRIVATE
        SDL3::SDL3
        glm::glm
        EnTT::EnTT
        box2d
        bgfx
)

## Main
add_executable(hylozoa.exe
        main.cpp
)
target_link_libraries(hylozoa.exe
        hylozoa_engine
)

if(WIN32)
  add_custom_command(TARGET hylozoa.exe POST_BUILD
          COMMAND "${CMAKE_COMMAND}" -E copy_if_different $<TARGET_FILE:SDL3::SDL3> $<TARGET_FILE_DIR:hylozoa.exe>
          VERBATIM
  )
endif()


message(STATUS "${Yellow}Compiling files:\n${LIB_FILES}${ColourReset}")