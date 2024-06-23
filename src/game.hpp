#ifndef GAME_H_
#define GAME_H_

#include "world.hpp"
#include "storm.hpp"
#include "wind.hpp"
#include "warning.hpp"
#include "cursor.hpp"

class Game {
public:
    Game();
    ~Game();

    void loop();

private:
    constexpr static float SCR_WIDTH = 600.0f;
    constexpr static float SCR_HEIGHT = 960.0f;
    constexpr static float SCR_W_HALF = static_cast<float>(SCR_WIDTH) / 2.0f;
    constexpr static float SCR_H_HALF = static_cast<float>(SCR_HEIGHT) / 2.0f;
    constexpr static float TETROMINO_SIZE = 50.0f;

    WorldMgr world;
    WindState wind;
    WarningView warning;
    PlayerCursor cursor;
    StormView storm;

    constexpr static double UPDATE_CLOUDS_IVAL = 0.1f;
    constexpr static double NEW_WAVE_IVAL = 7.5f;
    constexpr static double PLAYER_CURSOR_IVAL = 1.0f;

    double updateCloudsTimer;
    double newWaveTimer;
    double playerCursorTimer;

    inline bool isUpdateCloudsTime(const float t) const {
        return t - updateCloudsTimer > UPDATE_CLOUDS_IVAL;
    }

    inline bool isNewWaveTime(const float t) const {
        return t - newWaveTimer > NEW_WAVE_IVAL;
    }

    inline bool isPlayerCursorTime(const float t) const {
        return t - playerCursorTimer > PLAYER_CURSOR_IVAL;
    }

    void initWindow();
    void setupFloor();

    void step(const float t, const float dt);
    void draw() const;
};

#endif