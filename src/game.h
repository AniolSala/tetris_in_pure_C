#include <stdbool.h>
#ifndef GAME_H_
#define GAME_H_

#define BOARD_WIDTH 320
#define BOARD_HEIGHT 640
#define SCREEN_WIDTH 520
#define SCREEN_HEIGHT 640

#define N_X 10
#define N_Y 20

#define CELL_WIDTH BOARD_WIDTH / N_X
#define CELL_HEIGHT BOARD_HEIGHT / N_Y

// A cell of the board can be empty or filled with one
// of the seven different tetrominos
#define EMPTY 0
#define TETR_S 1
#define TETR_Z 2
#define TETR_L 3
#define TETR_J 4
#define TETR_T 5
#define TETR_O 6
#define TETR_I 7

typedef struct
{
    int board[N_X * (N_Y + 4)]; // 3 extra columns for tetromino initiallization
    int tetromino[4];           // Keep track of the moving tetromino
    int falling_tt;             // The type of block is falling
    int shadow_tt[4];           // Positions of the shadow
    int next_tt;                // Next tetromino to fall
    int saved_tt;               // The tetromino saved
    bool saved;                 // Check if a figure has been save in this turn
    bool state;                 // Game finished or not finished
    bool pause;                 // Game paused
    long unsigned score;        // Score of the player
} game_t;

#endif
