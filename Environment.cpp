#include "Environment.h"

#include "RenderSystem.h"
#include "Utils.h"
#include "Inventory.h"
#include <GL/gl.h>

namespace ZL {

int Environment::windowHeaderHeight = 0;
int Environment::width = 0;
int Environment::height = 0;
float Environment::zoom = 3.f;

bool Environment::leftPressed = false;
bool Environment::rightPressed = false;
bool Environment::upPressed = false;
bool Environment::downPressed = false;

Vector3f Environment::cameraShift = {0, 0, 0};
Vector3f Environment::characterPos = {0, 0, 0};

float Environment::cameraPhi = 0.f;
float Environment::cameraAlpha = 0.3*M_PI / 2.0;

float Environment::violaCurrentIdleFrame = 0.f;
int Environment::violaLastIdleFrame = -1;
float Environment::violaCurrentWalkFrame = 0.f;
int Environment::violaLastWalkFrame = 0;

int Environment::violaCurrentAnimation = 0;
float Environment::violaAngleAroundY = 0.f;

bool Environment::settings_inverseVertical = false;

SDL_Window* Environment::window = nullptr;

float Environment::cameraDefaultVerticalShift = -150.f;
float Environment::itemDefaultVerticalShift = -80.f;
bool Environment::showMouse = false;

bool Environment::exitGameLoop = false;

bool Environment::gameIsLoading = true;

float Environment::monsterTimer = 0.0;
int Environment::monsterState = 1;

bool Environment::finalIsGood = false;
bool Environment::finalIsBad = false;

bool Environment::goToLevel3 = false;
float Environment::goTolevel3Timer = 0;


} // namespace ZL
