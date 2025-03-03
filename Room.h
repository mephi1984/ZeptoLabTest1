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
      objects.reserve(30);
  }

  std::vector<const ActiveObject*> findByHighlighted(bool highlighted) const {
      std::vector<const ActiveObject*> result;

      for (auto& o : objects)
      {
          if (o.highlighted == highlighted) {
              result.push_back(&o);
          }
      }
      return result;
  }

  void removeByPtr(const ActiveObject* ptr) {

      for (int i = 0; i < objects.size(); i++)
      {
          if (ptr == &objects[i])
          {
              objects.erase(objects.begin() + i);
              return;
          }
      }
  }

};
}
