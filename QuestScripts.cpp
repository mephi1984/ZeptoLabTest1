#include "QuestScripts.h"
#include "GameObjectManager.h"
#include "Inventory.h"
#include <iostream>

namespace ZL
{

    std::function<void(GameObjectManager&, size_t)> createRoom1Logic()
    {
        return [](GameObjectManager& gom, size_t ms)
        {

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
