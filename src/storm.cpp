#include "storm.hpp"

/* --- initialization --- */

StormView::StormView(const float width, const float height, const size_t clouds, const size_t droplets)
    : WIDTH(width), 
      HEIGHT(height), 
      NCLOUDS(clouds), 
      NCLOUDCIRCLES(8), 
      CLOUDRADIUS(60.0f), 
      NDROPLETS(droplets),
      WIND_EFFECT_X(300.0f),
      WIND_EFFECT_Y(500.0f) {

    initClouds();
    initDroplets();
}

/* --- public --- */

void StormView::updateClouds() {
    for (Cloud& cloud : clouds)
        for (Vector2& component : cloud.components) {
            const float angle = MathUtils::randf(0.0f, 360.0f) * DEG2RAD;
            const float dist = MathUtils::randf(0.0f, CLOUDRADIUS);
            component = { cloud.position.x + std::cos(angle) * dist, cloud.position.y + std::sin(angle) * dist };
        }
}

void StormView::updateDroplets(const float dt, const float wind) {
    for (Vector2& droplet : droplets) {
        droplet.x += WIND_EFFECT_X * wind * dt;
        droplet.y += WIND_EFFECT_Y * dt;

        if (droplet.y > HEIGHT) {
            droplet.x = MathUtils::randf(0.0f, WIDTH);
            droplet.y = 0;
        }

        if (droplet.x < 0)
            droplet.x += WIDTH;
        else if (droplet.x > WIDTH)
            droplet.x -= WIDTH;
    }
}

void StormView::draw() const {
    for (const Cloud& cloud : clouds)
        drawMetaball(cloud);
    for (const Vector2& droplet : droplets)
        drawDroplet(droplet);
}

/* --- private --- */

void StormView::initClouds() {
    clouds.resize(NCLOUDS, Cloud());

    for (Cloud& cloud : clouds) {
        cloud.position = { 
            MathUtils::randf(CLOUDRADIUS, WIDTH - CLOUDRADIUS), 
            MathUtils::randf(CLOUDRADIUS / 2.0f, HEIGHT / CLOUDRADIUS / 5.0f)
        };

        cloud.components.resize(NCLOUDCIRCLES);
    }

    updateClouds();
}

void StormView::initDroplets() {
    droplets.resize(NDROPLETS);

    for (Vector2& droplet : droplets) {
        droplet = { 
            MathUtils::randf(0, WIDTH), 
            MathUtils::randf(0, HEIGHT)
        };
    }
}

void StormView::drawMetaball(const Cloud& cloud) const {
    for (const Vector2& component : cloud.components)
        DrawCircleV(component, CLOUDRADIUS, { 255, 255, 255, 48 });
}

void StormView::drawDroplet(const Vector2& droplet) const {
    DrawCircleV(droplet, 2.0f, WHITE);
}