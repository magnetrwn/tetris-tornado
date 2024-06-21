#include "effects.hpp"

/* --- initialization --- */

StormEffectMgr::StormEffectMgr(const float width, const float height, const size_t clouds, const size_t droplets)
    : WIDTH(width), HEIGHT(height), NCLOUDS(clouds), NCLOUDCIRCLES(8), CLOUDRADIUS(60.0f), NDROPLETS(droplets) {

    
    initClouds();
    initDroplets();
}

/* --- public --- */

void StormEffectMgr::updateClouds() {
    for (Cloud& cloud : clouds)
        for (Vector2& component : cloud.components) {
            const float angle = GetRandomValue(0, 360) * DEG2RAD;
            const float dist = GetRandomValue(0, static_cast<int>(CLOUDRADIUS));
            component = { cloud.position.x + std::cos(angle) * dist, cloud.position.y + std::sin(angle) * dist };
        }
}

void StormEffectMgr::updateDroplets(const float dt, const float wind) {
    const float windEffectX = 300.0f * wind;
    const float windEffectY = 500.0f;

    for (Vector2& droplet : droplets) {
        droplet.x += windEffectX * dt;
        droplet.y += windEffectY * dt;

        if (droplet.y > HEIGHT) {
            droplet.x = static_cast<float>(GetRandomValue(0, static_cast<int>(WIDTH)));
            droplet.y = 0;
        }

        if (droplet.x < 0)
            droplet.x += WIDTH;
        else if (droplet.x > WIDTH)
            droplet.x -= WIDTH;
    }
}


void StormEffectMgr::draw() const {
    for (const Cloud& cloud : clouds)
        drawMetaball(cloud);
    for (const Vector2& droplet : droplets)
        drawDroplet(droplet);
}

/* --- private --- */

void StormEffectMgr::initClouds() {
    clouds.resize(NCLOUDS, Cloud());

    for (Cloud& cloud : clouds) {
        cloud.position = { 
            static_cast<float>(GetRandomValue(CLOUDRADIUS, static_cast<int>(WIDTH) - CLOUDRADIUS)), 
            static_cast<float>(GetRandomValue(CLOUDRADIUS / 2.0f, static_cast<int>(HEIGHT / CLOUDRADIUS / 5.0f))) 
        };

        cloud.components.resize(NCLOUDCIRCLES);
    }

    updateClouds();
}

void StormEffectMgr::initDroplets() {
    droplets.resize(NDROPLETS);

    for (Vector2& droplet : droplets) {
        droplet = { 
            static_cast<float>(GetRandomValue(0, static_cast<int>(WIDTH))), 
            static_cast<float>(GetRandomValue(0, static_cast<int>(HEIGHT))) 
        };
    }
}

void StormEffectMgr::drawMetaball(const Cloud& cloud) const {
    for (const Vector2& component : cloud.components)
        DrawCircleV(component, CLOUDRADIUS, { 255, 255, 255, 48 });
}

void StormEffectMgr::drawDroplet(const Vector2& droplet) const {
    DrawCircleV(droplet, 2.0f, WHITE);
}