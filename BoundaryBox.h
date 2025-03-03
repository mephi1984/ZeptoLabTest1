#pragma once
#include "Math.h"
#include <vector>
#include <memory>

namespace ZL {

// Базовый класс для всех коллизий
class Collidable {
public:
    virtual bool checkCollision(const Vector3f& position) const = 0;
    virtual ~Collidable() = default;
};

// Прямоугольная граница комнаты
class BoundaryBox {
public:
    BoundaryBox()
    {
    }

    BoundaryBox(float width, float height) 
        : halfWidth(width/2)
        , halfHeight(height/2) {}

    bool isInside(const Vector3f& position) const {
        return (position.v[0] >= -halfWidth && position.v[0] <= halfWidth &&
                position.v[2] >= -halfHeight && position.v[2] <= halfHeight);
    }

private:
    float halfWidth = 0;
    float halfHeight = 0;
};

// Круглая коллизия для объектов
class CircleCollider : public Collidable {
public:
    CircleCollider(const Vector3f& center, float radius)
        : center(center)
        , radius(radius) {}

    bool checkCollision(const Vector3f& position) const override {
        float dx = position.v[0] - center.v[0];
        float dz = position.v[2] - center.v[2];
        return (dx * dx + dz * dz) <= (radius * radius);
    }

    void setPosition(const Vector3f& newPos) { center = newPos; }
    void setRadius(float newRadius) { radius = newRadius; }

private:
    Vector3f center;
    float radius;
};

// Прямоугольная коллизия для объектов
class RectangleCollider : public Collidable {
public:
    RectangleCollider(const Vector3f& min, const Vector3f& max)
        : minPoint(min)
        , maxPoint(max) {}

    bool checkCollision(const Vector3f& position) const override {
        return (position.v[0] >= minPoint.v[0] && position.v[0] <= maxPoint.v[0] &&
                position.v[2] >= minPoint.v[2] && position.v[2] <= maxPoint.v[2]);
    }

private:
    Vector3f minPoint;
    Vector3f maxPoint;
};

// Менеджер коллизий
class CollisionManager {
public:
    CollisionManager()
    {

    }

    void setRoomBoundary(float width, float height) {
        roomBoundary = BoundaryBox(width, height);
    }

    void addCollider(std::shared_ptr<Collidable> collider) {
        colliders.push_back(collider);
    }

    bool checkCollision(const Vector3f& position) const {
        // Проверяем границы комнаты
        if (!roomBoundary.isInside(position)) {
            return true;
        }

        // Проверяем коллизии с объектами
        for (const auto& collider : colliders) {
            if (collider->checkCollision(position)) {
                return true;
            }
        }
        return false;
    }

private:
    BoundaryBox roomBoundary;
    std::vector<std::shared_ptr<Collidable>> colliders;
};

} // namespace ZL
