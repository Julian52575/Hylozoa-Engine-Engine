#ifndef HYLOZOA_ENGINE_BGFX_MANAGER_HPP
#define HYLOZOA_ENGINE_BGFX_MANAGER_HPP

#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include <fstream>
#include <glm/vec3.hpp>

#include "layout.hpp"
#include "Hylozoa-Engine/BGFX/renderer/LightRenderer.hpp"
#include "Hylozoa-Engine/BGFX/renderer/PrimitiveRenderer.hpp"

#include "composite_fs.h"

namespace Hylozoa::BGFX {
    enum class RenderLayer : uint16_t{
        World = 0,      // Le jeu
        Light = 1,      // lumières
        Particles = 2,  // particules
        UI = 3,         // interface utilisateur
        Debug = 4,      // débogage
        Count           // len
    };

    enum class ShapeType {
        Rectangle = 0,
        Circle = 1,
        Triangle = 2,
        Texture = 3,
        Count
    };

    class bgfx_manager{
        public:
            bgfx_manager(){};

            bool isInitialized() const {
                return this->_initialized;
            };

            void initialize(SDL_Window* window){
                int width, height;
                SDL_GetWindowSize(window, &width, &height);
                bgfx::PlatformData pd = loadWindow(window);
                bgfx::Init init;
                init.platformData = pd;
                init.type = bgfx::RendererType::Count;
                init.resolution.width = width;
                init.resolution.height = height;
                init.resolution.reset = BGFX_RESET_VSYNC;

                if (!bgfx::init(init)) {
                    SDL_Log("Couldn't initialize bgfx");
                    return;
                }
                
                this->_Primrenderer.initialize();
                this->_Lightrenderer.initialize();

                
                this->updateMatrix(width, height);

                
                //partie final render
                uint64_t flags = BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;
                this->_worldFB = bgfx::createFrameBuffer(uint16_t(width), uint16_t(height), bgfx::TextureFormat::RGBA8, flags);

                // // Framebuffer pour les Lumières (on peut utiliser RGBA8 aussi)
                this->_lightFB = bgfx::createFrameBuffer(uint16_t(width), uint16_t(height), bgfx::TextureFormat::RGBA8, flags);

                this->_s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
                this->_s_texLight = bgfx::createUniform("s_texLight", bgfx::UniformType::Sampler);

                bgfx::ShaderHandle vsh = bgfx::createShader(bgfx::makeRef(light_vs_vertex, sizeof(light_vs_vertex)));
                bgfx::ShaderHandle fsh = bgfx::createShader(bgfx::makeRef(composite_fs_fragment, sizeof(composite_fs_fragment)));
                this->_compositeProgram = bgfx::createProgram(vsh, fsh, true);

                
                this->_initialized = true;

            };

            void terminate(){
                if (!this->_initialized)
                    return;
                this->_Primrenderer.terminate();
                this->_Lightrenderer.terminate();
                bgfx::shutdown();
                this->_initialized = false;
            };

            ~bgfx_manager(){
                terminate();
            };

            bgfx::PlatformData loadWindow(SDL_Window* window){
                SDL_PropertiesID props = SDL_GetWindowProperties(window);
                bgfx::PlatformData pd{};
                #if defined(_WIN32)
                    pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
                #elif defined(__linux__)
                    uint64_t x11Window = SDL_GetNumberProperty(props,SDL_PROP_WINDOW_X11_WINDOW_NUMBER,0);
                    void* x11Display = SDL_GetPointerProperty(props,SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
                    if (!x11Window || !x11Display) {
                        SDL_Log("Couldn't get X11 window/display pointers");
                        return pd;
                    }
                    pd.nwh = (void*)(uintptr_t)x11Window;
                    pd.ndt = x11Display;
                #elif defined(__APPLE__)
                    pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
                #endif

                return pd;
            };

            template<ShapeType T,typename... Args>
            void drawShape(Args... args){
                if constexpr (T == ShapeType::Rectangle){
                    this->_Primrenderer.drawRectangle((unsigned short)RenderLayer::World, args...);
                }
                else if constexpr (T == ShapeType::Circle){
                    this->_Primrenderer.drawCircle((unsigned short)RenderLayer::World, args...);
                }
                else if constexpr (T == ShapeType::Texture){
                    this->_Primrenderer.drawTexture((unsigned short)RenderLayer::World, args...);
                }
            };

            void renderCurrentScene(glm::vec2 position){
                bx::mtxLookAt(
                    this->_view,
                    bx::Vec3{position.x, position.y,-1.0f},
                    bx::Vec3{position.x, position.y, 0.0f},
                    bx::Vec3{0.0f, 1.0f, 0.0f}
                );
            };

            void updateMatrix(){
                bgfx::setViewFrameBuffer((unsigned short)RenderLayer::World, this->_worldFB);
                bgfx::setViewFrameBuffer((unsigned short)RenderLayer::Light, this->_lightFB);

                bgfx::setViewFrameBuffer((unsigned short)RenderLayer::Debug, BGFX_INVALID_HANDLE); //rempalcer par le backbuffer

                const RenderLayer layers[] = { RenderLayer::World, RenderLayer::Light };
                for (auto layer : layers) {
                    bgfx::setViewTransform((unsigned short)layer, this->_view, this->_proj);
                    bgfx::setViewRect((unsigned short)layer, 0, 0, this->_width, this->_height);
                }

                float identity[16];
                bx::mtxIdentity(identity);
                bgfx::setViewTransform((unsigned short)RenderLayer::Debug, identity, this->_proj);
                bgfx::setViewRect((unsigned short)RenderLayer::Debug, 0, 0, this->_width, this->_height);

                bgfx::setViewClear((unsigned short)RenderLayer::World, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
                bgfx::setViewClear((unsigned short)RenderLayer::Light, BGFX_CLEAR_COLOR, 0x000000FF, 1.0f, 0);
                bgfx::setViewClear((unsigned short)RenderLayer::Debug, BGFX_CLEAR_COLOR, 0x000000FF, 1.0f, 0);
            };

            void updateMatrix(int width, int height){
                if (this->_width != width || this->_height != height){
                    this->_width = (u_int16_t)width;
                    this->_height = (u_int16_t)height;
                    bgfx::reset(width, height, BGFX_RESET_NONE);
                    bx::mtxOrtho(
                        this->_proj,
                        0.0f,              // Gauche  
                        float(width),          // Droite 
                        float(height),           // Bas
                        0.0f,              // Haut
                        0.0f,                // Near
                        100.0f,              // Far
                        0.0f,                // Offset
                        bgfx::getCaps()->homogeneousDepth
                    );
                }
                updateMatrix();
            };

            void display(){
                bgfx::touch((unsigned short)RenderLayer::World);
                bgfx::touch((unsigned short)RenderLayer::Light);

                bgfx::setTexture(0, _s_texColor, bgfx::getTexture(_worldFB));
                bgfx::setTexture(1, _s_texLight, bgfx::getTexture(_lightFB));

                this->_Lightrenderer.setupLightQuad(glm::vec2(0,0), glm::vec2(_width, _height));

                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
                bgfx::submit((unsigned short)RenderLayer::Debug, _compositeProgram);

                bgfx::frame();
            };

            bgfx::TextureHandle loadTexture(const char* filepath,float *width = nullptr,float *height = nullptr);
            
            void renderLight(int x, int y, float intensity = 1.0f){
                float lightRadius = 200.0f;
                glm::vec2 pos( (float)x - lightRadius, (float)y - lightRadius );
                glm::vec2 size( lightRadius * 2.0f, lightRadius * 2.0f );
                
                this->_Lightrenderer.renderLight(
                    (unsigned short)RenderLayer::Light,
                    pos,
                    size,
                    glm::vec3(1.0f,0.0f,0.0f),
                    intensity
                );
            }
        
        private:
            bgfx::FrameBufferHandle _worldFB{BGFX_INVALID_HANDLE};
            bgfx::FrameBufferHandle _lightFB{BGFX_INVALID_HANDLE};
            bgfx::ProgramHandle _compositeProgram{BGFX_INVALID_HANDLE};

            bgfx::UniformHandle _s_texColor{BGFX_INVALID_HANDLE};
            bgfx::UniformHandle _s_texLight{BGFX_INVALID_HANDLE};

            bool _initialized{false};
            PrimitiveRenderer _Primrenderer;
            LightRenderer _Lightrenderer;
            float _view[16]{0};
            float _proj[16]{0};

            u_int16_t _width{0};
            u_int16_t _height{0};
            u_int16_t _xpos{0};
            u_int16_t _ypos{0};
    };
}

#endif // HYLOZOA_ENGINE_BGFX_MANAGER_HPP