#ifndef HYLOZOA_ENGINE_BGFX_MANAGER_HPP
#define HYLOZOA_ENGINE_BGFX_MANAGER_HPP

#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include <fstream>
#include <glm/vec3.hpp>

#include "layout.hpp"
#include "Hylozoa-Engine/BGFX/BGFX_renderer.hpp"
// #include "glsl/fs_simple.sc.bin.h"
// #include "glsl/vs_simple.sc.bin.h"


namespace Hylozoa::BGFX {
    enum class RenderLayer : uint16_t{
        World = 0,
        Light = 1,
        Particles = 2,
        UI = 3,
        Debug = 4,
        Count
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
                
                this->_renderer.initialize();

                bgfx::setViewClear((unsigned short)RenderLayer::World,BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,0x000000FF,1.0f,0);
                bgfx::setViewRect((unsigned short)RenderLayer::World, 0, 0, width, height);
                
                bgfx::setViewClear((unsigned short)RenderLayer::Light,BGFX_CLEAR_NONE,0,1.0f,0);
                bgfx::setViewRect((uint16_t)RenderLayer::Light,0, 0,width, height);
                
                this->updateMatrix(width, height);
                
                this->_initialized = true;


            };

            void terminate(){
                if (!this->_initialized)
                    return;
                this->_renderer.terminate();
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
                bgfx::setViewTransform((unsigned short)RenderLayer::World, _view, _proj);
                if constexpr (T == ShapeType::Rectangle){
                    this->_renderer.drawRectangle((unsigned short)RenderLayer::World, args...);
                }
                else if constexpr (T == ShapeType::Circle){
                    this->_renderer.drawCircle((unsigned short)RenderLayer::World, args...);
                }
                else if constexpr (T == ShapeType::Texture){
                    this->_renderer.drawTexture((unsigned short)RenderLayer::World, args...);
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
                bgfx::setViewRect((unsigned short)RenderLayer::World, this->_xpos, this->_ypos, this->_width, this->_height);
                bgfx::setViewTransform((unsigned short)RenderLayer::World, this->_view, this->_proj);

                bgfx::setViewRect((unsigned short)RenderLayer::Light, this->_xpos, this->_ypos, this->_width, this->_height);
                bgfx::setViewTransform((unsigned short)RenderLayer::Light, this->_view, this->_proj);
            };

            void updateMatrix(int width, int height){
                if (this->_width != width || this->_height != height){
                    this->_width = (u_int16_t)width;
                    this->_height = (u_int16_t)height;
                    bgfx::reset(width, height, BGFX_RESET_VSYNC);
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

            void display(bool submitted = true){
                // if (!submitted)
                //     bgfx::touch((unsigned short)RenderLayer::World);
                //si pas de render actif : bgfx::touch((unsigned short)RenderLayer::World);
                //si pas de lumière active : bgfx::touch((unsigned short)RenderLayer::Light);
                bgfx::touch((unsigned short)RenderLayer::World);
                // bgfx::touch((unsigned short)RenderLayer::Light);
                bgfx::frame();
            };

            bgfx::TextureHandle loadTexture(const char* filepath,float *width = nullptr,float *height = nullptr);
            
            void renderLight(int x, int y, int width=-1,int height= -1){
                bgfx::setViewTransform((unsigned short)RenderLayer::Light, nullptr, nullptr);
                bgfx::setViewRect((unsigned short)RenderLayer::Light, 0, 0, _width, _height);
                if (width < 0)
                    width = this->_width;
                if (height < 0)
                    height = this->_height;
                this->_renderer.renderLight(
                    (unsigned short)RenderLayer::Light,
                    glm::vec2(x,y),
                    glm::vec2(width,height),
                    glm::vec2(_width,_height),
                    glm::vec3(1.0f,0.0f,1.0f),
                    10
                );
            }
        
        private:
            // bgfx::ProgramHandle _m_textureProg;
            // bgfx::ProgramHandle _m_lightProg;

            // bgfx::UniformHandle u_lightPos;
            // bgfx::UniformHandle u_lightColor;
            // bgfx::UniformHandle u_screenSize;

            bool _initialized{false};
            BGFX_renderer _renderer;
            float _view[16]{0};
            float _proj[16]{0};

            u_int16_t _width{0};
            u_int16_t _height{0};
            u_int16_t _xpos{0};
            u_int16_t _ypos{0};
    };
}

#endif // HYLOZOA_ENGINE_BGFX_MANAGER_HPP