#ifndef CURSOR_H_
#define CURSOR_H_

#include <sys/types.h>
#include "raylib.h"
#include "tetromino.hpp"
#include "world.hpp"

class PlayerCursor {
public:
    using TetrId = ssize_t;

    PlayerCursor()
        : heldTetr(-1) {}

    TetrId get() const;
    void set(const TetrId tetrIdx);
    void draw() const;

private:
    TetrId heldTetr;
    Vector2 position;
    Vector2 size;
};

#endif