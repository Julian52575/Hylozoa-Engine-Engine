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

## Add shaders below
set(ALL_GENERATED_SHADER_HEADERS "")
if (WIN32)
  set (BGFX_SHADERC_PLATFORM "windows")
  set(VS_PROFILE "vs_5_0")
  set(FS_PROFILE "ps_5_0")
elseif(APPLE)
  set (BGFX_SHADERC_PLATFORM "osx")
  set(VS_PROFILE "metal")
  set(FS_PROFILE "metal")
else() # Linux / Android (OpenGL / Vulkan)
  set (BGFX_SHADERC_PLATFORM "linux")
  set(VS_PROFILE "440") # Ou "330" / "spirv" selon tes besoins
  set(FS_PROFILE "440") # Ou "330" / "spirv" selon tes besoins
endif()


set(BGFX_DIR "${bgfx_SOURCE_DIR}/bgfx")
function(compile_shader SHADER_NAME TYPE)
  set(INPUT_FILE "${INPUT_SHADERS_DIR}/${SHADER_NAME}.sc")
  set(OUTPUT_FILE "${OUTPUT_SHADERS_DIR}/${SHADER_NAME}.h")

  if(TYPE STREQUAL "vertex")
    set(PROFILE ${VS_PROFILE})
  else()
    set(PROFILE ${FS_PROFILE})
  endif()

  add_custom_command(
    OUTPUT ${OUTPUT_FILE}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_SHADERS_DIR}
    COMMAND shaderc -f ${INPUT_FILE}
            -o ${OUTPUT_FILE}
            --type ${TYPE}
            --platform ${BGFX_SHADERC_PLATFORM}
            --profile ${PROFILE}
            --varyingdef ${INPUT_SHADERS_DIR}/varying.def.sc
            -i ${BGFX_DIR}/src
            --bin2c ${SHADER_NAME}_${TYPE}
            
    DEPENDS "${INPUT_FILE}" "${INPUT_SHADERS_DIR}/varying.def.sc"
    COMMENT "Compiling ${TYPE} shader ${SHADER_NAME}.sc"
    VERBATIM
  )

  set(GENERATED_BINARIES ${GENERATED_BINARIES} "${OUTPUT_FILE}" PARENT_SCOPE)
endfunction()

set(GENERATED_BINARIES "")
compile_shader(vs_simple vertex)
compile_shader(fs_simple fragment)
compile_shader(light_fs fragment)
compile_shader(light_vs vertex)
compile_shader(composite_fs fragment)
add_custom_target(shaders ALL DEPENDS ${GENERATED_BINARIES})


## Shared library 
## Edit the list of file here:
set(LIB_FILES
  Placeholder/Placeholder.cpp
  Components/Rendering/Renderable.cpp
  Core/Engine.cpp
  Core/Entity.cpp
  Core/Input.cpp
  Core/Time.cpp
  Systems/Transform/Transform.cpp
  Systems/Physics/Collision.cpp
  Components/Transform/Transform.cpp
  SDL/SDL_Manager.cpp
  BGFX/BGFX_manager.cpp
  Systems/Renderer/Renderer.cpp
  Systems/Movement/Movement.cpp
)
# Add Hylozoa-Engine/ to file path
list(TRANSFORM LIB_FILES PREPEND "Hylozoa-Engine/")
# Add shaders to library files
list(APPEND LIB_FILES ${ALL_GENERATED_SHADER_HEADERS})

##### HE build definition #####

add_library(hylozoa_engine STATIC
  ${LIB_FILES}
)

add_dependencies(hylozoa_engine shaders)
# Add include directories for the library
target_include_directories(hylozoa_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${BOX2D_INCLUDE_DIR}
    ${SDL3_INCLUDE_DIR}
    ${glm_INCLUDE_DIR}
    ${EnTT_INCLUDE_DIR}
    ${stb_SOURCE_DIR}

    ${bgfx_SOURCE_DIR}/bgfx/include
    ${bgfx_SOURCE_DIR}/bx/include
    ${bgfx_SOURCE_DIR}/bimg/include
    
    ${OUTPUT_SHADERS_DIR}
)
target_link_libraries(hylozoa_engine PRIVATE
  SDL3::SDL3
  glm::glm
  EnTT::EnTT
  box2d
  bgfx
  bx
  bimg
)

##### Development executable #####

add_executable(hylozoa.exe
    main.cpp
)
target_link_libraries(hylozoa.exe
    hylozoa_engine
)

message(STATUS "${Yellow}Compiling files:\n${LIB_FILES}${ColourReset}")