#include "wind.hpp"

void WindState::setTargetWind(const float wind) {
    targetWind = std::fmin(std::fmax(wind, MINWIND), MAXWIND);
    isChanging = true;
    diff = std::abs(currentWind - targetWind);
    start = currentWind;
}

float WindState::getWind() const {
    return currentWind;
}

void WindState::step(const float dt) {
    if (!isChanging)
        return;

    if (std::abs(currentWind - targetWind) < targetStep) {
        currentWind = targetWind;
        isChanging = false;
        return;
    }

    const float completion = std::abs(currentWind - start) / diff;
    currentWind += targetStep * MathUtils::quadraticEase(completion) * ((currentWind < targetWind) ? 1.0f : -1.0f) * dt / 0.016f;

}