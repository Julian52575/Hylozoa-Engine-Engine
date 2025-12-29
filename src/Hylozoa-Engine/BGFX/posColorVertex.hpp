#ifndef POS_COLOR_VERTEX_HPP
#define POS_COLOR_VERTEX_HPP
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

#endif // POS_COLOR_VERTEX_HPP