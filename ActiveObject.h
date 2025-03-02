#pragma once
#include "TextureManager.h"
#include "Math.h"
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

    ZL::Vector3f objectPos;
    bool highlighted = false;
};

}