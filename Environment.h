#pragma once
#include "Math.h"

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

    static ZL::Vector3f cameraShift;
    static ZL::Vector3f characterPos;
};
