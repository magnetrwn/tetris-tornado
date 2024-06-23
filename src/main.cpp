#include "game.hpp"

int main(void) {
    Game game;
    game.loop();
    return 0;
}

/*
#include <sys/types.h>

#include "box2d/box2d.h"
#include "raylib.h"

#include "world.hpp"
#include "storm.hpp"
#include "wind.hpp"
#include "warning.hpp"
#include "cursor.hpp"

int main(void) {
    constexpr static float SCR_WIDTH = 600.0f;
    constexpr static float SCR_HEIGHT = 960.0f;
    constexpr static float SCR_W_HALF = static_cast<float>(SCR_WIDTH) / 2.0f;
    constexpr static float SCR_H_HALF = static_cast<float>(SCR_HEIGHT) / 2.0f;
    constexpr static float TETROMINO_SIZE = 50.0f;

    #ifdef FPS
        SetTargetFPS(FPS);
    #else
        SetTargetFPS(60);
    #endif

    #ifdef VSYNC
        SetConfigFlags(FLAG_VSYNC_HINT);
    #endif

    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Raylib + Box2D Testing");

    WorldMgr worldMgr(b2Vec2{ 0.0f, 10.0f});

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF, SCR_HEIGHT - 30.0f, 200.0f, 20.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF + 100.0f, SCR_HEIGHT - 50.0f, 20.0f, 60.0f),
        WorldMgr::BodyType::STATIC
    );

    worldMgr.add(
        WorldMgr::BodyInit(SCR_W_HALF - 100.0f, SCR_HEIGHT - 50.0f, 20.0f, 60.0f),
        WorldMgr::BodyType::STATIC
    );

    double updateCloudsTime = GetTime();
    double changeWindTime = GetTime();
    WindState wind;
    double addWarningTime = GetTime();
    WarningView warning(SCR_WIDTH, SCR_HEIGHT);
    PlayerCursor cursor(TETROMINO_SIZE, { 192, 216, 255, 127 });
    float playerCursorTime = GetTime();
    StormView storm(SCR_WIDTH, SCR_HEIGHT, 24, 80);

    cursor.set(GetRandomValue(0, Tetromino::TETROMINO_COUNT - 1));

    while (!WindowShouldClose()) {
        BeginDrawing();

            float rndUnit = static_cast<float>(GetRandomValue(0, 10000) / 10000.0f);

            ClearBackground({ 53, 53, 53, 255 });

            worldMgr.prune(SCR_H_HALF + 275.0f, SCR_W_HALF, SCR_H_HALF);    
            worldMgr.update(GetFrameTime());
            worldMgr.draw();

            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                cursor.update(GetMousePosition(), -6.0f);
            else
                cursor.update(GetMousePosition());

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and GetTime() - playerCursorTime >= 1.0f) {
                playerCursorTime = GetTime();
                const PlayerCursor::CursorInfo info = cursor.get();

                if (info.tetrIdx != -1) {
                    worldMgr.add(
                        WorldMgr::BodyInit(
                            info.pos.x, 
                            info.pos.y, 
                            TETROMINO_SIZE, 
                            TETROMINO_SIZE,
                            info.deg
                        ),
                        WorldMgr::BodyType::DYNAMIC,
                        info.tetrIdx,
                        { 103, 164, 249, static_cast<unsigned char>(GetRandomValue(192, 255)) }
                    );

                    cursor.set(GetRandomValue(0, Tetromino::TETROMINO_COUNT - 1));
                }
            }

            if (GetTime() - playerCursorTime < 1.0f)
                cursor.updateColor({ 255, 0, 0, 127 });
            else
                cursor.updateColor({ 192, 216, 255, 127 });

            if (GetTime() - updateCloudsTime >= 0.1f) {
                updateCloudsTime = GetTime();
                storm.updateClouds();
            }

            if (GetTime() - changeWindTime >= 3.0f) {
                changeWindTime = GetTime();
                wind.setTargetWind(rndUnit * 16.0f - 8.0f);
            }

            if (GetTime() - addWarningTime >= 1.0f) {
                addWarningTime = GetTime();
                warning.startWarning(static_cast<WarningView::WarningDir>(GetRandomValue(0, 2)));
            }

            wind.step(GetFrameTime());
            warning.step(GetFrameTime());
            storm.updateDroplets(GetFrameTime(), wind.getWind());
            storm.draw();
            cursor.draw();
            warning.draw();
            
            DrawText(std::to_string(GetFPS()).c_str(), 10, 10, 30, RED);
            DrawText(std::to_string(worldMgr.count()).c_str(), 10, 52, 30, RED); 
            DrawText(std::to_string(wind.getWind()).c_str(), 10, 94, 30, RED);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}*/