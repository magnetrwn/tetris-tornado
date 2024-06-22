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

    // NOTE: use between 0.0f and 1.0f by passing a ratio of the start to the end.
    inline static float quadraticEase(const float t) {
        return -2.0f * t * t + 2.0f * t + 0.025f;
    }
};

#endif