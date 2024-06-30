#include "cursor.hpp"

/* --- public --- */

PlayerCursor::CursorInfo PlayerCursor::get() const {
    return cur;
}

void PlayerCursor::update(const Vector2& pos, const float degOffset) {
    cur.pos = pos;
    cur.deg += degOffset;
    cur.deg = std::fmod(cur.deg, 360.0f);
}

void PlayerCursor::updateSize(const float sizeOffset) {
    cur.size = std::fmin(std::fmax(cur.size + sizeOffset, PIECE_MIN), PIECE_MAX);
}

void PlayerCursor::updateColor(const Color newColor) {
    color = newColor;
}

void PlayerCursor::set(const TetrId tetrIdx) {
    cur.tetrIdx = tetrIdx;
}

void PlayerCursor::draw() const {
    // NOTE: possible code duplication in WorldMgr
    const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[cur.tetrIdx];
    const Vector2 centroid = Tetromino::getCentroid(shapeArray, cur.size, cur.size);

    for (Vector2& square : Tetromino::getSquares(shapeArray, cur.size, cur.size)) {
        MathUtils::rot2D(square, centroid, cur.deg * DEG2RAD);
        square.x += cur.pos.x - centroid.x;
        square.y += cur.pos.y - centroid.y;

        DrawRectanglePro(
            { square.x, square.y, cur.size, cur.size }, 
            { cur.size / 2.0f, cur.size / 2.0f }, 
            cur.deg, 
            color
        );
    }
}