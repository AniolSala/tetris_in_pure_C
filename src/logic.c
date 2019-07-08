#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game.h"
#include "./tetromino.h"

void new_game(game_t* game)
{
    for (int i = 0; i < (N_Y + 4) * N_X; i++) {
        game->board[i] = EMPTY;
    }
    game->falling_tt = rand() % 8;
    game->next_tt = rand() % 8;
    game->state = 1;

    generate_new_tt(game, N_X / 2, N_Y);
}

void check_player_loose(game_t* game)
{
    for (int i = 0; i < N_X; i++) {
        if (game->board[N_Y * N_X + i] != EMPTY) {
            printf("You lost!\n");
            game->state = 0;
        }
    }
}

void check_line_made(game_t* game)
{
    int cells_filled;
    for (int i = 0; i < N_Y; i++) {
        cells_filled = 0;
        for (int j = 0; j < N_X; j++) {
            if (game->board[i * N_X + j] == EMPTY) {
                break;
            } else {
                cells_filled++;
            }
        }
        // Line `i` completed!
        if (cells_filled == N_X) {
            printf("Line %d completed!", i);
            for (int k = i; k < N_Y; k++) {
                for (int m = 0; m < N_X; m++) {
                    game->board[k * N_X + m] = game->board[(k + 1) * N_X + m];
                }
            }
        }
    }
}
