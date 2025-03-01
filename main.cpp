#include "Game.h"
#include "Environment.h"

int main(int argc, char* argv[]) {
    constexpr int CONST_WIDTH = 1280;
    constexpr int CONST_HEIGHT = 720;

    ZL::Environment::width = CONST_WIDTH;
    ZL::Environment::height = CONST_HEIGHT;

    ZL::Game game;
    game.setup();

#ifdef EMSCRIPTEN
    emscripten_set_main_loop([](){ game.update(); }, 0, 1);
#else
    while (!game.shouldExit()) {
        game.update();
        SDL_Delay(2);
    }
#endif

    return 0;
}
