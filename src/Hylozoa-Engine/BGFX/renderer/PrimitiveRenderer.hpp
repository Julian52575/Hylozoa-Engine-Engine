#ifndef HYLOZOA_ENGINE_BGFX_PRIMITIVERENDERER_HPP
#define HYLOZOA_ENGINE_BGFX_PRIMITIVERENDERER_HPP

#include <bgfx/bgfx.h>
#include "Hylozoa-Engine/BGFX/layout.hpp"
#include <glm/glm.hpp>
#include <bx/math.h>

#include "fs_simple.h"
#include "vs_simple.h"

#include <iostream>
namespace Hylozoa::BGFX {

    class PrimitiveRenderer {
        public:
            PrimitiveRenderer() {
                for (uint16_t i = 0; i < circleSegments; ++i){
                    float angle = (float)i * 2.0f * bx::kPi / (float)circleSegments;
                    _circleCache[i] = glm::vec2(bx::cos(angle), bx::sin(angle));
                }
            };
            ~PrimitiveRenderer() {
                terminate();
            };
            void initialize() {
                PosColorVertex::init();
                PosTexVertex::init();
                this->_s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
                bgfx::ShaderHandle fsh = bgfx::createShader(bgfx::makeRef(fs_simple_fragment, sizeof(fs_simple_fragment)));
                bgfx::ShaderHandle vsh = bgfx::createShader(bgfx::makeRef(vs_simple_vertex, sizeof(vs_simple_vertex)));
                this->_texture = bgfx::createProgram(vsh, fsh, true);

                uint32_t white = 0xFFFFFFFF;
                this->_whiteTex = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_NONE, bgfx::makeRef(&white, 4));

                this->_initialized = true;
            }
            void terminate() {
                if (!this->_initialized)
                    return;
                if (bgfx::isValid(_s_texColor)){
                    bgfx::destroy(_s_texColor);
                    this->_s_texColor = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_texture)){
                    bgfx::destroy(_texture);
                    this->_texture = BGFX_INVALID_HANDLE;
                }
                this->_initialized = false;
            }

            void drawRectangle(uint16_t viewId, glm::vec2 position, glm::vec2 size, uint32_t color){
                bgfx::TransientVertexBuffer tvb;
                bgfx::TransientIndexBuffer tib;

                // 1. Allouer l'espace pour 4 sommets et 6 indices
                bgfx::allocTransientVertexBuffer(&tvb, 4, PosColorVertex::layout);
                bgfx::allocTransientIndexBuffer(&tib, 6);
                if (tvb.data == nullptr || tib.data == nullptr) {
                    return; // allocation failed
                }

                // 2. Remplir les sommets (PosColorVertex attend x, y, z, abgr)
                PosColorVertex* verts = (PosColorVertex*)tvb.data;
                float left   = position.x;
                float right  = position.x + size.x;
                float top    = position.y;
                float bottom = position.y + size.y;

                verts[0] = { left, top, 0.0f, color }; // Haut-Gauche
                verts[1] = { right, top, 0.0f, color }; // Haut-Droite
                verts[2] = { left, bottom, 0.0f, color }; // Bas-Gauche
                verts[3] = { right, bottom, 0.0f, color }; // Bas-Droite

                //3. Remplir les indices pour deux triangles formant le rectangle
                uint16_t* indices = (uint16_t*)tib.data;
                indices[0] = 0; indices[1] = 1; indices[2] = 2;
                indices[3] = 1; indices[4] = 3; indices[5] = 2;

                // 4. Configurer l'état du CPU
                bgfx::setVertexBuffer(0, &tvb);
                bgfx::setIndexBuffer(&tib);

                float model[16];
                bx::mtxIdentity(model);
                bgfx::setTransform(model);

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
                bgfx::setTexture(0, _s_texColor, _whiteTex);
                bgfx::submit(viewId, _texture);
            }

            void drawCircle(u_int16_t viewId, glm::vec2 center, float radius, uint32_t color){
                bgfx::TransientVertexBuffer tvb;
                bgfx::TransientIndexBuffer tib;

                bgfx::allocTransientVertexBuffer(&tvb, circleSegments + 1, PosColorVertex::layout);
                bgfx::allocTransientIndexBuffer(&tib, circleSegments * 3);
                if (tvb.data == nullptr || tib.data == nullptr) {
                    return; // allocation failed
                }

                PosColorVertex* verts = (PosColorVertex*)tvb.data;
                uint16_t* indices = (uint16_t*)tib.data;

                verts[0] = { center.x, center.y, 0.0f, color }; // Centre du cercle
                for (uint32_t i = 0; i < circleSegments; ++i) {
                    float x = center.x + (_circleCache[i].x * radius);
                    float y = center.y + (_circleCache[i].y * radius);
                    
                    verts[i + 1] = { x, y, 0.0f, color };

                    // Indices for the triangles
                    uint16_t idx = i * 3;
                    indices[idx] = 0; // Center
                    indices[idx + 1] = i + 1;
                    indices[idx + 2] = (i == circleSegments - 1) ? 1 : i + 2;
                }

                bgfx::setVertexBuffer(0, &tvb);
                bgfx::setIndexBuffer(&tib);

                float model[16];
                bx::mtxIdentity(model);
                bgfx::setTransform(model);

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A  | BGFX_STATE_MSAA);
                bgfx::setTexture(0, _s_texColor, _whiteTex);
                bgfx::submit(viewId, _texture);
            }

            void drawTexture(uint16_t viewId, glm::vec2 position, glm::vec2 size, bgfx::TextureHandle texture, uint32_t color=0xFFFFFFFF){
                if (!bgfx::isValid(texture)) {
                    std::cout << "Invalid texture handle in drawTexture\n";
                    return;
                }

                bgfx::TransientVertexBuffer tvb;
                bgfx::TransientIndexBuffer tib;

                bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexVertex::layout);
                bgfx::allocTransientIndexBuffer(&tib, 6);
                if (tvb.data == nullptr || tib.data == nullptr) {
                    return; // allocation failed
                }

                PosTexVertex* verts = (PosTexVertex*)tvb.data;
                float left   = position.x;
                float right  = position.x + size.x;
                float top    = position.y;
                float bottom = position.y + size.y;

                verts[0] = { left, top, 0.0f, 0.0f, 0.0f, color }; // Haut-Gauche
                verts[1] = { right, top, 0.0f, 1.0f, 0.0f, color }; // Haut-Droite
                verts[2] = { left, bottom, 0.0f, 0.0f, 1.0f, color }; // Bas-Gauche
                verts[3] = { right, bottom, 0.0f, 1.0f, 1.0f, color }; // Bas-Droite

                uint16_t* indices = (uint16_t*)tib.data;
                indices[0] = 0; indices[1] = 1; indices[2] = 2;
                indices[3] = 1; indices[4] = 3; indices[5] = 2;

                bgfx::setTexture(
                    0,
                    _s_texColor, 
                    texture,
                    BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP
                );

                bgfx::setVertexBuffer(0, &tvb);
                bgfx::setIndexBuffer(&tib);

                float model[16];
                bx::mtxIdentity(model);
                bgfx::setTransform(model);

                bgfx::setState(
                    BGFX_STATE_WRITE_RGB | 
                    BGFX_STATE_WRITE_A | 
                    BGFX_STATE_BLEND_ALPHA | 
                    BGFX_STATE_MSAA
                );
                bgfx::submit(viewId, _texture);
            }
        
            private:
                static constexpr uint16_t circleSegments = 64;
                glm::vec2 _circleCache[circleSegments];
                bgfx::ProgramHandle _texture;
                bgfx::UniformHandle _s_texColor{BGFX_INVALID_HANDLE};
                bgfx::TextureHandle _whiteTex{BGFX_INVALID_HANDLE};
                bool _initialized{false};

    };

} // namespace Hylozoa::BGFX

#endif // HYLOZOA_ENGINE_BGFX_PRIMITIVERENDERER_HPP