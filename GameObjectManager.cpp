#include "GameObjectManager.h"
#include "Environment.h"
#include "ObjLoader.h"
#include "Inventory.h"
#include "QuestScripts.h"
#include "TextModel.h"  // Add this include for LoadFromTextFile

namespace ZL {

const float GameObjectManager::INVENTORY_ICON_SIZE = 44.0f;
const float GameObjectManager::INVENTORY_MARGIN = 44.0f;

const float GameObjectManager::SELECTED_CUBE_ICON_SIZE = 244.0f;
const float GameObjectManager::SELECTED_CUBE_MARGIN = 50.0f;

void GameObjectManager::initializeLoadingScreen()
{
    loadingScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./loading.bmp"));

    loadingScreenMesh = CreateRect2D(
        { Environment::width / 2.f, Environment::height / 2.f },
        { Environment::width / 2.f, Environment::height / 2.f },
        0.5f
    );
    loadingScreenMeshMutable.AssignFrom(loadingScreenMesh);
    loadingScreenMeshMutable.RefreshVBO();
}

void GameObjectManager::initialize() {

    initializeLoadingScreen();

    std::function<bool()> loadingFunction1 = [this]()
        {

            current_room_index = 0;
            objects_in_inventory = 0;
	bearName = "";
  current_room_index = 0;
  objects_in_inventory = 0;

            //coneTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./conus.bmp"));

            // Load models
            /*
            colorCubeMesh = CreateCube3D(5.0);
            colorCubeMeshMutable.data = CreateCube3D(5.0);
            colorCubeMeshMutable.RefreshVBO();
            */
            return true;
        };


    loadingThread = std::thread([this]() {
        
        preloadedRoomMeshArr.resize(2);

        preloadedRoomMeshArr[0] = ZL::LoadFromTextFile("./oneroom001.txt");
        preloadedRoomMeshArr[0].Scale(10);
        preloadedRoomMeshArr[0].Move(Vector3f{ 0, 93, 0 });

        preloadedRoomMeshArr[1] = ZL::LoadFromTextFile("./secondroom001.txt");
        preloadedRoomMeshArr[1].Scale(10);
        preloadedRoomMeshArr[1].Move(Vector3f{ 0, 93, 0 });



        //violaIdleModel.LoadFromFile("./idleviola001.txt");
        violaIdleModel.LoadFromFile("./idleviola008.txt");

        violaWalkModel.LoadFromFile("./walkviola008.txt");
        sideThreadLoadingCompleted = true;
    });

    std::function<bool()> loadingFunction2 = [this]()
        {
            return sideThreadLoadingCompleted;
        };

    std::function<bool()> loadingFunction3 = [this]()
        {

            // Create active object
            ActiveObject cubeForFirstRoomT;
            cubeForFirstRoomT.name = "cube_T";
            cubeForFirstRoomT.activeObjectMesh = ZL::LoadFromTextFile("./cube001.txt");
            cubeForFirstRoomT.activeObjectMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundZ(M_PI * 0.5)));
            cubeForFirstRoomT.activeObjectMesh.Scale(10);
            cubeForFirstRoomT.activeObjectMeshMutable.AssignFrom(cubeForFirstRoomT.activeObjectMesh);
            cubeForFirstRoomT.activeObjectMeshMutable.RefreshVBO();
            cubeForFirstRoomT.objectPos = Vector3f{ -190, 90 , 280 };
            cubeForFirstRoomT.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Material_Base_color_1001-_2_.bmp"));
            cubeForFirstRoomT.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
            cubeForFirstRoomT.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
            cubeForFirstRoomT.activeObjectScreenMeshMutable.AssignFrom(cubeForFirstRoomT.activeObjectScreenMesh);
            cubeForFirstRoomT.activeObjectScreenMeshMutable.RefreshVBO();
            cubeForFirstRoomT.inventoryIconTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./textures/inventory_objects/cubic_T_icon.bmp32"));


            ActiveObject cubeForFirstRoomO;
            cubeForFirstRoomO.name = "cube_O";
            cubeForFirstRoomO.activeObjectMesh = ZL::LoadFromTextFile("./cube001.txt");
            cubeForFirstRoomO.activeObjectMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundZ(M_PI * 0.5)));
            cubeForFirstRoomO.activeObjectMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundX(M_PI * 1.5)));
            cubeForFirstRoomO.activeObjectMesh.Scale(10);
            cubeForFirstRoomO.activeObjectMeshMutable.AssignFrom(cubeForFirstRoomO.activeObjectMesh);
            cubeForFirstRoomO.activeObjectMeshMutable.RefreshVBO();
            cubeForFirstRoomO.objectPos = Vector3f{ 185, 90 , -365 };
            cubeForFirstRoomO.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Material_Base_color_1001-_3.bmp"));
            cubeForFirstRoomO.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
            cubeForFirstRoomO.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
            cubeForFirstRoomO.activeObjectScreenMeshMutable.AssignFrom(cubeForFirstRoomO.activeObjectScreenMesh);
            cubeForFirstRoomO.activeObjectScreenMeshMutable.RefreshVBO();
            cubeForFirstRoomO.inventoryIconTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./textures/inventory_objects/cubic_O_icon.bmp32"));
           

            ActiveObject cubeForFirstRoomM;
            cubeForFirstRoomM.name = "cube_M";
            cubeForFirstRoomM.activeObjectMesh = ZL::LoadFromTextFile("./cube001.txt");
            cubeForFirstRoomO.activeObjectMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundZ(M_PI * 0.5)));
            cubeForFirstRoomO.activeObjectMesh.RotateByMatrix(QuatToMatrix(QuatFromRotateAroundX(M_PI)));

            cubeForFirstRoomM.activeObjectMesh.Scale(10);
            cubeForFirstRoomM.activeObjectMeshMutable.AssignFrom(cubeForFirstRoomO.activeObjectMesh);
            cubeForFirstRoomM.activeObjectMeshMutable.RefreshVBO();
            cubeForFirstRoomM.objectPos = Vector3f{ 200, 95 , 230 };
            cubeForFirstRoomM.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Material_Base_color_1001_4.bmp"));
            cubeForFirstRoomM.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
            cubeForFirstRoomM.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
            cubeForFirstRoomM.activeObjectScreenMeshMutable.AssignFrom(cubeForFirstRoomO.activeObjectScreenMesh);
            cubeForFirstRoomM.activeObjectScreenMeshMutable.RefreshVBO();
            cubeForFirstRoomM.inventoryIconTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./textures/inventory_objects/cubic_M_icon.bmp32"));

            ActiveObject ao2;
            ao2.name = "book";
            ao2.activeObjectMesh = ZL::LoadFromTextFile("./book001.txt");  // Add ZL:: namespace
            ao2.activeObjectMesh.Scale(0);
            ao2.activeObjectMeshMutable.AssignFrom(ao2.activeObjectMesh);
            ao2.activeObjectMeshMutable.RefreshVBO();
            ao2.objectPos = Vector3f{ 50, 0, -300 };
            ao2.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./book03.bmp"));
            ao2.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
            ao2.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
            ao2.activeObjectScreenMeshMutable.AssignFrom(ao2.activeObjectScreenMesh);
            ao2.activeObjectScreenMeshMutable.RefreshVBO();
            ao2.inventoryIconTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./textures/inventory_objects/cubic_T_icon.bmp32"));
            


            ActiveObject carToy;
            carToy.name = "carToy";
            carToy.activeObjectMesh = ZL::LoadFromTextFile("./car.txt");  // Add ZL:: namespace
            carToy.activeObjectMesh.Scale(10);
            carToy.activeObjectMeshMutable.AssignFrom(carToy.activeObjectMesh);
            carToy.activeObjectMeshMutable.RefreshVBO();
            carToy.objectPos = Vector3f{ 50, 0, -300 };
            carToy.activeObjectTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./book03.bmp"));
            carToy.activeObjectScreenTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp24("./aoscreen01.bmp"));
            carToy.activeObjectScreenMesh = CreateRect2D({ 0.f, 0.f }, { 64.f, 64.f }, 0.5);
            carToy.activeObjectScreenMeshMutable.AssignFrom(carToy.activeObjectScreenMesh);
            carToy.activeObjectScreenMeshMutable.RefreshVBO();
            carToy.inventoryIconTexturePtr = std::make_shared<Texture>(CreateTextureDataFromBmp32("./textures/inventory_objects/cubic_T_icon.bmp32"));
            



            Room room_1;
            room_1.roomTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./Material_Base_color_1001.bmp"));
            room_1.objects.push_back(cubeForFirstRoomT);
            room_1.objects.push_back(cubeForFirstRoomO);
            room_1.objects.push_back(cubeForFirstRoomM);
            room_1.sound_name = "Symphony No.6 (1st movement).ogg";
            room_1.roomLogic = createRoom1Logic();
            room_1.textMesh = preloadedRoomMeshArr[0];
            room_1.textMeshMutable.AssignFrom(room_1.textMesh);
            room_1.collisionMgr.setRoomBoundary(790, 790);
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ 80, 0, 200 }, Vector3f{ 400, 0, 400 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -220, 0, 165 }, Vector3f{ -143, 0, 230 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -400, 0, 125 }, Vector3f{ -121, 0, 400 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -380, 0, -92 }, Vector3f{ -240, 0, 100 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -399, 0, -390 }, Vector3f{ -236, 0, -100 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -220, 0, -400 }, Vector3f{ -150, 0, -230 }));
            room_1.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ 156, 0, -400 }, Vector3f{ 380, 0, -360 }));
        
            rooms.push_back(room_1);
            //aoMgr.addActiveObject(ao1);

            Room room_2;
            room_2.roomTexture = std::make_shared<Texture>(CreateTextureDataFromBmp24("./seconroom.bmp"));
            room_2.objects.push_back(ao2);
            room_2.objects.push_back(carToy);
            room_2.sound_name = "Symphony No.6 (1st movement).ogg";
            room_2.roomLogic = createRoom2Logic();
            room_2.textMesh = preloadedRoomMeshArr[1];
            room_2.textMeshMutable.AssignFrom(room_2.textMesh);
            room_2.collisionMgr.setRoomBoundary(790, 790);
            room_2.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -227, 0, -166 }, Vector3f{ 398, 0, -154 }));
            room_2.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -328, 0, 182 }, Vector3f{ -216, 0, 332 }));
            room_2.collisionMgr.addCollider(std::make_shared<RectangleCollider>(Vector3f{ -227, 0, -400 }, Vector3f{ -208, 0, -165}));
            rooms.push_back(room_2);

            activeObjects = rooms[current_room_index].objects;

            // Initialize audio
            /*
            audioPlayer = std::make_unique<AudioPlayer>();
            if (audioPlayer) {
                audioPlayer->playMusic(rooms[current_room_index].sound_name);
            }*/
            audioPlayerAsync.resetAsync();
            audioPlayerAsync.playMusicAsync(rooms[current_room_index].sound_name);

            // Initialize inventory
            inventoryIconMesh = CreateRect2D(
                { 0.0f, 40.0f },
                { INVENTORY_ICON_SIZE / 2, INVENTORY_ICON_SIZE / 2 },
                0.5f
            );
            inventoryIconMeshMutable.AssignFrom(inventoryIconMesh);
            inventoryIconMeshMutable.RefreshVBO();

            //roomTexturePtr = rooms[current_room_index].roomTexture;


            monsterTexturePtr1 = std::make_shared<Texture>(CreateTextureDataFromBmp32("./monster001.bmp32"));
            monsterTexturePtr2 = std::make_shared<Texture>(CreateTextureDataFromBmp32("./monster002.bmp32"));


            monsterScreenMesh = CreateRect2D({ 0.f, 0.f }, { 300.f, 300.f }, 0.5);
            monsterScreenMeshMutable.AssignFrom(monsterScreenMesh);
            monsterScreenMeshMutable.RefreshVBO();


            //SDL_ShowCursor(SDL_DISABLE);
            SDL_SetRelativeMouseMode(SDL_TRUE);


            return true;

        };

        loadingFunctions.push_back(loadingFunction1);
        loadingFunctions.push_back(loadingFunction2);
        loadingFunctions.push_back(loadingFunction3);
}

void GameObjectManager::switch_room(int index){
    current_room_index = index;

    //roomTexturePtr = rooms[current_room_index].roomTexture;

    
    //audioPlayer.reset();  // This deletes the current AudioPlayer

    // Reinitialize it
    /*audioPlayer = std::make_unique<AudioPlayer>();
    if (audioPlayer) {
        audioPlayer->playMusic(rooms[current_room_index].sound_name);
    }*/
    audioPlayerAsync.stopAsync();
    audioPlayerAsync.resetAsync();
    audioPlayerAsync.playMusicAsync(rooms[current_room_index].sound_name);

    activeObjects = rooms[current_room_index].objects;

    std::cout << "Current music" << rooms[current_room_index].sound_name << std::endl;
}

void GameObjectManager::handleEvent(const SDL_Event& event) {
    // debug room switching
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT) {
//      todo comment this action
        switch_room(1);
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN) {
        const auto highlightedObjects = rooms[current_room_index].findByHighlighted(true);

      if (InventoryItem* item = GetItemSelected(true)) {
        std::cout << item->name << std::endl;

        if (current_room_index==0) {

            if (bearName.length() <= 3) {
              if (item->name == "cube_T"){
                    bearName += "T";
                    selectedCubes.push_back(*item);
                    gInventoryMap.erase(item->name);
                    objects_in_inventory--;
                }
              else if (item->name == "cube_O"){
                    bearName += "O";
                    selectedCubes.push_back(*item);
                    gInventoryMap.erase(item->name);
                    objects_in_inventory--;
                }
              else if (item->name == "cube_M"){
                    bearName += "M";
                    selectedCubes.push_back(*item);
                    gInventoryMap.erase(item->name);
                    objects_in_inventory--;
              }
            }
            else if (bearName.length() >= 3 && !(bearName.compare("TOM") == 0)) {
              bearName = "";
              for (const auto& cube : selectedCubes) {
                gInventoryMap[cube.name] = cube;
            }
              selectedCubes.clear();
            }
        }
      }
      else {
          const auto highlightedObjects = rooms[current_room_index].findByHighlighted(true);

        for (auto* ao : highlightedObjects) {
            if (!ao) {
                continue;
            }

            AddItemToInventory(ao->name, ao->inventoryIconTexturePtr, objects_in_inventory+1);
            objects_in_inventory++;

            rooms[current_room_index].removeByPtr(ao);
            activeObjects = rooms[current_room_index].objects;


            //aoMgr.removeByName(ao->name);
        }
        // bx.Interpolate(animationCounter);
        // animationCounter += 2;
       }
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
        if (Environment::zoom > 4) {
            Environment::zoom = 4;
        }
    }
    else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                Environment::showMouse = !Environment::showMouse;

                if (Environment::showMouse) {
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }
                else {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    lastMouseX = 0;
                    lastMouseY = 0;
                }
                break;

            case SDLK_ESCAPE:
            case SDLK_q:
                if (loadingThread.joinable())
                {
                    loadingThread.join();
                }
                audioPlayerAsync.exit();
                Environment::exitGameLoop = true;
                break;
            case SDLK_LEFT:
            case SDLK_a:
                Environment::leftPressed = true;
                audioPlayerAsync.playSoundAsync("walk.ogg"); // Заменено
                if (Environment::violaCurrentAnimation == 0) {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = true;
                audioPlayerAsync.playSoundAsync("walk.ogg"); // Заменено
                if (Environment::violaCurrentAnimation == 0) {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = true;
                audioPlayerAsync.playSoundAsync("walk.ogg"); // Заменено
                if (Environment::violaCurrentAnimation == 0) {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = true;
                audioPlayerAsync.playSoundAsync("walk.ogg"); // Заменено
                if (Environment::violaCurrentAnimation == 0) {
                    Environment::violaCurrentAnimation = 1;
                    Environment::violaLastWalkFrame = -1;
                }
                break;
            case SDLK_1:
            case SDLK_2:
            case SDLK_3:
            case SDLK_4:
            case SDLK_5:
            case SDLK_6:
            case SDLK_7:
            case SDLK_8:
            case SDLK_9:
                {

                UnselectAllItems();
                if (InventoryItem* item = GetItemByHotkey(event.key.keysym.sym - SDLK_1 + 1)) {
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
                if (!Environment::leftPressed && !Environment::rightPressed && !Environment::upPressed && !Environment::downPressed) {
                    if (Environment::violaCurrentAnimation == 1) {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_RIGHT:
            case SDLK_d:
                Environment::rightPressed = false;
                if (!Environment::leftPressed && !Environment::rightPressed && !Environment::upPressed && !Environment::downPressed) {
                    if (Environment::violaCurrentAnimation == 1) {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_UP:
            case SDLK_w:
                Environment::upPressed = false;
                if (!Environment::leftPressed && !Environment::rightPressed && !Environment::upPressed && !Environment::downPressed) {
                    if (Environment::violaCurrentAnimation == 1) {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
            case SDLK_DOWN:
            case SDLK_s:
                Environment::downPressed = false;
                if (!Environment::leftPressed && !Environment::rightPressed && !Environment::upPressed && !Environment::downPressed) {
                    if (Environment::violaCurrentAnimation == 1) {
                        Environment::violaCurrentAnimation = 0;
                        Environment::violaCurrentIdleFrame = -1;
                    }
                }
                break;
        }
    }
    if (event.type == SDL_MOUSEMOTION) {
        if (!Environment::showMouse) {
            int mouseX, mouseY;
            SDL_GetRelativeMouseState(&mouseX, &mouseY);

            float diffX = 0.01f * mouseX;
            float diffY = 0.01f * mouseY;

            Environment::cameraPhi += diffX;

            if (Environment::settings_inverseVertical) {
                Environment::cameraAlpha -= diffY;
            }
            else {
                Environment::cameraAlpha += diffY;
            }
            if (Environment::cameraAlpha < 0.1 * M_PI / 2.0) {
                Environment::cameraAlpha = 0.1 * M_PI / 2.0;
            }
            else if (Environment::cameraAlpha > 0.9 * M_PI / 2.0) {
                Environment::cameraAlpha = 0.9 * M_PI / 2.0;
            }
        }
        else {
            lastMouseX = event.motion.x;
            lastMouseY = event.motion.y;
        }
    }
}

void GameObjectManager::updateScene(size_t ms) {
    const float SPEED = 0.1f;

    Vector2f directionVector = { 0.f, SPEED }; // x and z

    // Вычисляем новые координаты вектора
    float x_new = directionVector.v[0] * cos(Environment::cameraPhi) - directionVector.v[1] * sin(Environment::cameraPhi);
    float y_new = directionVector.v[0] * sin(Environment::cameraPhi) + directionVector.v[1] * cos(Environment::cameraPhi);

    // Обновляем вектор
    directionVector.v[0] = x_new;
    directionVector.v[1] = y_new;

    // Only forward is allowed
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
    }
    */

    Vector3f newPosition = Environment::cameraShift;
    if (Environment::upPressed) {
        newPosition.v[0] += directionVector.v[0] * ms;
        newPosition.v[2] += directionVector.v[1] * ms;
    }
    if (Environment::downPressed) {
        newPosition.v[0] -= directionVector.v[0] * ms;
        newPosition.v[2] -= directionVector.v[1] * ms;
    }
    if (Environment::rightPressed) {
        newPosition.v[2] += directionVector.v[0] * ms;
        newPosition.v[0] -= directionVector.v[1] * ms;
    }
    if (Environment::leftPressed) {
        newPosition.v[2] -= directionVector.v[0] * ms;
        newPosition.v[0] += directionVector.v[1] * ms;
    }


    Vector3f characterNewPos{-newPosition.v[0], -newPosition.v[1], -newPosition.v[2]};

    std::cout << "Player position: x=" << characterNewPos.v[0]
        << ", y=" << characterNewPos.v[1]
        << ", z=" << characterNewPos.v[2] << "\r";
    std::cout.flush();

    // Заменяем проверку walkArea.isInside() на проверку через collisionMgr
    if (rooms[current_room_index].collisionMgr.checkCollision(characterNewPos) == false) {
        Environment::cameraShift = newPosition;
        Environment::characterPos = characterNewPos;
        /*
        std::cout << "Player position: x=" << characterNewPos.v[0]
                  << ", y=" << characterNewPos.v[1]
                  << ", z=" << characterNewPos.v[2] << "\r";
        std::cout.flush(); // Чтобы обновлялось в той же строке*/
    }

    for (auto& obj : rooms[current_room_index].objects)
    {
        float dist = sqrtf(
            pow(Environment::characterPos.v[0] - obj.objectPos.v[0], 2) +
            pow(Environment::characterPos.v[1] - obj.objectPos.v[1], 2) +
            pow(Environment::characterPos.v[2] - obj.objectPos.v[2], 2)
        );
        obj.highlighted = (dist < 150.f);
    }

    /*
    for (auto& [key, obj] : aoMgr.activeObjectsEntities) {
        float dist = sqrtf(
            pow(Environment::characterPos.v[0] - obj.objectPos.v[0], 2) +
            pow(Environment::characterPos.v[1] - obj.objectPos.v[1], 2) +
            pow(Environment::characterPos.v[2] - obj.objectPos.v[2], 2)
        );
        obj.highlighted = (dist < 50.f);
    }*/

    if (rooms[current_room_index].roomLogic) {
        rooms[current_room_index].roomLogic(*this, ms);
    }

    if (Environment::violaCurrentAnimation == 0) {
        Environment::violaCurrentIdleFrame += ms / 24.f;
        //Environment::violaCurrentIdleFrame = 0;

        while (Environment::violaCurrentIdleFrame >= 40) {
            Environment::violaCurrentIdleFrame -= 40;
        }

        if (int(Environment::violaCurrentIdleFrame) != Environment::violaLastIdleFrame) {
            violaIdleModel.Interpolate(int(Environment::violaCurrentIdleFrame));
            Environment::violaLastIdleFrame = int(Environment::violaCurrentIdleFrame);
        }
    }
    else if (Environment::violaCurrentAnimation == 1) {
        Environment::violaCurrentWalkFrame += ms / 24.f;

        while (Environment::violaCurrentWalkFrame >= 30) {
            Environment::violaCurrentWalkFrame -= 30;
        }

        if (int(Environment::violaCurrentWalkFrame) != Environment::violaLastWalkFrame) {
            violaWalkModel.Interpolate(int(Environment::violaCurrentWalkFrame));
            Environment::violaLastWalkFrame = int(Environment::violaCurrentWalkFrame);
        }
    }

    if (Environment::monsterState == 0)
    {
        Environment::monsterTimer += ms;

        if (Environment::monsterTimer > 500)
        {
            Environment::monsterTimer = 0;
            Environment::monsterState = 1;
        }
    }
    else
    {
        Environment::monsterTimer += ms;

        if (Environment::monsterTimer > 500)
        {
            Environment::monsterTimer = 0;
            Environment::monsterState = 0;
        }
    }

    //float Environment::monsterTimer = 0.0;
    //int Environment::monsterState = 1;
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

    Vector4f inx = { objectPos.v[0], objectPos.v[1], objectPos.v[2], 1.0f };
    Vector4f clipCoords = MultMatrixVector(projectionModelView, inx);

    float ndcX = clipCoords.v[0] / clipCoords.v[3];
    float ndcY = clipCoords.v[1] / clipCoords.v[3];

    screenX = (int)((ndcX + 1.0f) * 0.5f * screenWidth);
    screenY = (int)((1.0f + ndcY) * 0.5f * screenHeight);
}

}  // namespace ZL
