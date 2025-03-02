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

    ZL::Vector3f objectPos;
    bool highlighted = false;
};

class ActiveObjectManager {
    public:
        std::unordered_map<std::string, ActiveObject> activeObjectsEntities;

        // Добавить или обновить объект в контейнере
        void addActiveObject(const ActiveObject& object) {
            activeObjectsEntities[object.name] = object;
        }

        // Найти объект по имени (возвращает указатель, nullptr — если не найден)
        ActiveObject* findByName(const std::string& name) {
            auto it = activeObjectsEntities.find(name);
            if (it != activeObjectsEntities.end()) {
                return &it->second;
            }
            return nullptr;
        }

        // Найти все объекты с нужным значением highlighted
        // (возвращает список указателей на найденные объекты)
    // ActiveObject.h




        void removeByName(const std::string& name) {
                activeObjectsEntities.erase(name);
        }
    };
}
