#include "text.hpp"

/* --- public --- */

void MovingTextMgr::add(const char* text, const float size, const Color color) {
    events.push_back({ text, PARAMS.startX, PARAMS.startY, size, color });
}

void MovingTextMgr::step(const float dt) {
    for (TextEvent& event : events) {
        event.x += PARAMS.travelX * PARAMS.speed * dt;
        event.y += PARAMS.travelY * PARAMS.speed * dt;
    }

    events.erase(std::remove_if(events.begin(), events.end(), [this] (const TextEvent& event) {
        const float dx = event.x - PARAMS.startX;
        const float dy = event.y - PARAMS.startY;
        const float maxDistX = std::abs(PARAMS.travelX);
        const float maxDistY = std::abs(PARAMS.travelY);

        return (dx * dx > maxDistX * maxDistX) or (dy * dy > maxDistY * maxDistY);
    }), events.end());
}

void MovingTextMgr::draw() const {
    for (const TextEvent& event : events) {
        DrawTextEx(
            font, 
            event.text, 
            { event.x - MeasureTextEx(font, event.text, event.size, 2).x / 2, event.y }, 
            event.size, 
            2, 
            event.color
        );
    }
}