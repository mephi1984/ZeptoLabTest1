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
        return [](GameObjectManager& gom, size_t ms)
        {
            if (gom.bearName.compare("TOM") == 0) {
                gInventoryMap.clear();
                gom.objects_in_inventory = 0;
//                std::this_thread::sleep_for(std::chrono::seconds(1));
                gom.switch_room(1);
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

}
