#include "BGFX_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <bgfx/platform.h>
#include <bx/math.h>

#include <cstdint>

namespace Hylozoa::BGFX {

void BGFXManager::initialize(SDL_Window *window) {
    if (m_initialized || window == nullptr)
        return;

    int width = 0;
    int height = 0;
    SDL_GetWindowSize(window, &width, &height);

    bgfx::Init init;
    init.platformData = loadWindow(window);
    init.type = bgfx::RendererType::Count;
    init.resolution.width = static_cast<std::uint32_t>(width);
    init.resolution.height = static_cast<std::uint32_t>(height);
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init)) {
        SDL_Log("Couldn't initialize bgfx");
        return;
    }

    m_primitiveRenderer.initialize();
    m_lightRenderer.initialize();

    createFrameBuffers(width, height);

    m_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
    m_texLight = bgfx::createUniform("s_texLight", bgfx::UniformType::Sampler);
    m_ambientColor =
        bgfx::createUniform("u_ambientColor", bgfx::UniformType::Vec4);

    bgfx::ShaderHandle vsh =
        bgfx::createShader(bgfx::makeRef(light_vs_vertex,
                                         sizeof(light_vs_vertex)));
    bgfx::ShaderHandle fsh =
        bgfx::createShader(bgfx::makeRef(composite_fs_fragment,
                                         sizeof(composite_fs_fragment)));
    m_compositeProgram = bgfx::createProgram(vsh, fsh, true);

    m_initialized = true;
    updateMatrix(width, height);
}

void BGFXManager::terminate() {
    if (!m_initialized)
        return;

    m_primitiveRenderer.terminate();
    m_lightRenderer.terminate();

    destroyFrameBuffers();

    if (bgfx::isValid(m_texColor))
        bgfx::destroy(m_texColor);
    if (bgfx::isValid(m_texLight))
        bgfx::destroy(m_texLight);
    if (bgfx::isValid(m_ambientColor))
        bgfx::destroy(m_ambientColor);
    if (bgfx::isValid(m_compositeProgram))
        bgfx::destroy(m_compositeProgram);

    m_texColor = BGFX_INVALID_HANDLE;
    m_texLight = BGFX_INVALID_HANDLE;
    m_ambientColor = BGFX_INVALID_HANDLE;
    m_compositeProgram = BGFX_INVALID_HANDLE;

    bgfx::shutdown();
    m_initialized = false;
}

void BGFXManager::updateView(glm::vec2 cameraPosition) {
    m_cameraPosition = cameraPosition;
    bx::mtxLookAt(m_view, bx::Vec3{cameraPosition.x, cameraPosition.y, -1.0f},
                  bx::Vec3{cameraPosition.x, cameraPosition.y, 0.0f},
                  bx::Vec3{0.0f, 1.0f, 0.0f});
}

void BGFXManager::updateMatrix() {
    bgfx::setViewFrameBuffer(static_cast<std::uint16_t>(RenderLayer::World),
                             m_worldFrameBuffer);
    bgfx::setViewFrameBuffer(static_cast<std::uint16_t>(RenderLayer::Light),
                             m_lightFrameBuffer);
    bgfx::setViewFrameBuffer(static_cast<std::uint16_t>(RenderLayer::Composite),
                             BGFX_INVALID_HANDLE);

    const RenderLayer layers[] = {RenderLayer::World, RenderLayer::Light,
                                  RenderLayer::Composite};
    for (auto layer : layers) {
        std::uint16_t viewId = static_cast<std::uint16_t>(layer);
        bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
                           0x000000FF, 1.0f, 0);
        bgfx::setViewTransform(viewId, m_view, m_projection);
        bgfx::setViewRect(viewId, 0, 0,
                          static_cast<std::uint16_t>(m_screenSize.x),
                          static_cast<std::uint16_t>(m_screenSize.y));
    }
}

void BGFXManager::updateMatrix(int width, int height) {
    if (m_screenSize.x != width || m_screenSize.y != height) {
        m_screenSize = glm::vec2(static_cast<float>(width),
                                 static_cast<float>(height));
        bgfx::reset(static_cast<std::uint32_t>(width),
                    static_cast<std::uint32_t>(height), BGFX_RESET_VSYNC);

        destroyFrameBuffers();
        createFrameBuffers(width, height);

        bx::mtxOrtho(m_projection, 0.0f, static_cast<float>(width),
                     static_cast<float>(height), 0.0f, 0.0f, 100.0f, 0.0f,
                     bgfx::getCaps()->homogeneousDepth);
    }

    updateMatrix();
}

void BGFXManager::display(glm::vec4 ambientColor) {
    bgfx::touch(static_cast<std::uint16_t>(RenderLayer::World));
    bgfx::touch(static_cast<std::uint16_t>(RenderLayer::Light));

    bgfx::setTexture(0, m_texColor, bgfx::getTexture(m_worldFrameBuffer));
    bgfx::setTexture(1, m_texLight, bgfx::getTexture(m_lightFrameBuffer));

    m_lightRenderer.setupLightQuad(glm::vec2(0.0f), m_screenSize);

    bgfx::setUniform(m_ambientColor, &ambientColor);
    bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
    bgfx::submit(static_cast<std::uint16_t>(RenderLayer::Composite),
                 m_compositeProgram);

    bgfx::frame();
}

bgfx::TextureHandle BGFXManager::loadTexture(const char *filepath, float *width,
                                             float *height) {
    int loadedWidth = 0;
    int loadedHeight = 0;
    int channels = 0;
    unsigned char *data =
        stbi_load(filepath, &loadedWidth, &loadedHeight, &channels, 4);

    if (data == nullptr) {
        SDL_Log("STB failed to load %s", filepath);
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory *memory = bgfx::copy(data, loadedWidth * loadedHeight * 4);
    stbi_image_free(data);

    bgfx::TextureHandle texture = bgfx::createTexture2D(
        static_cast<std::uint16_t>(loadedWidth),
        static_cast<std::uint16_t>(loadedHeight), false, 1,
        bgfx::TextureFormat::RGBA8,
        BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP |
            BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT,
        memory);

    if (!bgfx::isValid(texture)) {
        SDL_Log("Failed to create GPU texture for %s", filepath);
        return BGFX_INVALID_HANDLE;
    }

    if (width != nullptr)
        *width = static_cast<float>(loadedWidth);
    if (height != nullptr)
        *height = static_cast<float>(loadedHeight);

    return texture;
}

void BGFXManager::renderLight(glm::vec2 center, float intensity, float radius,
                              glm::vec3 color) {
    glm::vec2 position(center.x - radius, center.y - radius);
    glm::vec2 size(radius * 2.0f, radius * 2.0f);

    m_lightRenderer.renderLight(static_cast<std::uint16_t>(RenderLayer::Light),
                                position, size, color, intensity);
}

bgfx::PlatformData BGFXManager::loadWindow(SDL_Window *window) {
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    bgfx::PlatformData platformData{};

#if defined(_WIN32)
    platformData.nwh =
        SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
#elif defined(__linux__)
    std::uint64_t x11Window =
        SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
    void *x11Display =
        SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
    if (x11Window == 0 || x11Display == nullptr) {
        SDL_Log("Couldn't get X11 window/display pointers for bgfx");
        return platformData;
    }
    platformData.nwh = reinterpret_cast<void *>(static_cast<uintptr_t>(x11Window));
    platformData.ndt = x11Display;
#elif defined(__APPLE__)
    platformData.nwh =
        SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
#endif

    return platformData;
}

void BGFXManager::destroyFrameBuffers() {
    if (bgfx::isValid(m_worldFrameBuffer))
        bgfx::destroy(m_worldFrameBuffer);
    if (bgfx::isValid(m_lightFrameBuffer))
        bgfx::destroy(m_lightFrameBuffer);

    m_worldFrameBuffer = BGFX_INVALID_HANDLE;
    m_lightFrameBuffer = BGFX_INVALID_HANDLE;
}

void BGFXManager::createFrameBuffers(int width, int height) {
    std::uint64_t flags =
        BGFX_TEXTURE_RT | BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

    m_worldFrameBuffer = bgfx::createFrameBuffer(
        static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height),
        bgfx::TextureFormat::RGBA8, flags);
    m_lightFrameBuffer = bgfx::createFrameBuffer(
        static_cast<std::uint16_t>(width), static_cast<std::uint16_t>(height),
        bgfx::TextureFormat::RGBA8, flags);
}

} // namespace Hylozoa::BGFX
