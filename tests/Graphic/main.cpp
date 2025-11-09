// tests/Graphic/main.cpp
// This file is part of the tests for everyone Graphic related that cannot be automatically tested.
//
// Manual tests can be added here.
#include <SDL3/SDL.h>

#include <Hylozoa-Engine/Components/Rendering/Renderable.hpp>
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

static void createRenderAnimatedTexture()
{
    // Render Texture
    Hylozoa::Components::Rendering::RenderableTexture::Specs textureSpecs;
    
    textureSpecs.texturePath = "assets/marioSheet.png";
    textureSpecs.originOffset = {0, 0};
    textureSpecs.textureScale = {1.0f, 1.0f};

    Hylozoa::Components::Rendering::RenderableTexture texture(renderer, textureSpecs);
    SDL_FRect destRect = {0, 0, 0, 200};

    texture.getSDLRect(destRect);
    // Animation
    Hylozoa::Components::Rendering::RenderableTexture::AnimationSpecs animation;
    animation.frameRectWidth = 32;
    animation.frameRectHeight = 32;
    animation.frameCount = 4;
    animation.frameDuration = 0.2f; // seconds per frame
    animation.frameDisplacement = {32, 0}; // offset between frames
    // Set first frame
    animation.frameRect = {0, 0, animation.frameRectWidth, animation.frameRectHeight};
    SDL_RenderTexture(renderer.get(), texture.getSDLTexture() , &animation.frameRect, &destRect);
}

static void createRenderTexture()
{
    // Render Texture
    Hylozoa::Components::Rendering::RenderableTexture::Specs textureSpecs;
    
    textureSpecs.texturePath = "assets/marioSheet.png";
    textureSpecs.originOffset = {0, 0};
    textureSpecs.textureScale = {1.0f, 1.0f};

    Hylozoa::Components::Rendering::RenderableTexture texture(renderer, textureSpecs);
    SDL_FRect destRect = {0, 0, 0, 0};

    texture.getSDLRect(destRect);
    SDL_RenderTexture(renderer.get(), texture.getSDLTexture() , NULL, &destRect);
}

static void createRenderRectangle()
{
    Hylozoa::Components::Rendering::Renderable sprite;

    sprite.color = Hylozoa::Components::Color{255, 0, 0, 255};
    Hylozoa::Components::Rendering::RenderableShape shape;

    shape.type = Hylozoa::Components::Rendering::RenderableShape::ShapeType::Rectangle;
    shape.specs = Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs{20.0f, 50.f};
    shape.outlineColor = {255, 255, 255, 255};
    shape.outlineThickness = 2.0f;
    SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);

    // Use std::variant to get specs
    Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs& rectSpecs = std::get<Hylozoa::Components::Rendering::RenderableShape::RectangleSpecs>(shape.specs);

    fillRect.y = 150;
    fillRect.w = rectSpecs.width;
    fillRect.h = rectSpecs.height;

    SDL_RenderFillRect(renderer.get(), &fillRect);
}

static void createRenderCircle()
{
    // Lets implement circle rendering from a radius variable
    Hylozoa::Components::Rendering::Renderable sprite;
    sprite.color = Hylozoa::Components::Color{0, 255, 0, 255};
    Hylozoa::Components::Rendering::RenderableShape shape;
    shape.type = Hylozoa::Components::Rendering::RenderableShape::ShapeType::Circle;
    shape.specs = Hylozoa::Components::Rendering::RenderableShape::CircleSpecs{
        40.0f
    };
    shape.outlineColor = {255, 255, 255, 255};
    shape.outlineThickness = 2.0f;

    SDL_SetRenderDrawColor(renderer.get(), sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a);
    // Use std::variant to get specs
    Hylozoa::Components::Rendering::RenderableShape::CircleSpecs& circleSpecs = std::get<Hylozoa::Components::Rendering::RenderableShape::CircleSpecs>(shape.specs);
    // Simple circle rendering algorithm (not optimized)
    int centerX = 100;;
    int centerY = 350;
    int radius = static_cast<int>(circleSpecs.radius);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderPoint(renderer.get(), centerX + dx, centerY + dy);
            }
        }
    }
}

int main()
{
    SDL_AppInit();
    SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255);
    SDL_RenderClear(renderer.get());

    createRenderTexture();
    createRenderRectangle();
    createRenderCircle();
    createRenderAnimatedTexture();

    // Wait and exit
    SDL_RenderPresent(renderer.get());
    SDL_Delay(3000); // Wait 3 seconds to see the window
    SDL_Quit();
    return 0;
}