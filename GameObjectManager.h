#pragma once
#include "TextureManager.h"
#include "BoneAnimatedModel.h"
#include "cmakeaudioplayer/include/AudioPlayer.hpp"
#include <memory>
#include <vector>
#include "ActiveObject.h"
#include <SDL2/SDL.h>

namespace ZL {

class GameObjectManager {
public:
    void initialize();
    void update();
    void handleEvent(const SDL_Event& event);
    void updateScene(size_t ms);
    void checkMouseIntersection(int mouseX, int mouseY, const Matrix4f& projectionModelView); // Добавляем новый метод

    std::shared_ptr<ZL::Texture> testObjTexturePtr;
    std::shared_ptr<ZL::Texture> roomTexturePtr;
    std::shared_ptr<ZL::Texture> coneTexturePtr;

    ZL::VertexDataStruct colorCubeMesh;
    ZL::VertexRenderStruct colorCubeMeshMutable;

    ZL::VertexDataStruct testObjMesh;
    ZL::VertexRenderStruct testObjMeshMutable;

    ZL::BoneSystem bx;
    ZL::VertexRenderStruct bxMutable;

    ZL::VertexDataStruct textMesh;
    ZL::VertexRenderStruct textMeshMutable;

    ZL::VertexDataStruct coneMesh;
    ZL::VertexRenderStruct coneMeshMutable;

    std::vector<ActiveObject> activeObjects;
    std::unique_ptr<AudioPlayer> audioPlayer;

    ZL::VertexDataStruct inventoryIconMesh;
    ZL::VertexRenderStruct inventoryIconMeshMutable;
    
    static const float INVENTORY_ICON_SIZE;
    static const float INVENTORY_MARGIN;

private:
    int animationCounter = 0;
    int lastMouseX = 0;  // Добавляем переменные для хранения позиции мыши
    int lastMouseY = 0;
    bool isPointInObject(int screenX, int screenY, int objectScreenX, int objectScreenY) const;
    void worldToScreenCoordinates(Vector3f objectPos,  // Добавляем метод
        Matrix4f projectionModelView,
        int screenWidth, int screenHeight,
        int& screenX, int& screenY);
};

}  // namespace ZL
