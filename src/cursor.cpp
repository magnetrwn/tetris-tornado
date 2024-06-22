#include "cursor.hpp"

/* --- public --- */

PlayerCursor::TetrId PlayerCursor::get() const {
    return heldTetr;
}

void PlayerCursor::set(const TetrId tetrIdx) {
    heldTetr = tetrIdx;
}

/*void PlayerCursor::draw() const {
    const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[heldTetr];
    
    const Vector2 centroid = { 
        position.x / WorldMgr::UNIT, 
        position.y / WorldMgr::UNIT
    };

    for (size_t i = 0; i < Tetromino::TETROMINO_DEFW; ++i)
        for (size_t j = 0; j < Tetromino::TETROMINO_DEFH; ++j)
            if (shapeArray[j * Tetromino::TETROMINO_DEFW + i]) {
                Vector2 sqOffset = {
                    position.x / WorldMgr::UNIT + i * entry.second.details.w,
                    position.y / WorldMgr::UNIT + j * entry.second.details.h
                };

                MathUtils::rot2D(sqOffset, centroid, 0.0f);

                Rectangle rec = {
                    sqOffset.x,
                    sqOffset.y,
                    entry.second.details.w,
                    entry.second.details.h
                };

                DrawRectanglePro(rec, { entry.second.details.w / 2.0f, entry.second.details.h / 2.0f }, deg, WHITE);
                DrawCircle(centroid.x, centroid.y, 3.0f, RED);
                DrawCircle(rec.x, rec.y, 3.0f, BLUE);
            }
}*/