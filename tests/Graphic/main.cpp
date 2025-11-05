// tests/Graphic/main.cpp
// This file is part of the tests for everyone Graphic related that cannot be automatically tested.
//
// Manual tests can be added here.
#include <SDL3/SDL.h>

#include <Hylozoa-Engine/Components/Rendering/Sprite.hpp>
#include <Hylozoa-Engine/Components/Color.hpp>
#include <Hylozoa-Engine/Components/Vector2.hpp>

static SDL_Window *window = NULL;
static std::shared_ptr<SDL_Renderer> renderer = nullptr;
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
SDL_FRect fillRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

/* This function runs once at startup. */
bool SDL_AppInit()
{
    SDL_Renderer *tmpRenderer = NULL;

    SDL_SetAppMetadata("Example Renderer Textures", "1.0", "com.example.renderer-textures");
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!SDL_CreateWindowAndRenderer("examples/renderer/textures", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &tmpRenderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return false;
    }
    SDL_SetRenderLogicalPresentation(tmpRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    renderer = std::shared_ptr<SDL_Renderer>(tmpRenderer, SDL_DestroyRenderer);
    return true;
}

static void createRenderTexture()
{
    // Render Texture
    Hylozoa::Components::Rendering::Texture::Specs textureSpecs;
    
    textureSpecs.texturePath = "assets/marioSheet.png";
    textureSpecs.originOffset = {0, 0};
    textureSpecs.textureScale = {1.0f, 1.0f};

    Hylozoa::Components::Rendering::Texture texture(renderer, textureSpecs);
    SDL_FRect destRect = {0, 0, 0, 0};

    texture.getSDLRect(destRect);
    SDL_RenderTexture(renderer.get(), texture.getSDLTexture() , NULL, &destRect);
}

int main()
{
    SDL_AppInit();
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    createRenderTexture();

    // Wait and exit
    SDL_RenderPresent(renderer.get());
    SDL_Delay(3000); // Wait 3 seconds to see the window
    SDL_Quit();
    return 0;
}