#ifndef HYLOZOA_ENGINE_BGFX_RENDERER_HPP
#define HYLOZOA_ENGINE_BGFX_RENDERER_HPP

#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <bx/math.h>
#include "Hylozoa-Engine/BGFX/posColorVertex.hpp"

namespace Hylozoa::BGFX {
    class BGFX_renderer{
        public:
            BGFX_renderer(){
                //cache circle points
                for (uint16_t i = 0; i < circleSegments; ++i){
                    float angle = (float)i * 2.0f * bx::kPi / (float)circleSegments;
                    _circleCache[i] = glm::vec2(bx::cos(angle), bx::sin(angle));
                }
            };
            ~BGFX_renderer(){};

            void drawRectangle(uint16_t viewId,bgfx::ProgramHandle program, glm::vec2 position, glm::vec2 size, uint32_t color){
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
                bgfx::submit(viewId, program);
            }

            void drawCircle(u_int16_t viewId,bgfx::ProgramHandle program, glm::vec2 center, float radius, uint32_t color){
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

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
                bgfx::submit(viewId, program);
            }
        private:
            static constexpr uint16_t circleSegments = 64;
            glm::vec2 _circleCache[circleSegments];
    };
}

#endif // HYLOZOA_ENGINE_BGFX_RENDERER_HPP