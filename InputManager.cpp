#include "InputManager.h"

namespace ZL {
    InputManager& InputManager::getInstance() {
        static InputManager instance;
        return instance;
    }

    void InputManager::processInput() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                mShouldQuit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                mKeys[event.key.keysym.scancode] = true;
            }
            else if (event.type == SDL_KEYUP) {
                mKeys[event.key.keysym.scancode] = false;
            }
        }
    }

    bool InputManager::isKeyPressed(SDL_Scancode key) const {
        return mKeys[key];
    }
}
