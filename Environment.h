#pragma once
#include "Math.h"

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

    static float violaCurrentIdleFrame;
};

} // namespace ZL
