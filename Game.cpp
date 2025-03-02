#include "Game.h"
#include "AnimatedModel.h"
#include "BoneAnimatedModel.h"
#include "Utils.h"
#include "Inventory.h"  // Add this include
#include "OpenGlExtensions.h"
#include <iostream>

namespace ZL
{

Game::Game() 
    : window(nullptr)
    , glContext(nullptr)
    , newTickCount(0)
    , lastTickCount(0)
    , renderer(renderSystem.getRenderer())  // Инициализация ссылки на renderer
{
}

Game::~Game() {
    if (glContext) {
        SDL_GL_DeleteContext(glContext);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void Game::setup() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        Environment::width, Environment::height, SDL_WINDOW_OPENGL);

    Environment::window = window;
    
    glContext = SDL_GL_CreateContext(window);

    ZL::BindOpenGlFunctions();
    ZL::CheckGlError();

    std::cout << "Hello 1" << std::endl;

    // Initialize renderer
    renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultHideCam", "./defaultHideCam.vertex", "./defaultHideCam.fragment");

    // Initialize game objects
    std::cout << "Hello 2" << std::endl;

    gameObjects.initialize();

    std::cout << "Hello 3" << std::endl;

    renderer.InitOpenGL();

}

void Game::drawScene() {
    // Вместо прямого рисования используем RenderSystem
    renderSystem.drawScene(gameObjects);
}

void Game::processTickCount() {
    if (lastTickCount == 0) {
        lastTickCount = SDL_GetTicks64();
        return;
    }

    newTickCount = SDL_GetTicks64();
    if (newTickCount - lastTickCount > CONST_TIMER_INTERVAL) {
        size_t delta = (newTickCount - lastTickCount > CONST_MAX_TIME_INTERVAL) ? 
            CONST_MAX_TIME_INTERVAL : newTickCount - lastTickCount;
        
        gameObjects.updateScene(delta);
        
        lastTickCount = newTickCount;
    }
}

void Game::render() {
    SDL_GL_MakeCurrent(window, glContext);
    ZL::CheckGlError();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawScene();
    processTickCount();

    SDL_GL_SwapWindow(window);
}

void Game::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            Environment::exitGameLoop = true;
        }
        gameObjects.handleEvent(event);

    }
    render();
}

}  // namespace ZL