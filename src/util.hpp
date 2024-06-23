#ifndef UTIL_H_
#define UTIL_H_

#include <cmath>
#include <random>
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

    inline static float quadraticEase(const float t) {
        return -2.0f * t * t + 2.0f * t + 0.025f;
    }

    // NOTE: i know it's not the fastest, but it works
    inline static float randf(float lbound, float ubound) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(lbound, ubound);
        return dist(gen);
    }

    inline static int randi(int lbound, int ubound) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(lbound, ubound);
        return dist(gen);
    }
};

#endif