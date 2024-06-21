#ifndef EFFECTS_H_
#define EFFECTS_H_

#include <cmath>
#include <cstddef>
#include <vector>
#include "raylib.h"

class StormEffectMgr {
public:
    struct Cloud {
        std::vector<Vector2> components;
        Vector2 position;
    };

    StormEffectMgr(const float width, const float height, const size_t clouds, const size_t droplets);

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

    void initClouds();
    void initDroplets();
    void drawMetaball(const Cloud& cloud) const;
    void drawDroplet(const Vector2& droplet) const;
};

#endif