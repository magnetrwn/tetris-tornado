#ifndef CURSOR_H_
#define CURSOR_H_

#include <sys/types.h>
#include "raylib.h"
#include "tetromino.hpp"
#include "world.hpp"

class PlayerCursor {
public:
    using TetrId = ssize_t;
    struct CursorInfo {
        TetrId tetrIdx;
        Vector2 pos;
        float deg;
    };

    PlayerCursor(const float pieceSize, const Color color)
        : PIECE_SIZE(pieceSize), heldTetr(-1), position({ 0.0f, 0.0f }), deg(0.0f), color(color) {}

    CursorInfo get() const;
    void set(const TetrId tetrIdx);
    void update(const Vector2& pos, const float degOffset = 0.0f);
    void updateColor(const Color color);
    void draw() const;

private:
    const float PIECE_SIZE;

    TetrId heldTetr;
    Vector2 position;
    float deg;
    Color color;
};

#endif