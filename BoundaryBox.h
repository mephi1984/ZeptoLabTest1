#pragma once
#include "Math.h"

namespace ZL {
class BoundaryBox {
public:
    BoundaryBox(float width, float height) 
        : halfWidth(width/2)
        , halfHeight(height/2) {}

    bool isInside(const Vector3f& position) const {
        return (position.v[0] >= -halfWidth && position.v[0] <= halfWidth &&
                position.v[2] >= -halfHeight && position.v[2] <= halfHeight);
    }

private:
    float halfWidth;
    float halfHeight;
};
}
