#pragma once
#include "Math.h"
#include <vector>
#include <memory>

namespace ZL {
    class GameWorld {
    public:
        static GameWorld& getInstance();
        void update(float deltaTime);
        void addObject(const Vector3f& position);
        // ...other game world related methods...

    private:
        GameWorld() = default;
        std::vector<Vector3f> mGameObjects;
    };
}
