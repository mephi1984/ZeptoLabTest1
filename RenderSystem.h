#pragma once
#include "Renderer.h"
#include "ActiveObject.h"
#include "GameObjectManager.h"
#include <string>

namespace ZL {

class RenderSystem {
public:
    RenderSystem() = default;
    Renderer renderer;
    void initialize();
    void drawScene(GameObjectManager& gameObjects);
    Renderer& getRenderer() { return renderer; }
    
    void worldToScreenCoordinates(Vector3f objectPos,
        Matrix4f projectionModelView,
        int screenWidth, int screenHeight,
        int& screenX, int& screenY);

    void drawObjects(GameObjectManager& gameObjects);
    void drawDialog(GameObjectManager& gameObjects);

private:
    void drawWorld(GameObjectManager& gameObjects);
    void drawUI(const GameObjectManager& gameObjects);

    void drawViola(GameObjectManager& gameObjects);

    void drawLoadingScreen(const GameObjectManager& gameObjects);
    void drawFinalScreen(const GameObjectManager& gameObjects);
    void drawMonster(const GameObjectManager& gameObjects);

    ShaderManager shaderManager;
    Matrix4f currentProjectionModelView; // Добавлено для хранения матрицы между drawWorld и drawUI
    int lastMouseX = 0;
    int lastMouseY = 0;
};

} // namespace ZL
