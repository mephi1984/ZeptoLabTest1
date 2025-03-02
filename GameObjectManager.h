#pragma once
#include "TextureManager.h"
#include "BoneAnimatedModel.h"
#include "AudioPlayerAsync.h"
#include <memory>
#include <vector>
#include <list>
#include "ActiveObject.h"
#include "Room.h"
#ifdef __linux__
#include <SDL2/SDL.h>
#endif
#include "OpenGlExtensions.h"
#include <thread>

namespace ZL {

class GameObjectManager {
public:
    void initializeLoadingScreen();
    void initialize();

    void switch_room(int index);

    void handleEvent(const SDL_Event& event);
    void updateScene(size_t ms);
    void checkMouseIntersection(int mouseX, int mouseY, const Matrix4f& projectionModelView); // Добавляем новый метод

    std::shared_ptr<ZL::Texture> testObjTexturePtr;
    //std::shared_ptr<ZL::Texture> roomTexturePtr;
    std::shared_ptr<ZL::Texture> coneTexturePtr;

    //ZL::VertexDataStruct colorCubeMesh;
    //ZL::VertexRenderStruct colorCubeMeshMutable;

    //ZL::VertexDataStruct testObjMesh;
    //ZL::VertexRenderStruct testObjMeshMutable;

    ZL::BoneSystem violaIdleModel;
    ZL::VertexRenderStruct violaIdleModelMutable;

    ZL::BoneSystem violaWalkModel;
    ZL::VertexRenderStruct violaWalkModelMutable;

    std::vector<ZL::VertexDataStruct> preloadedRoomMeshArr;


    //ZL::VertexDataStruct coneMesh;      // Раскомментировали
    //ZL::VertexRenderStruct coneMeshMutable;  // Раскомментировали

    std::vector<ZL::ActiveObject> activeObjects;
    std::vector<ZL::Room> rooms;
    
    AudioPlayerAsync audioPlayerAsync;

    ZL::VertexDataStruct inventoryIconMesh;
    ZL::VertexRenderStruct inventoryIconMeshMutable;
    
    static const float INVENTORY_ICON_SIZE;
    static const float INVENTORY_MARGIN;
    ActiveObjectManager aoMgr;
    int objects_in_inventory;


    std::shared_ptr<ZL::Texture> loadingScreenTexturePtr;

    ZL::VertexDataStruct loadingScreenMesh;
    ZL::VertexRenderStruct loadingScreenMeshMutable;

    std::list<std::function<bool()>> loadingFunctions;
    std::thread loadingThread;
    bool sideThreadLoadingCompleted = false;

    int current_room_index;
private:

    //int animationCounter = 0;
    int lastMouseX = 0;  // Добавляем переменные для хранения позиции мыши
    int lastMouseY = 0;
    
    bool isPointInObject(int screenX, int screenY, int objectScreenX, int objectScreenY) const;
    void worldToScreenCoordinates(Vector3f objectPos,  // Добавляем метод
        Matrix4f projectionModelView,
        int screenWidth, int screenHeight,
        int& screenX, int& screenY);
    BoundaryBox walkArea{800.0f, 800.0f}; // Зона для ходьбы 800x800
};

}  // namespace ZL
