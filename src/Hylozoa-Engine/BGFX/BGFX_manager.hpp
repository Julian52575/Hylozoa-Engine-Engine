#ifndef HYLOZOA_ENGINE_BGFX_MANAGER_HPP
#define HYLOZOA_ENGINE_BGFX_MANAGER_HPP

#include <bgfx/bgfx.h>
#include <SDL3/SDL.h>
#include <fstream>

#include "posColorVertex.hpp"
#include "Hylozoa-Engine/BGFX/BGFX_renderer.hpp"

namespace Hylozoa::BGFX {
    enum class RenderLayer{
        World = 0,
        Particles = 1,
        UI = 2,
        Debug = 3,
        Count
    };

    enum class ShapeType {
        Rectangle = 0,
        Circle = 1,
        Triangle = 2,
        Count
    };

    class bgfx_manager{
        public:
            bgfx_manager(){};

            bgfx::ProgramHandle getProgramHandle() const {
                return this->_program;
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

                PosColorVertex::init();
                bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA);
                bgfx::setViewClear(this->_currentViewId,BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,0x000000FF,1.0f,0);
                bgfx::setViewRect(this->_currentViewId, 0, 0, width, height);

                bgfx::ShaderHandle vsh = loadShader("shaders/vs_simple.bin");
                bgfx::ShaderHandle fsh = loadShader("shaders/fs_simple.bin");

                
                this->updateMatrix(width, height);
                this->_program = bgfx::createProgram(vsh, fsh, true);
                this->_initialized = true;
            };

            bool setLayer(RenderLayer layer){
                if (static_cast<uint16_t>(layer) >= static_cast<uint16_t>(RenderLayer::Count))
                    return false;
                this->_currentViewId = static_cast<uint16_t>(layer);
                return true;
            };

            void terminate(){
                if (!this->_initialized)
                    return;
                if (bgfx::isValid(this->_program)){
                    bgfx::destroy(this->_program);
                    this->_program = BGFX_INVALID_HANDLE;
                }
                bgfx::shutdown();
                this->_initialized = false;
            };

            ~bgfx_manager(){
                terminate();
            };

            bgfx::ShaderHandle loadShader(const char* filename) {
                std::ifstream file(filename, std::ios::binary | std::ios::ate);
                if (!file.is_open()) {
                    SDL_Log("Couldn't open shader file: %s", filename);
                    return BGFX_INVALID_HANDLE;
                }
                std::streamsize size = file.tellg();
                file.seekg(0, std::ios::beg);

                const bgfx::Memory* mem = bgfx::alloc(static_cast<uint32_t>(size) + 1);
                if (!mem) {
                    SDL_Log("Couldn't allocate memory for shader: %s", filename);
                    return BGFX_INVALID_HANDLE;
                }
                file.read(reinterpret_cast<char*>(mem->data), size);
                mem->data[mem->size - 1] = '\0'; // Null-terminate

                return bgfx::createShader(mem);
            }

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
                    this->_renderer.drawRectangle(this->_currentViewId,this->_program, args...);
                }
                else if constexpr (T == ShapeType::Circle){
                    this->_renderer.drawCircle(this->_currentViewId,this->_program, args...);
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
                bgfx::setViewRect(this->_currentViewId, this->_xpos, this->_ypos, this->_width, this->_height);
                bgfx::setViewTransform(this->_currentViewId, this->_view, this->_proj);
            };
        
        private:
            bgfx::ProgramHandle _program;
            bool _initialized{false};
            BGFX_renderer _renderer;
            uint16_t _currentViewId{0};
            float _view[16]{0};
            float _proj[16]{0};

            u_int16_t _width{0};
            u_int16_t _height{0};
            u_int16_t _xpos{0};
            u_int16_t _ypos{0};
    };
}

#endif // HYLOZOA_ENGINE_BGFX_MANAGER_HPP