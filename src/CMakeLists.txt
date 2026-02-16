cmake_minimum_required(VERSION 3.28)
project(Hylozoa-Engine-Engine
  LANGUAGES CXX)
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
set(SDL_STATIC ON)
set(SDL_STATIC_PIC ON)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.4.0
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL3)
FetchContent_GetProperties(SDL3)
set(SDL3_INCLUDE_DIR "${SDL3_SOURCE_DIR}/include")

message(STATUS "Fetching SDL3_image...")
set(SDL3IMAGE_SAMPLES OFF)
set(SDL3IMAGE_TESTS OFF)
FetchContent_Declare(
  SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-3.4.0
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3_image)
set(SDL3IMAGE_INCLUDE_DIR "${sdl3_image_SOURCE_DIR}/include")

message(STATUS "Fetching SDL3_mixer...")
set(SDL3MIXER_SAMPLES OFF)
set(SDL3MIXER_TESTS OFF)
FetchContent_Declare(
  SDL3_mixer
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
  GIT_TAG prerelease-3.1.2
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(SDL3_mixer)
set(SDL3MIXER_INCLUDE_DIR "${sdl3_mixer_SOURCE_DIR}/include")

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
  GIT_TAG v3.1.1 # or the version you want
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)
FetchContent_GetProperties(box2d)
set_property(TARGET box2d PROPERTY COMPILE_WARNING_AS_ERROR OFF)  # Disable Errors on warning for MSVC compilation
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
SET(INPUT_SHADERS_DIR ${CMAKE_BINARY_DIR}/shaders)
SET(OUTPUT_SHADERS_DIR ${CMAKE_BINARY_DIR}/shaders/compiled)

## stb
FetchContent_Declare(
  stb
  GIT_REPOSITORY https://github.com/nothings/stb.git
  GIT_TAG master
  GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(stb)



set(BGFX_DIR "${bgfx_SOURCE_DIR}/bgfx")

FetchContent_Declare(json
  URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)
FetchContent_MakeAvailable(json)


## Shared library
## Edit the list of file here:
set(LIB_FILES
        Components/Rendering/Renderable.cpp
        Core/Engine.cpp
        Core/Entity.cpp
        Core/Input.cpp
        Core/Time.cpp
        Core/Scene.cpp
        Core/SceneSerializer.cpp
        Core/LayerManager.cpp
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

##### HE build definition #####

message(STATUS "${Yellow}Compiling engine with source files:\n${LIB_FILES}${ColourReset}")
add_library(hylozoa_engine STATIC
        ${LIB_FILES}
)

# Add include directories for the library
target_include_directories(hylozoa_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOX2D_INCLUDE_DIR}
    ${SDL3_INCLUDE_DIR}
    ${SDL3IMAGE_INCLUDE_DIR}
    ${SDL3MIXER_INCLUDE_DIR}
    ${glm_INCLUDE_DIR}
    ${EnTT_INCLUDE_DIR}
    ${stb_SOURCE_DIR}

    ${bgfx_SOURCE_DIR}/bgfx/include
    ${bgfx_SOURCE_DIR}/bx/include
    ${bgfx_SOURCE_DIR}/bimg/include
    
    ${OUTPUT_SHADERS_DIR}
)
target_link_libraries(hylozoa_engine PUBLIC
  SDL3::SDL3
  SDL3_image::SDL3_image
  glm::glm
  EnTT::EnTT
  box2d
  bgfx
  bx
  bimg
  nlohmann_json::nlohmann_json
)

##### Development executable #####

if (HE_ENGINE_BUILD_MAIN_EXECUTABLE)
  message(STATUS "${Yellow}Building main executable...${ColourReset}")
  add_executable(hylozoa_engine_main
      main.cpp
  )
  target_link_libraries(hylozoa_engine_main
      hylozoa_engine
  )
endif()
