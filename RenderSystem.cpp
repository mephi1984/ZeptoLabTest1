#include "RenderSystem.h"
#include "Environment.h"
#include "Utils.h"
#include "Inventory.h"
#include <GL/gl.h>

namespace ZL {

void RenderSystem::initialize() {
    renderer.shaderManager.AddShaderFromFiles("default", "./default.vertex", "./default.fragment");
    renderer.shaderManager.AddShaderFromFiles("defaultColor", "./defaultColor.vertex", "./defaultColor.fragment");
    renderer.InitOpenGL();
}

void RenderSystem::drawScene(const GameObjectManager& gameObjects) {
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

    drawWorld(gameObjects);
    drawUI(gameObjects);

    renderer.DisableVertexAttribArray(vPositionName);
    renderer.DisableVertexAttribArray(vTexCoordName);
    renderer.shaderManager.PopShader();
    
    CheckGlError();
}

void RenderSystem::drawWorld(const GameObjectManager& gameObjects) {
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
    for (const auto& ao : gameObjects.activeObjects) {
        renderer.PushMatrix();
        renderer.TranslateMatrix(ao.objectPos);
        glBindTexture(GL_TEXTURE_2D, ao.activeObjectTexturePtr->getTexID());
        renderer.DrawVertexRenderStruct(ao.activeObjectMeshMutable);
        renderer.PopMatrix();
    }

    // Draw room
    glBindTexture(GL_TEXTURE_2D, gameObjects.roomTexturePtr->getTexID());
    renderer.DrawVertexRenderStruct(gameObjects.textMeshMutable);

    Matrix4f latestProjectionModelView = renderer.GetProjectionModelViewMatrix();

    renderer.PopMatrix();
    renderer.PopProjectionMatrix();
    
    // Store matrix for UI rendering
    currentProjectionModelView = latestProjectionModelView;
}

void RenderSystem::drawUI(const GameObjectManager& gameObjects) {
    renderer.PushProjectionMatrix(static_cast<float>(Environment::width), 
        static_cast<float>(Environment::height));
    renderer.PushMatrix();
    renderer.LoadIdentity();

    // Draw highlighted objects UI
    for (const auto& ao : gameObjects.activeObjects) {
        if (ao.highlighted) {
            int screenX, screenY;
            worldToScreenCoordinates(ao.objectPos, currentProjectionModelView, 
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

void RenderSystem::worldToScreenCoordinates(Vector3f objectPos,
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

} // namespace ZL
