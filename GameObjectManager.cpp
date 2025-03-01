#include "GameObjectManager.h"
#include "Environment.h"
#include "ObjLoader.h"
#include "Inventory.h"
#include "TextModel.h"  // Add this include for LoadFromTextFile

namespace ZL {

const float GameObjectManager::INVENTORY_ICON_SIZE = 32.0f;
const float GameObjectManager::INVENTORY_MARGIN = 10.0f;

void GameObjectManager::initialize() {
    // Load textures
    roomTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp"));
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

    textMesh = ZL::LoadFromTextFile("./mesh001.txt");  // Add ZL:: namespace
    coneMesh = ZL::LoadFromTextFile("./cone001.txt");  // Add ZL:: namespace
    coneMesh.Scale(200);

    textMeshMutable.AssignFrom(textMesh);
    textMeshMutable.RefreshVBO();
    coneMeshMutable.AssignFrom(coneMesh);
    coneMeshMutable.RefreshVBO();

    // Load bone animations
    bx.LoadFromFile("mesh_armature_and_animation_data.txt");

    // Create active object
    ActiveObject ao1;
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
    activeObjects.push_back(ao1);

    // Initialize audio
    audioPlayer = std::make_unique<AudioPlayer>();
    if (audioPlayer) {
        audioPlayer->playMusic("Symphony No.6 (1st movement).ogg");
    }

    // Initialize inventory
    inventoryIconMesh = CreateRect2D(
        {0.0f, 0.0f},
        {INVENTORY_ICON_SIZE/2, INVENTORY_ICON_SIZE/2},
        0.5f
    );
    inventoryIconMeshMutable.AssignFrom(inventoryIconMesh);
    inventoryIconMeshMutable.RefreshVBO();

    // Add test items to inventory
    auto testRoomTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Kitchen_ceramics.bmp"));
    auto testConeTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./conus.bmp"));
    AddItemToInventory("RoomCeramics", testRoomTexture);
    AddItemToInventory("Cone", testConeTexture);
}

void GameObjectManager::update() {
    updateScene(16); // Добавим фиксированный timestep для обновления сцены
}

void GameObjectManager::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        bx.Interpolate(animationCounter);
        animationCounter += 2;
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
    }
    else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_a:
                Environment::leftPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = true;
                if (audioPlayer) {
                    audioPlayer->playSound("Звук-Идут-по-земле.ogg");
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
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = false;
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = false;
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = false;
                break;
        }
    }
}

void GameObjectManager::updateScene(size_t ms) {
    const float SPEED = 0.1f;
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
    }

    Environment::characterPos.v[0] = -Environment::cameraShift.v[0];
    Environment::characterPos.v[1] = -Environment::cameraShift.v[1];
    Environment::characterPos.v[2] = -Environment::cameraShift.v[2];

    for (auto& ao : activeObjects) {
        float dist = sqrtf(
            pow(Environment::characterPos.v[0] - ao.objectPos.v[0], 2) +
            pow(Environment::characterPos.v[1] - ao.objectPos.v[1], 2) +
            pow(Environment::characterPos.v[2] - ao.objectPos.v[2], 2)
        );
        ao.highlighted = (dist < 50.f);
    }
}

}  // namespace ZL
