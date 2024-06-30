#include "game.hpp"

/* --- initialization --- */

Game::Game() 
    : world(b2Vec2{ 0.0f, 9.8f }),
      wind(),
      warning(SCR_WIDTH, SCR_HEIGHT),
      cursor(TETROMINO_SIZE_MIN, TETROMINO_SIZE_MAX, { 192, 216, 255, 127 }),
      storm(SCR_WIDTH, SCR_HEIGHT, 24, 80),
      skipLaunchHelp(false), 
      upwardsText(font, { SCR_W_HALF, SCR_HEIGHT + FONT_L, 0.0f, -SCR_HEIGHT / 2, 0.5f }),
      score(0), paused(false) {

    initWindow();
    setupFloor();

    helpAtLaunchTimer = GetTime();
    updateCloudsTimer = GetTime();
    newWaveTimer = GetTime();
    playerCursorTimer = GetTime();

    cursor.set(MathUtils::randi(0, Tetromino::TETROMINO_COUNT - 1));

    font = LoadFontEx("./static/font/m6x11.ttf", FONT_MAX, nullptr, 0);
}

Game::~Game() {
    UnloadFont(font);
    CloseWindow();
}

/* --- public --- */

void Game::loop() {
    int fadeAlpha = 255;

    while (!WindowShouldClose()) {

        if (isShowHelpTime(GetTime())) {

            BeginDrawing();
                drawHelp(255);
            EndDrawing();

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                skipLaunchHelp = true;

        } else {

            if (!IsWindowFocused())
                paused = true;

            if (IsKeyPressed(KEY_P))
                paused = !paused;

            if (!paused)
                step(GetTime(), GetFrameTime());

            BeginDrawing();
                if (!paused)
                    draw();
                else
                    pauseDo();

                if (!isShowHelpTime(GetTime())) {
                    drawHelp(fadeAlpha);
                    fadeAlpha = std::max(0, fadeAlpha - 8);
                }
            EndDrawing();
        }
    }
}

/* --- private --- */

void Game::drawCenteredText(const char* text, const float y, const size_t size, const size_t spacing, const Color color) const {
    DrawTextEx(
        font, 
        text, 
        { SCR_W_HALF - MeasureTextEx(font, text, size, spacing).x / 2, y }, 
        size, 
        spacing, 
        color
    );
}

void Game::initWindow() {
    #ifdef FPS
        SetTargetFPS(FPS);
    #else
        SetTargetFPS(60);
    #endif

    #ifdef VSYNC
        SetConfigFlags(FLAG_VSYNC_HINT);
    #endif

    InitWindow(SCR_WIDTH, SCR_HEIGHT, "Tetris Tornado: a game by @magnetrwn");
}

void Game::setupFloor() {
    world.add(
        WorldMgr::BodyInit(SCR_W_HALF, SCR_HEIGHT - 30.0f, 200.0f, 20.0f),
        WorldMgr::BodyType::STATIC
    );

    world.add(
        WorldMgr::BodyInit(SCR_W_HALF + 100.0f, SCR_HEIGHT - 50.0f, 20.0f, 60.0f),
        WorldMgr::BodyType::STATIC
    );

    world.add(
        WorldMgr::BodyInit(SCR_W_HALF - 100.0f, SCR_HEIGHT - 50.0f, 20.0f, 60.0f),
        WorldMgr::BodyType::STATIC
    );
}

void Game::step(const float t, const float dt) {
    world.prune(SCR_H_HALF + 275.0f, SCR_W_HALF, SCR_H_HALF);
    world.applyAll(wind.getWind() * WIND_FORCE, 0.0f);
    world.update(dt);

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
        cursor.update(GetMousePosition(), SENSITIVITY);
    else
        cursor.update(GetMousePosition());

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and isPlayerCursorTime(t)) {
        playerCursorTimer = t;
        upwardsText.add("LEFT MOUSE BUTTON", FONT_M, WHITE);
        const PlayerCursor::CursorInfo info = cursor.get();

        if (info.tetrIdx != -1) {
            const BodyId id = 
                world.add(
                    WorldMgr::BodyInit(
                        info.pos.x,
                        info.pos.y,
                        info.size, 
                        info.size,
                        info.deg
                    ),
                    WorldMgr::BodyType::DYNAMIC,
                    info.tetrIdx,
                    { 103, 164, 249, static_cast<unsigned char>(MathUtils::randi(192, 255)) }
                );

            cursor.set(MathUtils::randi(0, Tetromino::TETROMINO_COUNT - 1));
            cursor.randomSize();
            awaitingEvaluationTimers.push_back({ id, 0.0f } );
        }
    }

    const float mouseWheel = GetMouseWheelMove();
    if (mouseWheel != 0.0f)
        cursor.updateSize(mouseWheel * SENSITIVITY);

    if (!isPlayerCursorTime(t))
        cursor.updateColor({ 255, 0, 0, 127 });
    else
        cursor.updateColor({ 192, 216, 255, 127 });

    if (isUpdateCloudsTime(t)) {
        updateCloudsTimer = t;
        storm.updateClouds();
    }

    if (isNewWaveTime(t)) {
        newWaveTimer = t;
        const float newWind = MathUtils::randf(-8.0f, 8.0f);
        wind.setTargetWind(newWind);
        if (newWind > 2.0f)
            warning.startWarning(WarningView::WarningDir::LEFT);
        else if (newWind < -2.0f)
            warning.startWarning(WarningView::WarningDir::RIGHT);
        else
            warning.startWarning(WarningView::WarningDir::UP);
    }

    for (size_t i = 0; i < awaitingEvaluationTimers.size(); ++i) {
        std::pair<BodyId, double>& entry = awaitingEvaluationTimers[i];

        if (!world.exists(entry.first)) {
            awaitingEvaluationTimers.erase(awaitingEvaluationTimers.begin() + i);
            continue;
        }

        entry.second += dt;

        if (entry.second > AWAITING_EVAL_IVAL) {
            world.changeColor(
                entry.first, 
                { 103, 164, 249, static_cast<unsigned char>(MathUtils::randi(192, 255)) }
            );

            awaitingEvaluationTimers.erase(awaitingEvaluationTimers.begin() + i);

            if (world.count() - awaitingEvaluationTimers.size() - FLOOR_COUNT == score + 1)
                score++;

        } else {
            world.changeColor(
                entry.first, 
                (std::fmod(entry.second, 0.25f) < 0.125f) ? WHITE : Color{ 103, 164, 249, 208 }
            );
        }
    }

    wind.step(dt);
    warning.step(dt);
    storm.updateDroplets(dt, wind.getWind());
    upwardsText.step(dt);
}

void Game::draw() const {
    ClearBackground({ 53, 53, 53, 255 });
    world.draw();
    cursor.draw();
    storm.draw();
    warning.draw();
    upwardsText.draw();

    const float w = wind.getWind();

    // TODO: all under here is inefficient, should generate the new data only when the score/wind changes
    // TODO: complexity is high, should be refactored into smaller functions

    const unsigned char alpha = static_cast<unsigned char>(std::min(127, static_cast<int>(std::abs(w) * 200.0f)));

    std::string kgfText = std::to_string(w * WIND_FORCE * 0.1019f) + " kgf";
    std::string scoreText = std::to_string(score);
    
    drawCenteredText(kgfText.c_str(), 12, FONT_S, 10, { 53, 53, 53, 127 });
    drawCenteredText(scoreText.c_str(), SCR_HEIGHT - 100, FONT_M, 4, WHITE);

    DrawRectanglePro(
        { SCR_W_HALF, 48, std::abs(w) * 50, 16 }, 
        { std::abs(w) * 25, 0 }, 
        0.0f, 
        { 53, 53, 53, alpha }
    );

    if (w < 0.0f)
        DrawTriangle(
            { SCR_W_HALF + w * 25, 48 },
            { SCR_W_HALF + w * 25 - 16, 56 },
            { SCR_W_HALF + w * 25, 64 },
            { 53, 53, 53, alpha }
        );
    else
        DrawTriangle(
            { SCR_W_HALF + w * 25 + 16, 56 },
            { SCR_W_HALF + w * 25, 48 },
            { SCR_W_HALF + w * 25, 64 },
            { 53, 53, 53, alpha }
        );
}

void Game::drawHelp(const unsigned char alpha) const {
    using AlignedYStr = std::pair<const char*, float>;

    constexpr static float TOP_TEXT_OFFSET = 90.0f;
    constexpr static std::array<AlignedYStr, 6> HELP_STRINGS = {
        AlignedYStr{ "-- Controls: --", TOP_TEXT_OFFSET },
        AlignedYStr{ "LMB >> place,", TOP_TEXT_OFFSET + FONT_S * 2.0f },
        AlignedYStr{ "RMB >> rotate,", TOP_TEXT_OFFSET + FONT_S * 3.5f },
        AlignedYStr{ "Mouse wheel >> resize,", TOP_TEXT_OFFSET + FONT_S * 5.0f },
        AlignedYStr{ "P >> pause game,", TOP_TEXT_OFFSET + FONT_S * 6.5f },
        AlignedYStr{ "The game is endless!", SCR_H_HALF + FONT_S * 2.0f },
    };

    DrawRectangle( 0, 0, SCR_WIDTH, SCR_HEIGHT, { 0, 0, 0, alpha });

    for (const std::pair<const char*, float>& str : HELP_STRINGS)
        drawCenteredText(str.first, str.second, FONT_S, 2, { 255, 255, 255, alpha });

    drawCenteredText("Get Ready!", SCR_H_HALF - FONT_L / 2.0f, FONT_L, 4, { 255, 255, 255, alpha });
    DrawTextEx(font, "Your score:", { SCR_W_HALF - 220, SCR_HEIGHT - 95 }, FONT_S, 2, { 255, 255, 255, alpha });
    drawCenteredText("0", SCR_HEIGHT - 100, FONT_M, 4, { 255, 255, 255, alpha });
    DrawTextEx(font, "LMB to skip", { SCR_WIDTH - 200, SCR_HEIGHT - 50 }, FONT_S, 2, { 255, 255, 255, alpha });
}

void Game::pauseDo() {
    storm.updateDroplets(GetFrameTime(), wind.getWind());
    draw();
    drawCenteredText("PAUSED", SCR_H_HALF - FONT_L / 2.0f, FONT_L, 4, WHITE);
}