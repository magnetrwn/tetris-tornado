# Tetris Tornado

**A game for the Lazy Dev 2024 Summer Jam.**

## Quick Start

If you want to build for your platform, do the following:

+ `git clone https://github.com/magnetrwn/Tetris-tornado`
+ `cd Tetris-tornado`
+ `./build.sh <platform> && build/main` (Linux)

Where `<platform>` is one of the following:

+ `windows`
+ `linux`

## Gameplay

You start with a small base to drop tetrominoes on. During the game, there will be random gusts of wind, indicated by red arrows to prepare the player before they pick up speed. The player can then prepare for the pieces to be moved in the direction of the wind and adjust their strategy accordingly.

The main objective of the game is to make as big of a tower as possible. The game is **endless**, with the score being the maximum number of pieces that have been kept for a certain amount of time (score rises only once blocks stop blinking, and if the total amount goes above the displayed value).

## Controls

+ Move the mouse to handle the placement cursor.
+ `LMB` to place the piece.
+ `RMB` to rotate the piece.

If the tetromino has been placed, there will be a short delay before the next piece is allowed to be spawned, indicated by the cursor turning red.

## Dependencies

+ [Raylib](https://www.raylib.com/), tested with v5.0-1
+ [Box2D](https://box2d.org/), tested with v2.4.1-2

If you were looking for the cross-compilation toolchain, it's [this one](https://github.com/mstorsjo/llvm-mingw). This project expands from a template wrapper I made around Raylib and Box2D, including the correct static libraries to link against **on Windows**, that were built using the toolchain. If you are interested in where the compiled libraries come from please tell me.

**Note:** on Linux the libraries are linked dynamically, so you will need to have them installed on your system! This way the final executable is smaller and easier to distribute.

## Bugs

+ You can place pieces inside of each other.
+ There's a bunch more.

## License

This simple game is licensed under the BSD 3-Clause License. See the [LICENSE](LICENSE) file for more information.
