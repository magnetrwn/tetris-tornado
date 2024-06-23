#ifndef WARNING_H_
#define WARNING_H_

#include <vector>
#include <algorithm>
#include "raylib.h"
#include "util.hpp"

class WarningView {
public:
    enum class WarningDir {
        LEFT,
        RIGHT,
        UP
    };

    WarningView(const float width, const float height, const float duration = 0.4f)
        : WIDTH(width), HEIGHT(height), MARGIN(24.0f), DURATION(duration) {}

    void startWarning(const WarningDir dir);

    void step(const float dt);
    void draw() const;

private:
    const float WIDTH;
    const float HEIGHT;
    const float MARGIN;
    const float DURATION;

    std::vector<std::pair<WarningDir, float>> ongoing;
};

#endif