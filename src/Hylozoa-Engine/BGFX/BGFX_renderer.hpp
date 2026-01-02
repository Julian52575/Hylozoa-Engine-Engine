#ifndef HYLOZOA_ENGINE_BGFX_RENDERER_HPP
#define HYLOZOA_ENGINE_BGFX_RENDERER_HPP

#include <bgfx/bgfx.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <bx/math.h>
#include "Hylozoa-Engine/BGFX/layout.hpp"

#include "fs_simple.h"
#include "vs_simple.h"
#include "light_fs.h"
#include "light_vs.h"


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

            void initializeLayout(){
                PosColorVertex::init();
                PosTexVertex::init();
                PosTexCoord0Vertex::init();
            };

            void initializeUniform(){
                this->_s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
                this->_u_lightPos = bgfx::createUniform("u_lightPos", bgfx::UniformType::Vec4);
                this->_u_lightColor = bgfx::createUniform("u_lightColor", bgfx::UniformType::Vec4);
                this->_u_screenSize = bgfx::createUniform("u_screenSize", bgfx::UniformType::Vec4);
            }

            void loadShaders(){
                bgfx::ShaderHandle fsh = bgfx::createShader(bgfx::makeRef(fs_simple_fragment, sizeof(fs_simple_fragment)));
                bgfx::ShaderHandle vsh = bgfx::createShader(bgfx::makeRef(vs_simple_vertex, sizeof(vs_simple_vertex)));
                bgfx::ShaderHandle fsh_light = bgfx::createShader(bgfx::makeRef(light_fs_fragment, sizeof(light_fs_fragment)));
                bgfx::ShaderHandle vsh_light = bgfx::createShader(bgfx::makeRef(light_vs_vertex, sizeof(light_vs_vertex)));

                this->_m_textureProg = bgfx::createProgram(vsh, fsh, true);
                this->_m_lightProg = bgfx::createProgram(vsh_light, fsh_light, true);
            }

            void initialize(){
                initializeLayout();
                initializeUniform();
                loadShaders();
                _initialized = true;
            };

            void terminate(){
                if (!this->_initialized)
                    return;
                if (bgfx::isValid(_s_texColor)){
                    bgfx::destroy(_s_texColor);
                    _s_texColor = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_u_lightPos)){
                    bgfx::destroy(_u_lightPos);
                    _u_lightPos = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_u_lightColor)){
                    bgfx::destroy(_u_lightColor);
                    _u_lightColor = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_u_screenSize)){
                    bgfx::destroy(_u_screenSize);
                    _u_screenSize = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_m_textureProg)){
                    bgfx::destroy(_m_textureProg);
                    _m_textureProg = BGFX_INVALID_HANDLE;
                }
                if (bgfx::isValid(_m_lightProg)){
                    bgfx::destroy(_m_lightProg);
                    _m_lightProg = BGFX_INVALID_HANDLE;
                }
                _initialized = false;
            };

            ~BGFX_renderer(){
                terminate();
            };

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
                bgfx::submit(viewId, _m_textureProg);
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

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
                bgfx::submit(viewId, _m_textureProg);
            }

            void drawTexture(uint16_t viewId, glm::vec2 position, glm::vec2 size, bgfx::TextureHandle texture){
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

                uint32_t white = 0xFFFFFFFF; // Couleur blanche en format ABGR

                verts[0] = { left, top, 0.0f, 0.0f, 0.0f,white }; // Haut-Gauche
                verts[1] = { right, top, 0.0f, 1.0f, 0.0f, white }; // Haut-Droite
                verts[2] = { left, bottom, 0.0f, 0.0f, 1.0f, white }; // Bas-Gauche
                verts[3] = { right, bottom, 0.0f, 1.0f, 1.0f, white }; // Bas-Droite

                uint16_t* indices = (uint16_t*)tib.data;
                indices[0] = 0; indices[1] = 1; indices[2] = 2;
                indices[3] = 1; indices[4] = 3; indices[5] = 2;

                if (!bgfx::isValid(texture)) {
                    SDL_Log("ERREUR : La texture n'est pas valide !");
                    return;
                }
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
                bgfx::submit(viewId, _m_textureProg);
            }

            

            void renderLight(bgfx::ViewId viewId,glm::vec2 position,glm::vec2 size,glm::vec2 screenSize,glm::vec3 color= glm::vec3(1.0f,1.0f,1.0f), float intensity=10.0f){
                float screenSizeArr[4] = { screenSize.x, screenSize.y, 0.0f, 0.0f };
                bgfx::setUniform(this->_u_screenSize, screenSizeArr);

                float lightPos[4] = { position.x , position.y + screenSize.y / 2, 0.0f, 0.0f }; //centre de la lumière
                float lightCol[4] = { color.r, color.g, color.b, intensity };

                bgfx::setUniform(this->_u_lightPos, lightPos);
                bgfx::setUniform(this->_u_lightColor, lightCol);

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_BLEND_ADD);

                setupLightQuad(position,size, screenSize);

                bgfx::submit(viewId, this->_m_lightProg);
            }

        private:
            static constexpr uint16_t circleSegments = 64;
            glm::vec2 _circleCache[circleSegments];
            bool _initialized{false};

            bgfx::UniformHandle _s_texColor{BGFX_INVALID_HANDLE};
            bgfx::UniformHandle _u_lightPos{BGFX_INVALID_HANDLE};
            bgfx::UniformHandle _u_lightColor{BGFX_INVALID_HANDLE};
            bgfx::UniformHandle _u_screenSize{BGFX_INVALID_HANDLE};

            bgfx::ProgramHandle _m_textureProg;
            bgfx::ProgramHandle _m_lightProg;
            
            void setupLightQuad(glm::vec2 position,glm::vec2 size, glm::vec2 screenSize){
                // 2. Allouer 4 sommets pour un rectangle
                bgfx::TransientVertexBuffer tvb;
                bgfx::allocTransientVertexBuffer(&tvb, 4, PosTexCoord0Vertex::layout);
                if (tvb.data != nullptr) {
                    struct Vertex { float x, y, z, u, v; };
                    Vertex* v = (Vertex*)tvb.data;

                    float x1 = (position.x / (float)screenSize.x) * 2.0f - 1.0f;
                    float y1 = ((position.y / (float)screenSize.y) * 2.0f - 1.0f) * -1.0f; // Inversion Y pour BGFX
                    float x2 = ((position.x + size.x) / (float)screenSize.x) * 2.0f - 1.0f;
                    float y2 = (((position.y + size.y) / (float)screenSize.y) * 2.0f - 1.0f) * -1.0f; // Inversion Y pour BGFX
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
    };
}

#endif // HYLOZOA_ENGINE_BGFX_RENDERER_HPP