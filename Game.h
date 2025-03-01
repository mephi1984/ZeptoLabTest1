#pragma once

#include <SDL2/SDL.h>
#include "GameObjectManager.h"
#include "RenderSystem.h"
#include "Environment.h"

namespace ZL {

class Game {
public:
    Game();
    ~Game();
    
    void setup();
    void run();
    void update();
    void render();
    
    bool shouldExit() const { return exitGameLoop; }

private:
    void processTickCount();
    void drawScene();

    SDL_Window* window;
    SDL_GLContext glContext;
    RenderSystem renderSystem;
    GameObjectManager gameObjects;
    Renderer& renderer;  // Ссылка на renderer из RenderSystem
    
    bool exitGameLoop;
    size_t newTickCount;
    size_t lastTickCount;
    
    static const size_t CONST_TIMER_INTERVAL = 10;
    static const size_t CONST_MAX_TIME_INTERVAL = 1000;
};

} // namespace ZL