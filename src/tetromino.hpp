#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <array>
#include <vector>
#include <sys/types.h>
#include "raylib.h"

struct Tetromino {
    constexpr static size_t TETROMINO_COUNT = 7;
    constexpr static size_t TETROMINO_DEFW = 3;
    constexpr static size_t TETROMINO_DEFH = 4;
    constexpr static size_t TETROMINO_DEFGRID = TETROMINO_DEFW * TETROMINO_DEFH;

    using TetrominoId = ssize_t;
    using TetrominoArray = std::array<bool, TETROMINO_DEFGRID>;

    constexpr static const std::array<std::array<bool, TETROMINO_DEFGRID>, TETROMINO_COUNT> SHAPES = {
        TetrominoArray{ 0, 1, 0,
                                             0, 1, 0,
                                             0, 1, 0,
                                             0, 1, 0 },

        TetrominoArray{ 0, 1, 0,
                                             0, 1, 0,
                                             0, 1, 1,
                                             0, 0, 0 },

        TetrominoArray{ 0, 1, 0,
                                             0, 1, 0,
                                             1, 1, 0,
                                             0, 0, 0 },

        TetrominoArray{ 0, 1, 0,
                                             0, 1, 1,
                                             0, 1, 0,
                                             0, 0, 0 },

        TetrominoArray{ 0, 1, 1,
                                             0, 1, 1,
                                             0, 0, 0,
                                             0, 0, 0 },

        TetrominoArray{ 0, 1, 0,
                                             0, 1, 1,
                                             0, 0, 1,
                                             0, 0, 0 },

        TetrominoArray{ 0, 1, 0,
                                             1, 1, 0,
                                             1, 0, 0,
                                             0, 0, 0 },
    };

    inline static Vector2 getCentroid(const TetrominoArray& shape, const float blkWidth, const float blkHeight) {
        Vector2 centroid = { 0.0f, 0.0f };
        size_t count = 0;

        for (size_t i = 0; i < TETROMINO_DEFW; ++i)
            for (size_t j = 0; j < TETROMINO_DEFH; ++j)
                if (shape[j * TETROMINO_DEFW + i]) {
                    centroid.x += i;
                    centroid.y += j;
                    ++count;
                }

        centroid.x *= blkWidth / count / 2.0f;
        centroid.y *= blkHeight / count / 2.0f;

        return centroid;
    }

    inline static std::vector<Vector2> getSquares(const TetrominoArray& shape, const float blkWidth, const float blkHeight) {
        const Vector2 centroid = getCentroid(shape, blkWidth, blkHeight);
        std::vector<Vector2> squares;

        for (size_t i = 0; i < TETROMINO_DEFW; ++i)
            for (size_t j = 0; j < TETROMINO_DEFH; ++j)
                if (shape[j * TETROMINO_DEFW + i])
                    squares.push_back({ i * blkWidth - centroid.x, j * blkHeight - centroid.y });

        return squares;
    }
};

#endif