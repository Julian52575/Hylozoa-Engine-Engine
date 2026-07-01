#pragma once

#include "Hylozoa-Engine/BGFX/layout.hpp"

#include "fs_simple.h"
#include "vs_simple.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <glm/glm.hpp>

#include <array>
#include <cstdint>

namespace Hylozoa::BGFX {

class PrimitiveRenderer {
  public:
    PrimitiveRenderer() {
        for (std::uint16_t i = 0; i < circleSegments; ++i) {
            float angle = static_cast<float>(i) * 2.0f * bx::kPi /
                          static_cast<float>(circleSegments);
            m_circleCache[i] = glm::vec2(bx::cos(angle), bx::sin(angle));
        }
    }

    ~PrimitiveRenderer() { terminate(); }

    void initialize() {
        PosColorVertex::init();
        PosTexVertex::init();

        m_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);

        bgfx::ShaderHandle fsh =
            bgfx::createShader(bgfx::makeRef(fs_simple_fragment,
                                             sizeof(fs_simple_fragment)));
        bgfx::ShaderHandle vsh =
            bgfx::createShader(bgfx::makeRef(vs_simple_vertex,
                                             sizeof(vs_simple_vertex)));
        m_program = bgfx::createProgram(vsh, fsh, true);

        std::uint32_t white = 0xFFFFFFFF;
        m_whiteTexture = bgfx::createTexture2D(
            1, 1, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE,
            bgfx::copy(&white, sizeof(white)));

        m_initialized = true;
    }

    void terminate() {
        if (!m_initialized)
            return;

        if (bgfx::isValid(m_texColor))
            bgfx::destroy(m_texColor);
        if (bgfx::isValid(m_whiteTexture))
            bgfx::destroy(m_whiteTexture);
        if (bgfx::isValid(m_program))
            bgfx::destroy(m_program);

        m_texColor = BGFX_INVALID_HANDLE;
        m_whiteTexture = BGFX_INVALID_HANDLE;
        m_program = BGFX_INVALID_HANDLE;
        m_initialized = false;
    }

    void drawRectangle(std::uint16_t viewId, glm::vec2 position,
                       glm::vec2 size, std::uint32_t color) {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;
        bgfx::allocTransientVertexBuffer(&tvb, 4, PosColorVertex::layout);
        bgfx::allocTransientIndexBuffer(&tib, 6);
        if (tvb.data == nullptr || tib.data == nullptr)
            return;

        auto *verts = reinterpret_cast<PosColorVertex *>(tvb.data);
        float left = position.x;
        float right = position.x + size.x;
        float top = position.y;
        float bottom = position.y + size.y;

        verts[0] = {left, top, 0.0f, color};
        verts[1] = {right, top, 0.0f, color};
        verts[2] = {left, bottom, 0.0f, color};
        verts[3] = {right, bottom, 0.0f, color};

        auto *indices = reinterpret_cast<std::uint16_t *>(tib.data);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 3;
        indices[5] = 2;

        submit(viewId, tvb, tib, m_whiteTexture,
               BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
    }

    void drawCircle(std::uint16_t viewId, glm::vec2 center, float radius,
                    std::uint32_t color) {
        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;
        bgfx::allocTransientVertexBuffer(&tvb, circleSegments + 1,
                                         PosColorVertex::layout);
        bgfx::allocTransientIndexBuffer(&tib, circleSegments * 3);
        if (tvb.data == nullptr || tib.data == nullptr)
            return;

        auto *verts = reinterpret_cast<PosColorVertex *>(tvb.data);
        auto *indices = reinterpret_cast<std::uint16_t *>(tib.data);

        verts[0] = {center.x, center.y, 0.0f, color};
        for (std::uint16_t i = 0; i < circleSegments; ++i) {
            float x = center.x + (m_circleCache[i].x * radius);
            float y = center.y + (m_circleCache[i].y * radius);
            verts[i + 1] = {x, y, 0.0f, color};

            std::uint16_t idx = i * 3;
            indices[idx] = 0;
            indices[idx + 1] = i + 1;
            indices[idx + 2] = (i == circleSegments - 1) ? 1 : i + 2;
        }

        submit(viewId, tvb, tib, m_whiteTexture,
               BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
    }

    void drawTexture(std::uint16_t viewId, glm::vec2 position, glm::vec2 size,
                     bgfx::TextureHandle texture,
                     std::uint32_t color = 0xFFFFFFFF) {
        if (!bgfx::isValid(texture))
            return;

        bgfx::TransientVertexBuffer tvb;
        bgfx::TransientIndexBuffer tib;
        bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexVertex::layout);
        bgfx::allocTransientIndexBuffer(&tib, 6);
        if (tvb.data == nullptr || tib.data == nullptr)
            return;

        auto *verts = reinterpret_cast<PosTexVertex *>(tvb.data);
        float left = position.x;
        float right = position.x + size.x;
        float top = position.y;
        float bottom = position.y + size.y;

        verts[0] = {left, top, 0.0f, 0.0f, 0.0f, color};
        verts[1] = {right, top, 0.0f, 1.0f, 0.0f, color};
        verts[2] = {left, bottom, 0.0f, 0.0f, 1.0f, color};
        verts[3] = {right, bottom, 0.0f, 1.0f, 1.0f, color};

        auto *indices = reinterpret_cast<std::uint16_t *>(tib.data);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 1;
        indices[4] = 3;
        indices[5] = 2;

        submit(viewId, tvb, tib, texture,
               BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A |
                   BGFX_STATE_BLEND_ALPHA | BGFX_STATE_MSAA);
    }

  private:
    static constexpr std::uint16_t circleSegments = 64;

    void submit(std::uint16_t viewId, const bgfx::TransientVertexBuffer &tvb,
                const bgfx::TransientIndexBuffer &tib,
                bgfx::TextureHandle texture, std::uint64_t state) {
        bgfx::setTexture(0, m_texColor, texture,
                         BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP);
        bgfx::setVertexBuffer(0, &tvb);
        bgfx::setIndexBuffer(&tib);

        float model[16];
        bx::mtxIdentity(model);
        bgfx::setTransform(model);
        bgfx::setState(state);
        bgfx::submit(viewId, m_program);
    }

    std::array<glm::vec2, circleSegments> m_circleCache{};
    bgfx::ProgramHandle m_program{BGFX_INVALID_HANDLE};
    bgfx::UniformHandle m_texColor{BGFX_INVALID_HANDLE};
    bgfx::TextureHandle m_whiteTexture{BGFX_INVALID_HANDLE};
    bool m_initialized{false};
};

} // namespace Hylozoa::BGFX
