#pragma once
#include "Renderer.h"
#include "GameObjectManager.h"
#include <string>

namespace ZL {

class RenderSystem {
public:
    RenderSystem() = default;
    void initialize();
    void drawScene(const GameObjectManager& gameObjects);
    Renderer& getRenderer() { return renderer; }
    
    void worldToScreenCoordinates(Vector3f objectPos,
        Matrix4f projectionModelView,
        int screenWidth, int screenHeight,
        int& screenX, int& screenY);

private:
    void drawWorld(const GameObjectManager& gameObjects);
    void drawUI(const GameObjectManager& gameObjects);
    
    Renderer renderer;
    ShaderManager shaderManager;
    Matrix4f currentProjectionModelView; // Добавлено для хранения матрицы между drawWorld и drawUI
    int lastMouseX = 0;
    int lastMouseY = 0;
};

} // namespace ZL
