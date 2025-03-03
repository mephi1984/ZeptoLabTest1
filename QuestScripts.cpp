#include "QuestScripts.h"
#include "GameObjectManager.h"
#include "Inventory.h"
#include <iostream>
#include <chrono>
#include <thread>


namespace ZL
{

    std::function<void(GameObjectManager&, size_t)> createRoom1Logic()
    {
        return [elapsedTime = 0u](GameObjectManager& gom, size_t ms) mutable
        {
            if (gom.bearName.compare("TOM") == 0) {
                elapsedTime += ms;
                if (elapsedTime >= 2000) {
                    gInventoryMap.clear();
                    gom.switch_room(1);
                }
            } else if (gom.selectedCubes.size() >= 3 && gom.bearName.compare("TOM") != 0) {
              std::cout << "Else" << std::endl;
                elapsedTime += ms;
                if (elapsedTime >= 2000) {
                    gom.bearName = "";
                    for (const auto& cube : gom.selectedCubes) {
                        gInventoryMap[cube.name] = cube;
                    }
                    gom.selectedCubes.clear();
                    elapsedTime = 0;
                }
            }
        };
    }


    std::function<void(GameObjectManager&, size_t)> createRoom2Logic()
    {
        return [](GameObjectManager& gom, size_t ms)
//      Simple test logic
        {

        };
    }


    std::function<void(GameObjectManager&, size_t)> createRoom3Logic()
    {
        return [](GameObjectManager& gom, size_t ms)
//      Simple test logic
        {

        };
    }

}
