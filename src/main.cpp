#include <cstdint>

#include "box2d/box2d.h"
#include "raylib.h"

#include "world.hpp"

int main(void) {
    constexpr static float SCR_WIDTH = 360.0f;
    constexpr static float SCR_HEIGHT = 480.0f;
    constexpr static float SCR_W_HALF = static_cast<float>(SCR_WIDTH) / 2.0f;
    constexpr static float SCR_H_HALF = static_cast<float>(SCR_HEIGHT) / 2.0f;

    SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raylib + Box2D Testing");

    b2World world(b2Vec2{ 0.0f, 10.0f});

    WorldMgr worldMgr(world);

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF, SCR_HEIGHT - 30.0f, 100.0f, 20.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF - 120.0f, SCR_HEIGHT - 100.0f, 20.0f, 80.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF + 120.0f, SCR_HEIGHT - 100.0f, 20.0f, 80.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF - 80.0f, SCR_HEIGHT - 50.0f, 100.0f, 20.0f, 30.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF + 80.0f, SCR_HEIGHT - 50.0f, 100.0f, 20.0f, -30.0f),
        WorldMgr::BodyType::STATIC
    );    

    double lastTime = GetTime();
    float spawnAt = -50.0f;

    while (!WindowShouldClose()) {
        float rndUnit = static_cast<float>(GetRandomValue(0, 10000) / 10000.0f);

        worldMgr.prune(SCR_H_HALF + 100.0f, SCR_W_HALF, SCR_H_HALF);
        DrawText(std::to_string(worldMgr.count()).c_str(), 10, 10, 20, WHITE);

        if (GetTime() - lastTime >= 0.125f) {
            lastTime = GetTime();
            worldMgr.add(
                WorldMgr::BodyInit(SCR_W_HALF + spawnAt, rndUnit * 110.0f, 15.0f, 15.0f, rndUnit * 90.0f),
                WorldMgr::BodyType::DYNAMIC
            );
            spawnAt += 10.0f;
            if (spawnAt > 50.0f) spawnAt = -50.0f;
        }

        worldMgr.update();
        worldMgr.draw();
    }

    CloseWindow();

    return 0;
}