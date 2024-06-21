#ifndef TETROMINO_H_
#define TETROMINO_H_

#include <array>
#include <cstddef>

struct Tetromino {
    constexpr static size_t TETROMINO_COUNT = 7;
    constexpr static size_t TETROMINO_DEFW = 3;
    constexpr static size_t TETROMINO_DEFH = 4;
    constexpr static size_t TETROMINO_DEFGRID = TETROMINO_DEFW * TETROMINO_DEFH;

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
};

#endif