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

int main()
{
    SDL_AppInit();
    Hylozoa::Components::Rendering::SpriteSpecs spriteSpecs;
    SDL_FRect fillRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

    spriteSpecs.color = Hylozoa::Components::Color{255, 0, 0, 255};
    spriteSpecs.scale = Hylozoa::Components::Vector2{1.0f, 1.0f};
    spriteSpecs.visible = true;
    spriteSpecs.layer = 0;
    spriteSpecs.transparency = 0;
    //spriteSpecs.shapeSpecs.type = Hylozoa::Components::Rendering::SpriteType::Rectangle;
    //spriteSpecs.shapeSpecs.specifics = Hylozoa::Components::Rendering::SpriteSpecs::ShapeSpecs::RectangleSpecs{200
    spriteSpecs.shapeSpecs.outlineThickness = 5.0f;
    spriteSpecs.shapeSpecs.outlineColor = Hylozoa::Components::Color{255, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    // Render Circle
    Hylozoa::Components::Rendering::Sprite circle(renderer, spriteSpecs);
    fillRect.w = 200;
    fillRect.h = 200;
    SDL_RenderTexture(renderer.get(), circle.getSDLTexture() , NULL, &fillRect);

    // Render Rectangle
    spriteSpecs.shapeSpecs.type = Hylozoa::Components::Rendering::SpriteType::Rectangle;
    spriteSpecs.shapeSpecs.specifics = Hylozoa::Components::Rendering::SpriteSpecs::ShapeSpecs::RectangleSpecs{200, 100};
    Hylozoa::Components::Rendering::Sprite rectangle(renderer, spriteSpecs);
    fillRect.w = 220;
    fillRect.h = 200;
    SDL_RenderTexture(renderer.get(), rectangle.getSDLTexture() , NULL, &fillRect);

    // Render Texture
    Hylozoa::Components::Rendering::SpriteSpecs::ShapeSpecs::TextureSpecs textureSpecs;
    
    textureSpecs.texturePath = "assets/marioSheet.png";
    textureSpecs.originOffset = Hylozoa::Components::Vector2{0, 0};
    textureSpecs.textureScale = Hylozoa::Components::Vector2{1.0f, 1.0f};
    spriteSpecs.shapeSpecs.type = Hylozoa::Components::Rendering::SpriteType::Texture;
    spriteSpecs.shapeSpecs.specifics = textureSpecs;
    Hylozoa::Components::Rendering::Sprite texture(renderer, spriteSpecs);
    fillRect.w = 192;
    fillRect.h = 108;
    SDL_RenderTexture(renderer.get(), texture.getSDLTexture() , NULL, &fillRect);
    
    // Wait and exit
    SDL_RenderPresent(renderer.get());
    SDL_Delay(3000); // Wait 3 seconds to see the window
    SDL_Quit();
    return 0;
}