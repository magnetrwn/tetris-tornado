#ifndef WIND_H_
#define WIND_H_

#include <cmath>
#include "util.hpp"

class WindState {
public:
    WindState(const float min = -8.0f, const float max = 8.0f, const float wind = 0.0f, const float step = 0.09f)
        : MINWIND(min), MAXWIND(max), currentWind(wind), targetWind(wind), targetStep(step), isChanging(false) {}

    void setTargetWind(const float wind);
    float getWind() const;

    void step(const float dt);

private:
    const float MINWIND;
    const float MAXWIND;

    float currentWind;
    float targetWind;
    float targetStep;
    bool isChanging;

    float diff;
    float start;
};

#endif