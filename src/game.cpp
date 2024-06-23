#include "game.hpp"

/* --- initialization --- */

Game::Game() 
    : world(b2Vec2{ 0.0f, 9.8f }),
      wind(),
      warning(SCR_WIDTH, SCR_HEIGHT),
      cursor(TETROMINO_SIZE, { 192, 216, 255, 127 }),
      storm(SCR_WIDTH, SCR_HEIGHT, 24, 80),
      score(0) {

    initWindow();
    setupFloor();

    updateCloudsTimer = GetTime();
    newWaveTimer = GetTime();
    playerCursorTimer = GetTime();

    cursor.set(MathUtils::randi(0, Tetromino::TETROMINO_COUNT - 1));

    font = LoadFontEx("./static/font/ct_prolamina.ttf", 64, nullptr, 0);
}

Game::~Game() {
    UnloadFont(font);
    CloseWindow();
}

/* --- public --- */

void Game::loop() {
    while (!WindowShouldClose()) {
        step(GetTime(), GetFrameTime());
        draw();
    }
}

/* --- private --- */

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
        cursor.update(GetMousePosition(), 4.8f);
    else
        cursor.update(GetMousePosition());

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) and isPlayerCursorTime(t)) {
        playerCursorTimer = t;

        const PlayerCursor::CursorInfo info = cursor.get();

        if (info.tetrIdx != -1) {
            const BodyId id = 
                world.add(
                    WorldMgr::BodyInit(
                        info.pos.x, 
                        info.pos.y, 
                        TETROMINO_SIZE, 
                        TETROMINO_SIZE,
                        info.deg
                    ),
                    WorldMgr::BodyType::DYNAMIC,
                    info.tetrIdx,
                    { 103, 164, 249, static_cast<unsigned char>(MathUtils::randi(192, 255)) }
                );

            cursor.set(MathUtils::randi(0, Tetromino::TETROMINO_COUNT - 1));
            awaitingEvaluationTimers.push_back({ id, 0.0f } );
        }
    }

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
}

void Game::draw() const {
    BeginDrawing();
        ClearBackground({ 53, 53, 53, 255 });
        world.draw();
        cursor.draw();
        storm.draw();
        warning.draw();

        std::string kgfText = std::to_string(wind.getWind() * WIND_FORCE * 0.1019f) + " kgf";
        std::string scoreText = std::to_string(score);
        
        DrawTextEx(
            font, 
            kgfText.c_str(), 
            { SCR_W_HALF - MeasureTextEx(font, kgfText.c_str(), 36, 10).x / 2, 12 }, 
            36, 
            10, 
            { 53, 53, 53, 127 }
        );

        DrawTextEx(
            font, 
            scoreText.c_str(), 
            { SCR_W_HALF - MeasureTextEx(font, scoreText.c_str(), 48, 4).x / 2, SCR_HEIGHT - 100 }, 
            48, 
            4, 
            WHITE
        );
    EndDrawing();
}