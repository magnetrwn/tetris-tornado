#include "cursor.hpp"

/* --- public --- */

PlayerCursor::CursorInfo PlayerCursor::get() const {
    return { heldTetr, position, deg };
}

void PlayerCursor::update(const Vector2& pos, const float deg) {
    position = pos;
    this->deg = deg;
}

void PlayerCursor::set(const TetrId tetrIdx, const Color color) {
    heldTetr = tetrIdx;
    this->color = color;
}

void PlayerCursor::draw() const {
    // NOTE: possible code duplication in WorldMgr
    const Tetromino::TetrominoArray& shapeArray = Tetromino::SHAPES[heldTetr];
    const Vector2 centroid = Tetromino::getCentroid(shapeArray, PIECE_SIZE, PIECE_SIZE);

    for (Vector2& square : Tetromino::getSquares(shapeArray, PIECE_SIZE, PIECE_SIZE)) {
        MathUtils::rot2D(square, centroid, deg * DEG2RAD);
        square.x += position.x - centroid.x;
        square.y += position.y - centroid.y;

        DrawRectanglePro(
            { square.x, square.y, PIECE_SIZE, PIECE_SIZE }, 
            { PIECE_SIZE / 2.0f, PIECE_SIZE / 2.0f }, 
            deg, 
            color
        );
    }
}