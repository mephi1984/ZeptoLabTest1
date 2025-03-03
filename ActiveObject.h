#pragma once
#include "TextureManager.h"
#include "Math.h"
#include <unordered_map>

#include <memory>

namespace ZL {
struct ActiveObject {
    std::string name;
    std::shared_ptr<ZL::Texture> activeObjectTexturePtr;
    ZL::VertexDataStruct activeObjectMesh;
    ZL::VertexRenderStruct activeObjectMeshMutable;

    std::shared_ptr<ZL::Texture> activeObjectScreenTexturePtr;
    ZL::VertexDataStruct activeObjectScreenMesh;
    ZL::VertexRenderStruct activeObjectScreenMeshMutable;

    std::shared_ptr<ZL::Texture> inventoryIconTexturePtr;

    ZL::Vector3f objectPos;
    bool highlighted = false;
};
}
