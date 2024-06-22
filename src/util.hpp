#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include "raylib.h"

class MathUtils {
public:
    inline static Vector2 rot2D(Vector2 point, Vector2 origin, float rad) {
        const float s = std::sin(rad);
        const float c = std::cos(rad);

        point.x -= origin.x;
        point.y -= origin.y;

        point.x = point.x * c - point.y * s + origin.x;
        point.y = point.x * s + point.y * c + origin.y;
        
        return point;
    }
};

#endif