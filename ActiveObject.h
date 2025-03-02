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

class ActiveObjectManager {
    public:
        // Добавить или обновить объект в контейнере
        void addActiveObject(const ActiveObject& object) {
            activeObjects[object.name] = object;
        }

        // Найти объект по имени (возвращает указатель, nullptr — если не найден)
        ActiveObject* findByName(const std::string& name) {
            auto it = activeObjects.find(name);
            if (it != activeObjects.end()) {
                return &it->second;
            }
            return nullptr;
        }

        // Найти все объекты с нужным значением highlighted
        // (возвращает список указателей на найденные объекты)
        std::vector<ActiveObject*> findByHighlighted(bool highlighted) {
            std::vector<ActiveObject*> result;
            result.reserve(activeObjects.size());
            for (auto& [key, object] : activeObjects) {
                if (object.highlighted == highlighted) {
                    result.push_back(&object);
                }
            }
            return result;
        }

    private:
        // Хранение объектов по ключу name
        std::unordered_map<std::string, ActiveObject> activeObjects;
    };

}
