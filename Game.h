#pragma once

#include <SDL2/SDL.h>
#include "GameObjectManager.h"
#include "Renderer.h"
#include "Environment.h"

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
    ZL::Renderer renderer;
    GameObjectManager gameObjects;
    
    bool exitGameLoop;
    size_t newTickCount;
    size_t lastTickCount;
    
    static const size_t CONST_TIMER_INTERVAL = 10;
    static const size_t CONST_MAX_TIME_INTERVAL = 1000;
};