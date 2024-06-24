#ifndef GAME_H_
#define GAME_H_

#include <string>
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
    using BodyId = WorldMgr::BodyId;

    constexpr static float SCR_WIDTH = 600.0f;
    constexpr static float SCR_HEIGHT = 960.0f;
    constexpr static float SCR_W_HALF = static_cast<float>(SCR_WIDTH) / 2.0f;
    constexpr static float SCR_H_HALF = static_cast<float>(SCR_HEIGHT) / 2.0f;
    constexpr static float TETROMINO_SIZE = 50.0f;
    constexpr static float WIND_FORCE = 15.0f;

    WorldMgr world;
    WindState wind;
    WarningView warning;
    PlayerCursor cursor;
    StormView storm;

    constexpr static double HELP_AT_LAUNCH_IVAL = 4.0f;
    constexpr static double UPDATE_CLOUDS_IVAL = 0.1f;
    constexpr static double NEW_WAVE_IVAL = 7.5f;
    constexpr static double PLAYER_CURSOR_IVAL = 1.0f;
    constexpr static double AWAITING_EVAL_IVAL = 5.0f;

    double helpAtLaunchTimer;
    double updateCloudsTimer;
    double newWaveTimer;
    double playerCursorTimer;
    std::vector<std::pair<BodyId, double>> awaitingEvaluationTimers;

    inline bool isShowHelpTime(const float t) const {
        return t - helpAtLaunchTimer < HELP_AT_LAUNCH_IVAL;
    }

    inline bool isUpdateCloudsTime(const float t) const {
        return t - updateCloudsTimer > UPDATE_CLOUDS_IVAL;
    }

    inline bool isNewWaveTime(const float t) const {
        return t - newWaveTimer > NEW_WAVE_IVAL;
    }

    inline bool isPlayerCursorTime(const float t) const {
        return t - playerCursorTimer > PLAYER_CURSOR_IVAL;
    }

    inline bool isAwaitingEvaluationTime(const float t) const {
        return !awaitingEvaluationTimers.empty()
        and t - awaitingEvaluationTimers.front().second > AWAITING_EVAL_IVAL;
    }

    constexpr static size_t FLOOR_COUNT = 3;

    constexpr static size_t FONT_MAX = 160;
    constexpr static size_t FONT_L = 64;
    constexpr static size_t FONT_M = 48;
    constexpr static size_t FONT_S = 32;

    Font font;

    void drawCenteredText(const char* text, const float y, const size_t size, const size_t spacing, const Color color) const;

    size_t score;
    bool paused;

    void initWindow();
    void setupFloor();

    void step(const float t, const float dt);
    void draw() const;

    constexpr static float HELP_TEXT_INTERLINE = 120.0f;

    void drawHelp(const unsigned char alpha) const;
    void pauseDo();
};

#endif