#include "Game.h"
#include "AnimatedModel.h"
#include "BoneAnimatedModel.h"
#include "Utils.h"
#include "Inventory.h"  // Add this include
#include <SDL2/SDL.h>
#include <iostream>

namespace ZL
{

void Game::worldToScreenCoordinates(Vector3f objectPos,
    Matrix4f projectionModelView,
    int screenWidth, int screenHeight,
    int& screenX, int& screenY) {

    Vector4f inx = { objectPos.v[0], objectPos.v[1], objectPos.v[2], 1.0f};
    Vector4f clipCoords = MultMatrixVector(projectionModelView, inx);

    float ndcX = clipCoords.v[0] / clipCoords.v[3];
    float ndcY = clipCoords.v[1] / clipCoords.v[3];

    screenX = (int)((ndcX + 1.0f) * 0.5f * screenWidth);
    screenY = (int)((1.0f + ndcY) * 0.5f * screenHeight);
}

Game::Game() : window(nullptr), glContext(nullptr), exitGameLoop(false), 
    newTickCount(0), lastTickCount(0) {
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
    
    glContext = SDL_GL_CreateContext(window);

    ZL::BindOpenGlFunctions();
    ZL::CheckGlError();

    // Initialize renderer
    renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
    renderer.InitOpenGL();

    // Initialize game objects
    gameObjects.initialize();
}

void Game::drawScene() {
    static const std::string defaultShaderName = "default";
    static const std::string vPositionName = "vPosition";
    static const std::string vTexCoordName = "vTexCoord";
    static const std::string textureUniformName = "Texture";

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    
    glViewport(0, 0, Environment::width, Environment::height);

    renderer.shaderManager.PushShader(defaultShaderName);
    renderer.RenderUniform1i(textureUniformName, 0);

    renderer.EnableVertexAttribArray(vPositionName);
    renderer.EnableVertexAttribArray(vTexCoordName);

    // 3D Scene rendering
    {
        renderer.PushPerspectiveProjectionMatrix(1.0 / 1.5, 
            static_cast<float>(Environment::width) / static_cast<float>(Environment::height), 
            50, 10000);
        renderer.PushMatrix();

        renderer.LoadIdentity();
        renderer.TranslateMatrix({ 0,0, -100 * Environment::zoom });
        
        float t = 0.3;
        renderer.RotateMatrix(QuatFromRotateAroundX(t * M_PI / 2.0));

        // Draw cone
        glBindTexture(GL_TEXTURE_2D, gameObjects.coneTexturePtr->getTexID());
        renderer.DrawVertexRenderStruct(gameObjects.coneMeshMutable);

        renderer.TranslateMatrix(Environment::cameraShift);

        // Draw active objects
        for (auto& ao : gameObjects.activeObjects) {
            renderer.PushMatrix();
            renderer.TranslateMatrix(ao.objectPos);
            glBindTexture(GL_TEXTURE_2D, ao.activeObjectTexturePtr->getTexID());
            renderer.DrawVertexRenderStruct(ao.activeObjectMeshMutable);
            renderer.PopMatrix();
        }

        // Draw room
        glBindTexture(GL_TEXTURE_2D, gameObjects.roomTexturePtr->getTexID());
        renderer.DrawVertexRenderStruct(gameObjects.textMeshMutable);

        auto latestProjectionModelView = renderer.GetProjectionModelViewMatrix();

        renderer.PopMatrix();
        renderer.PopProjectionMatrix();

        // 2D UI rendering
        renderer.PushProjectionMatrix(static_cast<float>(Environment::width), 
            static_cast<float>(Environment::height));
        renderer.PushMatrix();
        renderer.LoadIdentity();

        // Draw highlighted objects UI
        for (auto& ao : gameObjects.activeObjects) {
            if (ao.highlighted) {
                int screenX, screenY;
                worldToScreenCoordinates(ao.objectPos, latestProjectionModelView, 
                    Environment::width, Environment::height, screenX, screenY);
                renderer.PushMatrix();
                renderer.TranslateMatrix(Vector3f{screenX + 0.f, screenY + 0.f, 0.0f});
                glBindTexture(GL_TEXTURE_2D, ao.activeObjectScreenTexturePtr->getTexID());
                renderer.DrawVertexRenderStruct(ao.activeObjectScreenMeshMutable);
                renderer.PopMatrix();
            }
        }

        // Draw inventory
        const auto& inventory = ZL::ReturnInventory();
        for (size_t i = 0; i < inventory.size(); ++i) {
            renderer.PushMatrix();
            float xPos = Environment::width - gameObjects.INVENTORY_MARGIN - gameObjects.INVENTORY_ICON_SIZE;
            float yPos = gameObjects.INVENTORY_MARGIN + i * (gameObjects.INVENTORY_ICON_SIZE + gameObjects.INVENTORY_MARGIN);
            renderer.TranslateMatrix(Vector3f{xPos, yPos, 0.0f});
            glBindTexture(GL_TEXTURE_2D, inventory[i].texture->getTexID());
            renderer.DrawVertexRenderStruct(gameObjects.inventoryIconMeshMutable);
            renderer.PopMatrix();
        }

        renderer.PopMatrix();
        renderer.PopProjectionMatrix();
    }

    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);
    renderer.shaderManager.PopShader();
    
    CheckGlError();
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
        
        gameObjects.update();
        
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
            exitGameLoop = true;
        }
        gameObjects.handleEvent(event);
    }
    render();
}

}  // namespace ZL