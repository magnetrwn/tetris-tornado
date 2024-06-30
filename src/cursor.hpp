#ifndef CURSOR_H_
#define CURSOR_H_

#include <sys/types.h>
#include "raylib.h"
#include "tetromino.hpp"
#include "world.hpp"

class PlayerCursor {
public:
    using TetrId = Tetromino::TetrominoId;
    
    struct CursorInfo {
        TetrId tetrIdx;
        Vector2 pos;
        float deg;
        float size;
    };

    PlayerCursor(const float pieceMin, const float pieceMax, const Color color)
        : PIECE_MIN(pieceMin), PIECE_MAX(pieceMax), cur({-1, { 0.0f, 0.0f }, 0.0f, ((pieceMin + pieceMax) / 2.0f)}), color(color) {}

    CursorInfo get() const;
    void set(const TetrId tetrIdx);
    void update(const Vector2& pos, const float degOffset = 0.0f);
    void updateSize(const float sizeOffset);
    void updateColor(const Color color);
    void draw() const;

private:
    const float PIECE_MIN;
    const float PIECE_MAX;
    
    CursorInfo cur;
    Color color;
};

#endif