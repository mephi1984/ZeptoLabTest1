#pragma once
#include <functional>

namespace ZL {
    class GameObjectManager;

    std::function<void(GameObjectManager&, size_t)> createRoom1Logic();

}
