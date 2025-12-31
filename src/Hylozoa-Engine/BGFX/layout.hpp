#ifndef LAYOUT_HPP
#define LAYOUT_HPP
#include <bgfx/bgfx.h>

struct PosColorVertex {
  float x;
  float y;
  float z;
  uint32_t abgr;
  inline static bgfx::VertexLayout layout;
  static void init() {
        layout.begin()
              .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
              .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
              .end();
    }
};

struct PosTexVertex {
  float x,y,z;
  float u,v;
  uint32_t abgr;
  static void init() {
        layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0,   2, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
        .end();
      }
  inline static bgfx::VertexLayout layout;
};

#endif // LAYOUT_HPP