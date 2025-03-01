#include "Environment.h"

namespace ZL {

int Environment::windowHeaderHeight = 0;
int Environment::width = 0;
int Environment::height = 0;
float Environment::zoom = 10.0f;

bool Environment::leftPressed = false;
bool Environment::rightPressed = false;
bool Environment::upPressed = false;
bool Environment::downPressed = false;

Vector3f Environment::cameraShift = {0, 0, 0};
Vector3f Environment::characterPos = {0, 0, 0};

} // namespace ZL
