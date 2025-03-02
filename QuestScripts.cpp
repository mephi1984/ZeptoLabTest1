#include "QuestScripts.h"
#include "GameObjectManager.h"
#include "Inventory.h"
#include <iostream>

namespace ZL
{

    std::function<void(GameObjectManager&, size_t)> createRoom1Logic()
    {
        return [](GameObjectManager& gom, size_t ms)
//      Simple test logic
        {
            if (GetItemByName("book")) {
                std::cout << "[Room 1] Игрок поднял книгу!\n";

                 gom.switch_room(1);
            }
        };
    }

}
