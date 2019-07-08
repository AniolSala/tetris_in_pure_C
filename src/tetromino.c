#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./game.h"
#include "./tetromino.h"

void generate_new_tt(game_t* game, int x_pos, int y_pos)
{
    /* Generate a new tetromino on the position given by
    the grid coords (x_pos, y_pos) */

    int pieces[] = { 'S', 'Z', 'L', 'J', 'T', 'O', 'I' };

    // New tetromino
    game->falling_tt
        = game->next_tt;
    game->next_tt = rand() % 7 + 1;
    printf("Next piece: %c\n", pieces[game->next_tt - 1]);

    // The indexes of the board that contain a tetromino
    switch (game->falling_tt) {
    case (TETR_S): // S
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos - 1;
        game->tetromino[1] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 1) * N_X + x_pos + 0;
        game->tetromino[3] = (y_pos + 1) * N_X + x_pos + 1;
        break;
    case (TETR_Z): // Z
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos + 1;
        game->tetromino[1] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 1) * N_X + x_pos + 0;
        game->tetromino[3] = (y_pos + 1) * N_X + x_pos - 1;
        break;
    case (TETR_L): // L
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[1] = (y_pos + 1) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 0) * N_X + x_pos + 1;
        game->tetromino[3] = (y_pos + 2) * N_X + x_pos + 0;
        break;
    case (TETR_J): // J
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[1] = (y_pos + 1) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 0) * N_X + x_pos - 1;
        game->tetromino[3] = (y_pos + 2) * N_X + x_pos + 0;
        break;
    case (TETR_T): // T
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos - 1;
        game->tetromino[1] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 0) * N_X + x_pos + 1;
        game->tetromino[3] = (y_pos + 1) * N_X + x_pos + 0;
        break;
    case (TETR_O): // O
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos - 1;
        game->tetromino[1] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 1) * N_X + x_pos - 1;
        game->tetromino[3] = (y_pos + 1) * N_X + x_pos + 0;
        break;
    case (TETR_I): // I
        game->tetromino[0] = (y_pos + 0) * N_X + x_pos + 0;
        game->tetromino[1] = (y_pos + 1) * N_X + x_pos + 0;
        game->tetromino[2] = (y_pos + 2) * N_X + x_pos + 0;
        game->tetromino[3] = (y_pos + 3) * N_X + x_pos + 0;
        break;
    }
}

bool check_move(game_t* game, int* dx)
{
    /* Check if if the move is valid */
    for (int i = 0; i < 4; i++) {
        int pos_tt = game->tetromino[i];
        int next_pos_tt = game->tetromino[i] + dx[i];

        bool cond_1 = abs(next_pos_tt % N_X - pos_tt % N_X) >= 4; // Left wall
        // bool cond_2 = (next_pos_tt % N_X > pos_tt % N_X) && (dx[i] < 0);          // Left wall
        bool cond_2 = next_pos_tt < 0;                                            // Bottom
        bool cond_3 = game->board[pos_tt + dx[i]] != EMPTY && pos_tt + dx[i] > 0; // Collision

        if (cond_1 || cond_2 || cond_3) {
            return false;
        }
    }

    return true;
}

int* get_dx_rotation(game_t* game)
{
    /* Get the displacement `dx` for a rotation
    If `rot` = +1 -> clockwise rotation
       `rot` = -1 -> counterclockwise rotation
    
    For a pi / 2 rotation:
      Clockwise:
        (x', y') = (y, -x)
      Counterclockwise:
        (x', y') = (-y, x)
    */

    // Calculate a rotation using relative coordinates to the
    // 2nd piece:
    int* rotated_dx = (int*)malloc(4 * sizeof(int));

    // The rotation is easly computed in greed coords
    int x_ref = game->tetromino[1] % N_X;
    int y_ref = game->tetromino[1] / N_X;

    for (int i = 0; i < 4; i++) {
        int x_rel = game->tetromino[i] % N_X - x_ref;
        int y_rel = game->tetromino[i] / N_X - y_ref;

        // Now: new_x = + y_rel
        //      new_y = - x_rel

        // Get the position of each piece in board coordinates
        rotated_dx[i] = (y_rel - x_rel) + (-x_rel - y_rel) * N_X;
    }
    return rotated_dx;
}

void move_tt(game_t* game, int* dx)
{
    // ----- Moving ----------------------------
    bool allow_move = check_move(game, dx);
    if (allow_move) {
        // Update position
        for (int i = 0; i < 4; i++) {
            game->tetromino[i] += dx[i];
        }
    } else if (!allow_move && dx[0] == -N_X) {
        // Leave it here!
        for (int i = 0; i < 4; i++) {
            game->board[game->tetromino[i]] = game->falling_tt;
        }
        // Generete a new tetromino
        generate_new_tt(game, N_X / 2, N_Y);
    }
}

void rotate_tt(game_t* game)
{
    // Don't rotate the O
    int* dx_rotation = get_dx_rotation(game);
    if (game->falling_tt != TETR_O) {
        move_tt(game, dx_rotation);
    }

    free(dx_rotation);
    dx_rotation = NULL;
}
