#pragma once

#include "Hylozoa-Engine/BGFX/layout.hpp"

#include "light_fs.h"
#include "light_vs.h"

#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cstdint>

namespace Hylozoa::BGFX {

class LightRenderer {
  public:
    LightRenderer() = default;
    ~LightRenderer() { terminate(); }

    void initialize() {
        PosTexCoord0Vertex::init();

        m_lightColor =
            bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4);

        bgfx::ShaderHandle fsh =
            bgfx::createShader(bgfx::makeRef(light_fs_fragment,
                                             sizeof(light_fs_fragment)));
        bgfx::ShaderHandle vsh =
            bgfx::createShader(bgfx::makeRef(light_vs_vertex,
                                             sizeof(light_vs_vertex)));

        m_program = bgfx::createProgram(vsh, fsh, true);
        m_initialized = true;
    }

    void terminate() {
        if (!m_initialized)
            return;

        if (bgfx::isValid(m_lightColor))
            bgfx::destroy(m_lightColor);
        if (bgfx::isValid(m_program))
            bgfx::destroy(m_program);

        m_lightColor = BGFX_INVALID_HANDLE;
        m_program = BGFX_INVALID_HANDLE;
        m_initialized = false;
    }

    void renderLight(bgfx::ViewId viewId, glm::vec2 position, glm::vec2 size,
                     glm::vec3 color = glm::vec3(1.0f), float intensity = 1.0f) {
        float lightColor[4] = {color.r, color.g, color.b, intensity};
        bgfx::setUniform(m_lightColor, lightColor);
        bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                       BGFX_STATE_BLEND_ADD);
        setupLightQuad(position, size);
        bgfx::submit(viewId, m_program);
    }

    void setupLightQuad(glm::vec2 position, glm::vec2 size) {
        bgfx::TransientVertexBuffer tvb;
        bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexCoord0Vertex::layout);
        if (tvb.data == nullptr)
            return;

        auto *verts = reinterpret_cast<PosTexCoord0Vertex *>(tvb.data);
        float x1 = position.x;
        float y1 = position.y;
        float x2 = position.x + size.x;
        float y2 = position.y + size.y;

        verts[0] = {x1, y1, 0.0f, 0.0f, 0.0f};
        verts[1] = {x2, y1, 0.0f, 1.0f, 0.0f};
        verts[2] = {x1, y2, 0.0f, 0.0f, 1.0f};
        verts[3] = {x2, y2, 0.0f, 1.0f, 1.0f};
        bgfx::setVertexBuffer(0, &tvb);

        bgfx::TransientIndexBuffer tib;
        bgfx::allocTransientIndexBuffer(&tib, 6);
        if (tib.data == nullptr)
            return;

        auto *indices = reinterpret_cast<std::uint16_t *>(tib.data);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 3;
        indices[5] = 2;
        bgfx::setIndexBuffer(&tib);
    }

  private:
    bgfx::ProgramHandle m_program{BGFX_INVALID_HANDLE};
    bgfx::UniformHandle m_lightColor{BGFX_INVALID_HANDLE};
    bool m_initialized{false};
};

} // namespace Hylozoa::BGFX
