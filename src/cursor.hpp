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

    PlayerCursor(const float pieceSize)
        : PIECE_SIZE(pieceSize), heldTetr(-1) {}

    CursorInfo get() const;
    void set(const TetrId tetrIdx, const Color color = WHITE);
    void update(const Vector2& pos, const float deg);
    void draw() const;

private: // TODO: wait for the last drop to be asleep before allowing a new tetromino
    const float PIECE_SIZE;

    TetrId heldTetr;
    Vector2 position; // TODO: limit to certain height and above
    Vector2 size;
    float deg;
    Color color;
};

#endif