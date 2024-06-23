#include "warning.hpp"

/* --- public --- */

void WarningView::startWarning(const WarningDir dir) {
    ongoing.push_back({ dir, 0.0f });
}

void WarningView::step(const float dt) {
    ongoing.erase(std::remove_if(ongoing.begin(), ongoing.end(), [this, dt] (std::pair<WarningDir, float>& warn) {
        warn.second += dt;
        return warn.second > DURATION;
    }), ongoing.end());
}

void WarningView::draw() const {
    for (const std::pair<WarningDir, float>& warn : ongoing) {
        const unsigned char alpha = static_cast<unsigned char>(255 * MathUtils::quadraticEase(warn.second / DURATION));

        switch (warn.first) {
            case WarningDir::LEFT:
                DrawTriangle(
                    { MARGIN, MARGIN },
                    { MARGIN, HEIGHT - MARGIN },
                    { WIDTH / 4.0f, HEIGHT / 2.0f },
                    { 255, 0, 0, alpha }
                );
                break;
            case WarningDir::RIGHT:
                DrawTriangle(
                    { WIDTH - WIDTH / 4.0f, HEIGHT / 2.0f },
                    { WIDTH - MARGIN, HEIGHT - MARGIN },
                    { WIDTH - MARGIN, MARGIN },
                    { 255, 0, 0, alpha }
                );
                break;
            case WarningDir::UP:
                DrawTriangle(
                    { WIDTH / 2.0f, HEIGHT / 8.0f },
                    { WIDTH - MARGIN, MARGIN },
                    { MARGIN, MARGIN },
                    { 255, 0, 0, alpha }
                );
                break;
        }
    }
}