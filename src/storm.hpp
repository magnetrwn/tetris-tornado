#ifndef STORM_H_
#define STORM_H_

#include <cmath>
#include <sys/types.h>
#include <vector>
#include "raylib.h"

class StormView { // NOTE: includes the wind effect, but WindState is not included
public:
    struct Cloud {
        std::vector<Vector2> components;
        Vector2 position;
    };

    StormView(const float width, const float height, const size_t clouds, const size_t droplets);

    void updateClouds();
    void updateDroplets(const float dt, const float wind = 0.0f);
    void draw() const;

private:
    const float WIDTH;
    const float HEIGHT;

    const size_t NCLOUDS;
    const size_t NCLOUDCIRCLES;
    const float CLOUDRADIUS;
    std::vector<Cloud> clouds;

    const size_t NDROPLETS;
    std::vector<Vector2> droplets;

    const float WIND_EFFECT_X;
    const float WIND_EFFECT_Y;

    void initClouds();
    void initDroplets();
    void drawMetaball(const Cloud& cloud) const;
    void drawDroplet(const Vector2& droplet) const;
};

#endif