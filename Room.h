#pragma once

#include "TextureManager.h"
#include "Math.h"
#include <memory>
#include "ActiveObject.h"

#include <functional>
#include "BoundaryBox.h" 
namespace ZL
{
struct Room{
  std::shared_ptr<ZL::Texture> roomTexture;
  std::vector<ActiveObject> objects;
  std::string sound_name;

  ZL::VertexDataStruct textMesh;
  ZL::VertexRenderStruct textMeshMutable;

  CollisionManager collisionMgr;

  std::function<void(class GameObjectManager&, size_t)> roomLogic;

  Room()
  {
  }

};
}
