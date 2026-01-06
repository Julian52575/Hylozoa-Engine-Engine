#ifndef HYLOZOA_ENGINE_BGFX_RENDERER_LIGHTRENDERER_HPP
#define HYLOZOA_ENGINE_BGFX_RENDERER_LIGHTRENDERER_HPP

#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Hylozoa-Engine/BGFX/layout.hpp"
#include "light_fs.h"
#include "light_vs.h"

namespace Hylozoa::BGFX {
    class LightRenderer {
        public:
            LightRenderer() = default;
            ~LightRenderer() = default;

            void initialize() {
                PosTexCoord0Vertex::init();

                this->_u_lightColor = bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4);
                this->_s_occMap = bgfx::createUniform("s_occMap", bgfx::UniformType::Sampler);

                bgfx::ShaderHandle fsh = bgfx::createShader(bgfx::makeRef(light_fs_fragment, sizeof(light_fs_fragment)));
                bgfx::ShaderHandle vsh = bgfx::createShader(bgfx::makeRef(light_vs_vertex, sizeof(light_vs_vertex)));

                this->_lightProgram = bgfx::createProgram(vsh, fsh, true);

                this->_initialized = true;
            }

            void terminate() {
                if (!this->_initialized)
                    return;
                if (bgfx::isValid(_u_lightColor)){
                    bgfx::destroy(_u_lightColor);
                    _u_lightColor = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_lightProgram)){
                    bgfx::destroy(_lightProgram);
                    _lightProgram = BGFX_INVALID_HANDLE;
                }
                this->_initialized = false;
            }

            void renderLight(bgfx::ViewId viewId,glm::vec2 position,glm::vec2 size,glm::vec3 color= glm::vec3(1.0f,1.0f,1.0f), float intensity=10.0f, bgfx::TextureHandle occlusionMap=BGFX_INVALID_HANDLE) {

                float lightCol[4] = { color.r, color.g, color.b, intensity };
                bgfx::setUniform(this->_u_lightColor, lightCol);


                if (bgfx::isValid(occlusionMap)) {
                    bgfx::setTexture(0, this->_s_occMap, occlusionMap); //rempalcer 0 par occlusion map sampler
                }

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_BLEND_ADD);

                this->setupLightQuad(position,size);

                bgfx::submit(viewId, this->_lightProgram);
            }

            //mettre en privé
            void setupLightQuad(glm::vec2 position,glm::vec2 size){
                // 2. Allouer 4 sommets pour un rectangle
                bgfx::TransientVertexBuffer tvb;
                bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexCoord0Vertex::layout);
                if (tvb.data != nullptr) {
                    struct Vertex { float x, y, z, u, v; };
                    Vertex* v = (Vertex*)tvb.data;

                    float x1 = position.x;
                    float y1 = position.y;
                    float x2 = position.x + size.x;
                    float y2 = position.y + size.y;

                    v[0] = { x1,  y1, 0.0f, 0.0f, 0.0f };
                    v[1] = { x2,  y1, 0.0f, 1.0f, 0.0f };
                    v[2] = { x1,  y2, 0.0f, 0.0f, 1.0f };
                    v[3] = { x2,  y2, 0.0f, 1.0f, 1.0f };
                    bgfx::setVertexBuffer(0, &tvb);
                }

                bgfx::TransientIndexBuffer tib;
                bgfx::allocTransientIndexBuffer(&tib, 6);
                if (tib.data != nullptr) {
                    uint16_t* indices = (uint16_t*)tib.data;
                    indices[0] = 0; indices[1] = 1; indices[2] = 2;
                    indices[3] = 1; indices[4] = 3; indices[5] = 2;
                    bgfx::setIndexBuffer(&tib);
                }
            };

        private:
            bgfx::ProgramHandle _lightProgram;
            bool _initialized{false};

            bgfx::UniformHandle _u_lightColor{BGFX_INVALID_HANDLE};
            bgfx::UniformHandle _s_occMap{BGFX_INVALID_HANDLE};
    };

} // namespace Hylozoa::BGFX

#endif // HYLOZOA_ENGINE_BGFX_RENDERER_LIGHTRENDERER_HPP