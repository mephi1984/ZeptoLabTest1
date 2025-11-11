#include "GameWorld.h"
#include "InputManager.h"

namespace ZL {
    GameWorld& GameWorld::getInstance() {
        static GameWorld instance;
        return instance;
    }

    void GameWorld::update(float deltaTime) {
        auto& input = InputManager::getInstance();
        
        // Update game state based on input
        if (input.isKeyPressed(SDL_SCANCODE_W)) {
            // Move forward
        }
        // ... handle other game logic ...
    }

    void GameWorld::addObject(const Vector3f& position) {
        mGameObjects.push_back(position);
    }
}
