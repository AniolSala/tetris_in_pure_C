#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./game.h"
#include "./tetromino.h"

void save_falling_tt(game_t* game)
{
    // Dont save twice in a turn
    if (game->saved) {
        return;
    }

    int tmp = game->next_tt;
    if (game->saved_tt) {
        // Swap the falling_tt with saved_tt, but leave next_tt the same
        game->next_tt = game->saved_tt;
        game->saved_tt = game->falling_tt;
        generate_new_tt(game, N_X / 2, N_Y);
        game->next_tt = tmp;
    } else {
        // If it is the first save next_tt doesnt change
        game->saved_tt = game->falling_tt;
        generate_new_tt(game, N_X / 2, N_Y);
    }

    game->saved = true;
}

void pause_game(game_t* game)
{
    game->pause = game->pause == false ? true : false;
}
void new_game(game_t* game)
{
    // Initialize the values for game
    for (int i = 0; i < (N_Y + 4) * N_X; i++) {
        game->board[i] = EMPTY;
    }
    game->next_tt = rand() % 7 + 1;
    game->saved_tt = EMPTY;
    game->saved = false;
    game->state = 1;
    game->pause = 0;
    game->score = 0;

    // Here we go
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

bool check_single_line(game_t* game, int line)
{
    int cells_filled = 0;
    for (int j = 0; j < N_X; j++) {
        if (game->board[line * N_X + j]) {
            return true;
        }
        cells_filled++;
    }
    return false;
}

void check_line_made(game_t* game)
{
    int cells_filled;
    int lines_completed = 0;

    int current_line = 0;
    while (current_line < N_Y) {
        cells_filled = 0;
        for (int j = 0; j < N_X; j++) {
            if (game->board[current_line * N_X + j] == EMPTY) {
                // printf("Line %d: Position (%d, %d) is empty\n", i, j, i);
                current_line++;
                break;
            } else {
                cells_filled++;
            }
        }
        // If line `i` completed:
        if (cells_filled == N_X) {
            lines_completed++;
            for (int k = current_line; k < N_Y; k++) {
                for (int m = 0; m < N_X; m++) {
                    game->board[k * N_X + m] = game->board[(k + 1) * N_X + m];
                }
            }
        }
    }
    switch (lines_completed) {
    case 1:
        game->score += 40;
        break;
    case 2:
        game->score += 100;
        break;
    case 3:
        game->score += 300;
        break;
    case 4:
        game->score += 1200;
        break;
    }
}
