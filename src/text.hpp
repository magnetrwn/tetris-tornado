#ifndef TEXT_H_
#define TEXT_H_

#include <vector>
#include <cmath>
#include <algorithm>
#include "raylib.h"

class MovingTextMgr {
public:
    struct TextEvent {
        const char* text;
        float x;
        float y;
        float size;
        Color color;
    };

    struct MovingTextMgrParams {
        float startX;
        float startY;
        float travelX;
        float travelY;
        float speed;
    };

    MovingTextMgr(const Font& font, const MovingTextMgrParams params) // NOTE: each text manager keeps one path
        : PARAMS(params), font(font) {}

    void add(const char* text, const float size, const Color color);
    void step(const float dt);
    void draw() const;

private:
    const MovingTextMgrParams PARAMS;
    const Font& font;

    std::vector<TextEvent> events;
};

#endif