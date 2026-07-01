#pragma once

#include <bgfx/bgfx.h>

#include <cstdint>

namespace Hylozoa::BGFX {

struct PosColorVertex {
    float x;
    float y;
    float z;
    std::uint32_t abgr;

    inline static bgfx::VertexLayout layout;

    static void init() {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
};

struct PosTexVertex {
    float x;
    float y;
    float z;
    float u;
    float v;
    std::uint32_t abgr;

    inline static bgfx::VertexLayout layout;

    static void init() {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }
};

struct PosTexCoord0Vertex {
    float x;
    float y;
    float z;
    float u;
    float v;

    inline static bgfx::VertexLayout layout;

    static void init() {
        layout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();
    }
};

} // namespace Hylozoa::BGFX
