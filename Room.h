#pragma once

#include "TextureManager.h"
#include "Math.h"
#include <memory>
#include "ActiveObject.h"
namespace ZL
{
struct Room{
  std::shared_ptr<ZL::Texture> roomTexture;
  std::vector<ActiveObject> objects;
};
}
