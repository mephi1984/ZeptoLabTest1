#pragma once
#include "Math.h"
#ifdef __linux__
#include <SDL2/SDL.h>
#endif
#include "OpenGlExtensions.h"

namespace ZL {

class Environment {
public:
    static int windowHeaderHeight;
    static int width;
    static int height;
    static float zoom;

    static bool leftPressed;
    static bool rightPressed;
    static bool upPressed;
    static bool downPressed;

    static Vector3f cameraShift;
    static Vector3f characterPos;
    static float cameraPhi;
    static float cameraAlpha;

    //Viola
    static float violaCurrentIdleFrame;
    static float violaCurrentWalkFrame;

    static int violaLastIdleFrame;
    static int violaLastWalkFrame;

    static int violaCurrentAnimation;

    static float violaAngleAroundY;

    static bool settings_inverseVertical;

    static SDL_Window* window;

    static float cameraDefaultVerticalShift;

    static bool showMouse;
    static bool exitGameLoop;


};

} // namespace ZL
