#include "GameObjectManager.h"
#include "Environment.h"
#include "ObjLoader.h"
#include "Inventory.h"
#include "QuestScripts.h"
#include "TextModel.h"  // Add this include for LoadFromTextFile

namespace ZL {

const float GameObjectManager::INVENTORY_ICON_SIZE = 64.0f;
const float GameObjectManager::INVENTORY_MARGIN = 10.0f;

void GameObjectManager::initialize() {

  current_room_index = 0;
  objects_in_inventory = 0;

    coneTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./conus.bmp"));

    // Load models
    colorCubeMesh = CreateCube3D(5.0);
    colorCubeMeshMutable.data = CreateCube3D(5.0);
    colorCubeMeshMutable.RefreshVBO();

    testObjMesh = LoadFromObjFile("./chair_01.obj");
    testObjMesh.Scale(10);
    testObjMesh.SwapZandY();
    testObjMeshMutable.data = testObjMesh;
    testObjMeshMutable.RefreshVBO();

    textMesh = ZL::LoadFromTextFile("./textures/mesh_first_room.txt");  // Add ZL:: namespace
    textMesh.Scale(10);
    textMesh.SwapZandY();
    textMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundX(M_PI * 0.5)));
    textMesh.Move(Vector3f{0, 93, 0});

    coneMesh = ZL::LoadFromTextFile("./cone001.txt");  // Add ZL:: namespace
    coneMesh.Scale(200);


    textMeshMutable.AssignFrom(textMesh);
    textMeshMutable.RefreshVBO();
    coneMeshMutable.AssignFrom(coneMesh);
    coneMeshMutable.RefreshVBO();

    // Load bone animations
    //bx.LoadFromFile("./violetta001.txt");
    violaIdleModel.LoadFromFile("./idleviola001.txt");
    violaWalkModel.LoadFromFile("./walkviolla001.txt");
    // Create active object
    
    ActiveObject ao1;
    ao1.name = "book";
    ao1.activeObjectMesh = ZL::LoadFromTextFile("./book001.txt");  // Add ZL:: namespace
    ao1.activeObjectMesh.Scale(4);
    ao1.activeObjectMeshMutable.AssignFrom(ao1.activeObjectMesh);
    ao1.activeObjectMeshMutable.RefreshVBO();
    ao1.objectPos = Vector3f{50, 0, -300};
    ao1.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./book03.bmp"));
    ao1.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
    ao1.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
    ao1.activeObjectScreenMeshMutable.AssignFrom(ao1.activeObjectScreenMesh);
    ao1.activeObjectScreenMeshMutable.RefreshVBO();

    /*
    ActiveObject ao2;
    ao2.name = "superchair001";
    ao2.activeObjectMesh = ZL::LoadFromTextFile("./superchair001.txt");  // Add ZL:: namespace
    ao2.activeObjectMesh.Scale(400);
    ao2.activeObjectMesh.SwapZandY();
    ao2.activeObjectMeshMutable.AssignFrom(ao2.activeObjectMesh);
    ao2.activeObjectMeshMutable.RefreshVBO();
    ao2.objectPos = Vector3f{ 0, 0, 0 };
    ao2.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./chair_01_Base_Color.bmp"));

    ao2.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
    ao2.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
    ao2.activeObjectScreenMeshMutable.AssignFrom(ao2.activeObjectScreenMesh);
    ao2.activeObjectScreenMeshMutable.RefreshVBO();
    */



    Room room_1;
    room_1.roomTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp"));
    room_1.objects.push_back(ao1);
    room_1.sound_name = "Symphony No.6 (1st movement).ogg";
    room_1.roomLogic = createRoom1Logic();
    rooms.push_back(room_1);
    aoMgr.addActiveObject(ao1);

    Room room_2;
    room_2.roomTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./background.bmp"));
    room_2.sound_name = "Symphony No.6 (1st movement).ogg";
    room_2.roomLogic = createRoom1Logic();
    rooms.push_back(room_2);

    activeObjects = rooms[current_room_index].objects;

    // Initialize audio
    audioPlayer = std::make_unique<AudioPlayer>();
    if (audioPlayer) {
        audioPlayer->playMusic(rooms[current_room_index].sound_name);
    }

    // Initialize inventory
    inventoryIconMesh = CreateRect2D(
        {0.0f, 40.0f},
        {INVENTORY_ICON_SIZE/2, INVENTORY_ICON_SIZE/2},
        0.5f
    );
    inventoryIconMeshMutable.AssignFrom(inventoryIconMesh);
    inventoryIconMeshMutable.RefreshVBO();

    roomTexturePtr = rooms[current_room_index].roomTexture;

    AddItemToInventory("book1", std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp")), objects_in_inventory+1);
    objects_in_inventory++;
    AddItemToInventory("book2", std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp")), objects_in_inventory+1);
    objects_in_inventory++;


    //SDL_ShowCursor(SDL_DISABLE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void GameObjectManager::switch_room(int index){
    current_room_index = index;

    roomTexturePtr = rooms[current_room_index].roomTexture;

    audioPlayer.reset();  // This deletes the current AudioPlayer

    // Reinitialize it
    audioPlayer = std::make_unique<AudioPlayer>();
    if (audioPlayer) {
        audioPlayer->playMusic(rooms[current_room_index].sound_name);
    }

    activeObjects = rooms[current_room_index].objects;

    std::cout << "Current music" << rooms[current_room_index].sound_name << std::endl;
}



void GameObjectManager::handleEvent(const SDL_Event& event) {
//  debug room switching
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
        switch_room(1);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
      const auto highlightedObjects = aoMgr.findByHighlighted(true);

    for (auto* ao : highlightedObjects) {
        if (!ao) {
            continue;
        }

        AddItemToInventory(ao->name, ao->activeObjectTexturePtr, objects_in_inventory+1);
        objects_in_inventory++;

        aoMgr.removeByName(ao->name);
    }
//        bx.Interpolate(animationCounter);
//        animationCounter += 2;
    }

    else if (event.type == SDL_MOUSEWHEEL) {
        static const float zoomstep = 1.0f;
        if (event.wheel.y > 0) {
            Environment::zoom -= zoomstep;
        }
        else if (event.wheel.y < 0) {
            Environment::zoom += zoomstep;
        }
        if (Environment::zoom < zoomstep) {
            Environment::zoom = zoomstep;
        }
        if (Environment::zoom > 4)
        {
            Environment::zoom = 4;
        }
    }
    else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_SPACE:
            Environment::showMouse = !Environment::showMouse;

            if (Environment::showMouse)
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
            }
            else
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                lastMouseX = 0;
                lastMouseY = 0;
            }
            break;

            case SDLK_ESCAPE:
            case SDLK_q:
                Environment::exitGameLoop = true;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                Environment::leftPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                if (Environment::violaCurrentAnimation == 0)
                {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                if (Environment::violaCurrentAnimation == 0)
                {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                if (Environment::violaCurrentAnimation == 0)
                {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                if (Environment::violaCurrentAnimation == 0)
                {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;

            case SDLK_1:
            case SDLK_2:
            {
                int hot_key = (event.key.keysym.sym == SDLK_1) ? 1 : 2;
                UnselectAllItems();
                if (InventoryItem* item = GetItemByHotkey(hot_key)) {
                    item->isSelected = true;
                }
            }
            break;
            // ...handle other keys...
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                Environment::leftPressed = false;
                if (Environment::leftPressed == false && Environment::rightPressed == false && Environment::upPressed == false && Environment::downPressed == false)
                {
                    if (Environment::violaCurrentAnimation == 1)
                    {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = false;
                if (Environment::leftPressed == false && Environment::rightPressed == false && Environment::upPressed == false && Environment::downPressed == false)
                {
                    if (Environment::violaCurrentAnimation == 1)
                    {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = false;
                if (Environment::leftPressed == false && Environment::rightPressed == false && Environment::upPressed == false && Environment::downPressed == false)
                {
                    if (Environment::violaCurrentAnimation == 1)
                    {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = false;
                if (Environment::leftPressed == false && Environment::rightPressed == false && Environment::upPressed == false && Environment::downPressed == false)
                {
                    if (Environment::violaCurrentAnimation == 1)
                    {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        
        if (Environment::showMouse == false)
        {
            int mouseX, mouseY;
            SDL_GetRelativeMouseState(&mouseX, &mouseY);

            float diffX = 0.01f * mouseX;

            float diffY = 0.01f * mouseY;

            Environment::cameraPhi += diffX;

            if (Environment::settings_inverseVertical)
            {
                Environment::cameraAlpha -= diffY;
            }
            else
            {
                Environment::cameraAlpha += diffY;
            }
            if (Environment::cameraAlpha < 0.1 * M_PI / 2.0)
            {
                Environment::cameraAlpha = 0.1 * M_PI / 2.0;
            }
            else if (Environment::cameraAlpha > 0.9 * M_PI / 2.0)
            {
                Environment::cameraAlpha = 0.9 * M_PI / 2.0;
            }

        }
        else
        {
            lastMouseX = event.motion.x;
            lastMouseY = event.motion.y;
        }
    }
}


void GameObjectManager::updateScene(size_t ms) {
    const float SPEED = 0.1f;

    Vector2f directionVector = { 0.f, SPEED }; //x and z

    // Вычисляем новые координаты вектора
    float x_new = directionVector.v[0] * cos(Environment::cameraPhi) - directionVector.v[1] * sin(Environment::cameraPhi);
    float y_new = directionVector.v[0] * sin(Environment::cameraPhi) + directionVector.v[1] * cos(Environment::cameraPhi);

    // Обновляем вектор
    directionVector.v[0] = x_new;
    directionVector.v[1] = y_new;

    //Only forward is allowed
    /*
    if (Environment::leftPressed) {
        Environment::cameraShift.v[0] += SPEED * ms;
    }
    if (Environment::rightPressed) {
        Environment::cameraShift.v[0] -= SPEED * ms;
    }
    if (Environment::upPressed) {
        Environment::cameraShift.v[2] += SPEED * ms;
    }
    if (Environment::downPressed) {
        Environment::cameraShift.v[2] -= SPEED * ms;
    }*/

    if (Environment::upPressed) {
        Environment::cameraShift.v[0] += directionVector.v[0] * ms;
        Environment::cameraShift.v[2] += directionVector.v[1] * ms;
    }
    if (Environment::downPressed) {
        Environment::cameraShift.v[0] -= directionVector.v[0] * ms;
        Environment::cameraShift.v[2] -= directionVector.v[1] * ms;
    }
    if (Environment::rightPressed) {
        Environment::cameraShift.v[2] += directionVector.v[0] * ms;
        Environment::cameraShift.v[0] -= directionVector.v[1] * ms;
    }
    if (Environment::leftPressed) {
        Environment::cameraShift.v[2] -= directionVector.v[0] * ms;
        Environment::cameraShift.v[0] += directionVector.v[1] * ms;
    }

    Environment::characterPos.v[0] = -Environment::cameraShift.v[0];
    Environment::characterPos.v[1] = -Environment::cameraShift.v[1];
    Environment::characterPos.v[2] = -Environment::cameraShift.v[2];

    for (auto& [key, obj] : aoMgr.activeObjectsEntities) {
        float dist = sqrtf(
            pow(Environment::characterPos.v[0] - obj.objectPos.v[0], 2) +
            pow(Environment::characterPos.v[1] - obj.objectPos.v[1], 2) +
            pow(Environment::characterPos.v[2] - obj.objectPos.v[2], 2)
        );
        obj.highlighted = (dist < 50.f);
    }


    if (rooms[current_room_index].roomLogic) {
        rooms[current_room_index].roomLogic(*this, ms);
    }

    if (Environment::violaCurrentAnimation == 0)
    {

        Environment::violaCurrentIdleFrame += ms / 24.f;

        while (Environment::violaCurrentIdleFrame >= 40)
        {
            Environment::violaCurrentIdleFrame -= 40;
        }

        if (int(Environment::violaCurrentIdleFrame) != Environment::violaLastIdleFrame)
        {
            violaIdleModel.Interpolate(int(Environment::violaCurrentIdleFrame));
            Environment::violaLastIdleFrame = int(Environment::violaCurrentIdleFrame);
        }
    }
    else if (Environment::violaCurrentAnimation == 1)
    {
        Environment::violaCurrentWalkFrame += ms / 24.f;

        while (Environment::violaCurrentWalkFrame >= 30)
        {
            Environment::violaCurrentWalkFrame -= 30;
        }

        if (int(Environment::violaCurrentWalkFrame) != Environment::violaLastWalkFrame)
        {
            violaWalkModel.Interpolate(int(Environment::violaCurrentWalkFrame));
            Environment::violaLastWalkFrame = int(Environment::violaCurrentWalkFrame);
        }
    }
}

bool GameObjectManager::isPointInObject(int screenX, int screenY, int objectScreenX, int objectScreenY) const {
    // Простая проверка попадания точки в квадрат 64x64 вокруг центра объекта
    const int objectSize = 32; // Половина размера области выделения
    return (screenX >= objectScreenX - objectSize && 
            screenX <= objectScreenX + objectSize &&
            screenY >= objectScreenY - objectSize && 
            screenY <= objectScreenY + objectSize);
}

void GameObjectManager::checkMouseIntersection(int mouseX, int mouseY, const Matrix4f& projectionModelView) {
    for (auto& ao : activeObjects) {
        int screenX, screenY;
        worldToScreenCoordinates(ao.objectPos, projectionModelView, 
            Environment::width, Environment::height, screenX, screenY);
            
        if (isPointInObject(mouseX, mouseY, screenX, screenY)) {
            std::cout << "Mouse over object at screen coordinates: " 
                      << screenX << ", " << screenY 
                      << " (world pos: " 
                      << ao.objectPos.v[0] << ", "
                      << ao.objectPos.v[1] << ", "
                      << ao.objectPos.v[2] << ")" 
                      << std::endl;
        }
    }
}

void GameObjectManager::worldToScreenCoordinates(Vector3f objectPos,
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

}  // namespace ZL
