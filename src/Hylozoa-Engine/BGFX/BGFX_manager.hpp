#pragma once

#include "Hylozoa-Engine/BGFX/renderer/LightRenderer.hpp"
#include "Hylozoa-Engine/BGFX/renderer/PrimitiveRenderer.hpp"

#include "composite_fs.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <cstdint>
#include <utility>

namespace Hylozoa::BGFX {

enum class RenderLayer : std::uint16_t {
    World = 0,
    Light = 1,
    Composite = 2,
    UI = 3,
    Debug = 4,
    Count
};

enum class ShapeType {
    Rectangle = 0,
    Circle = 1,
    Texture = 2,
};

class BGFXManager {
  public:
    BGFXManager() = default;
    ~BGFXManager() { terminate(); }

    bool isInitialized() const { return m_initialized; }

    void initialize(SDL_Window *window);
    void terminate();

    template <ShapeType T, typename... Args>
    void drawShape(Args &&...args) {
        if constexpr (T == ShapeType::Rectangle) {
            m_primitiveRenderer.drawRectangle(
                static_cast<std::uint16_t>(RenderLayer::World),
                std::forward<Args>(args)...);
        } else if constexpr (T == ShapeType::Circle) {
            m_primitiveRenderer.drawCircle(
                static_cast<std::uint16_t>(RenderLayer::World),
                std::forward<Args>(args)...);
        } else if constexpr (T == ShapeType::Texture) {
            m_primitiveRenderer.drawTexture(
                static_cast<std::uint16_t>(RenderLayer::World),
                std::forward<Args>(args)...);
        }
    }

    void updateView(glm::vec2 cameraPosition);
    void updateMatrix();
    void updateMatrix(int width, int height);
    void display(glm::vec4 ambientColor = glm::vec4(0.05f, 0.05f, 0.1f, 1.0f));

    bgfx::TextureHandle loadTexture(const char *filepath, float *width = nullptr,
                                    float *height = nullptr);

    void renderLight(glm::vec2 center, float intensity = 0.5f,
                     float radius = 200.0f,
                     glm::vec3 color = glm::vec3(1.0f));

  private:
    bgfx::PlatformData loadWindow(SDL_Window *window);
    void destroyFrameBuffers();
    void createFrameBuffers(int width, int height);

    bgfx::FrameBufferHandle m_worldFrameBuffer{BGFX_INVALID_HANDLE};
    bgfx::FrameBufferHandle m_lightFrameBuffer{BGFX_INVALID_HANDLE};
    bgfx::ProgramHandle m_compositeProgram{BGFX_INVALID_HANDLE};

    bgfx::UniformHandle m_texColor{BGFX_INVALID_HANDLE};
    bgfx::UniformHandle m_texLight{BGFX_INVALID_HANDLE};
    bgfx::UniformHandle m_ambientColor{BGFX_INVALID_HANDLE};

    PrimitiveRenderer m_primitiveRenderer;
    LightRenderer m_lightRenderer;

    bool m_initialized{false};
    float m_view[16]{0};
    float m_projection[16]{0};
    glm::vec2 m_cameraPosition{0.0f, 0.0f};
    glm::vec2 m_screenSize{0.0f, 0.0f};
};

} // namespace Hylozoa::BGFX
