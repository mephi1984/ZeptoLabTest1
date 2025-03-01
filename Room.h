#pragma once

#include <string>
#include <vector>
#include <memory>
#include "TextureManager.h"
#include "Renderer.h"
#include "ObjLoader.h"
#include "Physics.h"

namespace ZL
{
    struct ActiveObject
    {
        std::shared_ptr<Texture> activeObjectTexturePtr;
        VertexDataStruct activeObjectMesh;
        VertexRenderStruct activeObjectMeshMutable;

        std::shared_ptr<Texture> activeObjectScreenTexturePtr;
        VertexDataStruct activeObjectScreenMesh;
        VertexRenderStruct activeObjectScreenMeshMutable;

        Vector3f objectPos;
        bool highlighted = false;
    };

    class Room
    {
    private:
        std::string name;
        std::vector<std::shared_ptr<Texture>> textures;
        VertexDataStruct roomMesh;
        VertexRenderStruct roomMeshMutable;
        std::vector<ActiveObject> objects;

    public:
        Room(const std::string& roomName) : name(roomName) {}

        // Создаем меш комнаты (куб)
        void createRoomMesh(const std::string& texturePath)
        {
            // Загружаем текстуру
            auto texture = std::make_shared<Texture>(CreateTextureDataFromBmp24(texturePath));
            addTexture(texture); // Добавляем текстуру в комнату

            // Создаём меш комнаты на основе текстуры (например, прямоугольник)
            roomMesh = CreateRect2D({0.0f, 0.0f}, {1024.0f, 1024.0f}, 1.0f);
            roomMeshMutable.AssignFrom(roomMesh);
            roomMeshMutable.RefreshVBO();
        }


        void addTexture(std::shared_ptr<Texture> texture)
        {
            textures.push_back(texture);
        }

        void addObject(const ActiveObject& obj)
        {
            objects.push_back(obj);
        }

        std::vector<ActiveObject>& getObjects() {
            return objects;  // Теперь можно изменять элементы вектора
        }


        void render(Renderer& renderer)
        {
            renderer.PushMatrix();
            renderer.LoadIdentity();

            // Рисуем комнату
            if (!textures.empty())
            {
                glBindTexture(GL_TEXTURE_2D, textures[0]->getTexID());
            }
            renderer.DrawVertexRenderStruct(roomMeshMutable);

            // Рисуем объекты в комнате
            for (const auto& obj : objects)
            {
                renderer.PushMatrix();
                renderer.TranslateMatrix(obj.objectPos);
                glBindTexture(GL_TEXTURE_2D, obj.activeObjectTexturePtr->getTexID());
                renderer.DrawVertexRenderStruct(obj.activeObjectMeshMutable);
                renderer.PopMatrix();
            }

            renderer.PopMatrix();
        }

        std::string getName() const { return name; }
    };
}
