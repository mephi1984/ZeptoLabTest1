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
    glContext = SDL_GL_CreateContext(ZL::Environment::window);

    ZL::BindOpenGlFunctions();
    ZL::CheckGlError();

    std::cout << "Hello 1" << std::endl;

    // Initialize renderer
    /*
    renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultHideCam", "./defaultHideCam.vertex", "./defaultHideCam.fragment");
*/
    // Initialize game objects
    std::cout << "Hello 2" << std::endl;

    // gameObjects.initialize();

    std::cout << "Hello 3" << std::endl;

    renderer.InitOpenGL();

}

void Game::drawScene() {
    // Вместо прямого рисования используем RenderSystem
    renderSystem.drawScene(gameObjects);
}

void Game::processTickCount() {

    #if 0
    if (Environment::finalIsGood)
    {
        return;
    }
    if (Environment::finalIsBad)
    {
        return;
    }

    if (Environment::gameIsLoading)
    {
        if (gameObjects.loadingFunctions.size() != 0)
        {
            bool result = gameObjects.loadingFunctions.begin()->operator()();
            if (result)
            {
                gameObjects.loadingFunctions.erase(gameObjects.loadingFunctions.begin());
            }
        }
        else
        {
            Environment::gameIsLoading = false;
        }
        return;
    }

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
    #endif
}

void Game::render() {
    SDL_GL_MakeCurrent(ZL::Environment::window, glContext);
    ZL::CheckGlError();

    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // drawScene();
    processTickCount();

    SDL_GL_SwapWindow(ZL::Environment::window);
}

void Game::update() {
    SDL_Event event;
    std::cout << "HEE" << std::endl;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            if (gameObjects.loadingThread.joinable())
            {
                gameObjects.loadingThread.join();
            }
            #ifdef AUDIO
            gameObjects.audioPlayerAsync.exit();
            #endif
            Environment::exitGameLoop = true;
            
        }

        if (!Environment::gameIsLoading)
        {
            gameObjects.handleEvent(event);
        }

    }
    render();
}

}  // namespace ZL