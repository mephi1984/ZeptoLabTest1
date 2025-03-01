#pragma once
#include <SDL2/SDL.h>

namespace ZL {
    class InputManager {
    public:
        static InputManager& getInstance();
        void processInput();
        bool isKeyPressed(SDL_Scancode key) const;
        bool shouldQuit() const { return mShouldQuit; }

    private:
        InputManager() = default;
        bool mKeys[SDL_NUM_SCANCODES] = {false};
        bool mShouldQuit = false;
    };
}
