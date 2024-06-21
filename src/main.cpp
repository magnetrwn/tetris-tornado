#include <cstddef>

#include "box2d/box2d.h"
#include "raylib.h"

#include "world.hpp"
#include "effects.hpp"

int main(void) {
    constexpr static float SCR_WIDTH = 600.0f;
    constexpr static float SCR_HEIGHT = 960.0f;
    constexpr static float SCR_W_HALF = static_cast<float>(SCR_WIDTH) / 2.0f;
    constexpr static float SCR_H_HALF = static_cast<float>(SCR_HEIGHT) / 2.0f;

    #ifdef FPS
        SetTargetFPS(FPS);
    #else
        SetTargetFPS(60);
    #endif

    #ifdef VSYNC
        SetConfigFlags(FLAG_VSYNC_HINT);
    #endif

    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raylib + Box2D Testing");

    b2World world(b2Vec2{ 0.0f, 10.0f});

    WorldMgr worldMgr(world);

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF, SCR_HEIGHT - 30.0f, 500.0f, 20.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF - 120.0f, SCR_HEIGHT - 500.0f, 20.0f, 80.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF + 120.0f, SCR_HEIGHT - 500.0f, 20.0f, 80.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF - 80.0f, SCR_HEIGHT - 250.0f, 100.0f, 20.0f, 30.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF + 80.0f, SCR_HEIGHT - 250.0f, 100.0f, 20.0f, -30.0f),
        WorldMgr::BodyType::STATIC
    );    

    double lastTime = GetTime();
    double updateCloudsTime = GetTime();
    float spawnAt = -50.0f;

    StormEffectMgr storm(SCR_WIDTH, SCR_HEIGHT, 24, 80);

    while (!WindowShouldClose()) {
        BeginDrawing();

            float rndUnit = static_cast<float>(GetRandomValue(0, 10000) / 10000.0f);

            ClearBackground({ 53, 53, 53, 255 });

            worldMgr.prune(SCR_H_HALF + 64.0f, SCR_W_HALF, SCR_H_HALF);    

            if (GetTime() - lastTime >= 1.0f) {
                lastTime = GetTime();
                worldMgr.add(
                    WorldMgr::BodyInit(SCR_W_HALF + spawnAt, rndUnit * 110.0f, 25.0f, 25.0f, rndUnit * 90.0f),
                    WorldMgr::BodyType::DYNAMIC,
                    0
                );
                spawnAt += 10.0f;
                if (spawnAt > 50.0f) spawnAt = -50.0f;
            }

            worldMgr.update(GetFrameTime());
            worldMgr.draw();

            if (GetTime() - updateCloudsTime >= 0.1f) {
                updateCloudsTime = GetTime();
                storm.updateClouds();
            }

            storm.updateDroplets(GetFrameTime(), 2.0f);
            storm.draw();
            
            DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 30, RED);
            DrawText(std::to_string(worldMgr.count()).c_str(), 10, 52, 30, RED);  

        EndDrawing();
    }

    CloseWindow();

    return 0;
}