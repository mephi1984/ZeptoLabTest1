#include "Game.h"
#include "Environment.h"
ZL::Game game;
void MainLoop() {
    game.update();
}
int main(int argc, char* argv[]) {
    constexpr int CONST_WIDTH = 1280;
    constexpr int CONST_HEIGHT = 720;

    ZL::Environment::width = CONST_WIDTH;
    ZL::Environment::height = CONST_HEIGHT;


#ifdef EMSCRIPTEN
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

    SDL_Window* win = SDL_CreateWindow("Jumping Bird",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CONST_WIDTH, CONST_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (!win) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(win);
    if (!glContext) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Привязка контекста к окну — важно!
    SDL_GL_MakeCurrent(win, glContext);

    ZL::Environment::window = win;
#else
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
        SDL_Log("SDL init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    ZL::Environment::window = SDL_CreateWindow(
        "Jumping Bird",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CONST_WIDTH, CONST_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );

    SDL_GLContext ctx = SDL_GL_CreateContext(ZL::Environment::window);
    SDL_GL_MakeCurrent(ZL::Environment::window, ctx);
#endif

    game.setup();

#ifdef EMSCRIPTEN   
    emscripten_set_main_loop(MainLoop, 0, 1);
#else
    while (!game.shouldExit()) {
        game.update();
        SDL_Delay(2);
    }
#endif

    return 0;
}
