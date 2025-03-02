#pragma once

#include "TextureManager.h"
#include "Math.h"
#include <memory>
#include "ActiveObject.h"
#include <functional>

namespace ZL
{
struct Room{
  std::shared_ptr<ZL::Texture> roomTexture;
  std::vector<ActiveObject> objects;
  std::string sound_name;

  std::function<void(class GameObjectManager&, size_t)> roomLogic;

};
}
