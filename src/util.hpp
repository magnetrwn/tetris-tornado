#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include "raylib.h"

class MathUtils {
public:
    inline static void rot2D(Vector2& point, const Vector2& origin, const float rad) {
        const float s = std::sin(rad);
        const float c = std::cos(rad);

        Vector2 result = point;

        point.x -= origin.x;
        point.y -= origin.y;

        result.x = point.x * c - point.y * s + origin.x;
        result.y = point.x * s + point.y * c + origin.y;

        point = result;
    }
};

#endif