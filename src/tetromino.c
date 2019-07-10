#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "./game.h"
#include "./logic.h"
#include "./tetromino.h"

int** rel_pieces_pos(int tt_type)
{
    /* Here we define the (initial) shape of the tetrominos */

    int** pos = (int**)malloc(sizeof(int*) * 4);
    if (!pos) {
        printf("Error in rel_pieces_pos\n");
    }
    for (int i = 0; i < 4; i++) {
        pos[i] = (int*)malloc(2 * sizeof(int));
        if (!pos[i]) {
            printf("Error in rel_pieces_pos\n");
        }
    }

    switch (tt_type) {
    case TETR_S:
        pos[0][0] = 0;
        pos[0][1] = 0;

        pos[1][0] = -1;
        pos[1][1] = 0;

        pos[2][0] = 0;
        pos[2][1] = 1;

        pos[3][0] = 1;
        pos[3][1] = 1;
        break;
    case TETR_Z:
        pos[0][0] = 0;
        pos[0][1] = 0;

        pos[1][0] = 1;
        pos[1][1] = 0;

        pos[2][0] = 0;
        pos[2][1] = 1;

        pos[3][0] = -1;
        pos[3][1] = 1;
        break;
    case TETR_L:
        pos[0][0] = 0;
        pos[0][1] = 1;

        pos[1][0] = 0;
        pos[1][1] = 0;

        pos[2][0] = 1;
        pos[2][1] = 0;

        pos[3][0] = 0;
        pos[3][1] = 2;
        break;
    case TETR_J:
        pos[0][0] = 0;
        pos[0][1] = 1;

        pos[1][0] = 0;
        pos[1][1] = 0;

        pos[2][0] = -1;
        pos[2][1] = 0;

        pos[3][0] = 0;
        pos[3][1] = 2;
        break;
    case TETR_T:
        pos[0][0] = 0;
        pos[0][1] = 0;

        pos[1][0] = -1;
        pos[1][1] = 0;

        pos[2][0] = 1;
        pos[2][1] = 0;

        pos[3][0] = 0;
        pos[3][1] = 1;
        break;
    case TETR_O:
        pos[0][0] = 0;
        pos[0][1] = 0;

        pos[1][0] = -1;
        pos[1][1] = 0;

        pos[2][0] = -1;
        pos[2][1] = 1;

        pos[3][0] = 0;
        pos[3][1] = 1;
        break;
    case TETR_I:
        pos[0][0] = 0;
        pos[0][1] = 1;

        pos[1][0] = 0;
        pos[1][1] = 0;

        pos[2][0] = 0;
        pos[2][1] = 2;

        pos[3][0] = 0;
        pos[3][1] = 3;
        break;
    }

    return pos;
}

void generate_new_tt(game_t* game, int x_pos, int y_pos)
{
    /* Generate a new tetromino on the position given by
    the grid coords (x_pos, y_pos) */

    // New tetromino
    game->falling_tt = game->next_tt;
    game->next_tt = rand() % 7 + 1;

    // Initialize its position at the bottom of the screen
    int** rel_pos = rel_pieces_pos(game->falling_tt);
    for (int i = 0; i < 4; i++) {
        game->tetromino[i] = (x_pos + rel_pos[i][0]) + (y_pos + rel_pos[i][1]) * N_X;
    }
    update_shadow_coords(game);

    // Free the pointer
    for (int i = 0; i < 4; i++) {
        free(rel_pos[i]);
        rel_pos[i] = NULL;
    }
    free(rel_pos);
    rel_pos = NULL;
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
    // piece tetromino[0]:
    int* rotated_dx = (int*)malloc(4 * sizeof(int));

    // The rotation is easly computed in greed coords
    int x_ref = game->tetromino[0] % N_X;
    int y_ref = game->tetromino[0] / N_X;

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

void hard_move(game_t* game)
{
    // Extra points for a hard move!
    game->score += game->tetromino[0] / N_X - game->shadow_tt[0] / N_X;

    // Move the tetromino to the lowest possible spot
    for (int i = 0; i < 4; i++) {
        game->tetromino[i] = game->shadow_tt[i];
    }
}

void update_shadow_coords(game_t* game)
{
    int max_dx = 0;
    while (check_move(game, (int[]) { -max_dx, -max_dx, -max_dx, -max_dx })) {
        max_dx += N_X;
    }
    max_dx -= N_X;
    for (int i = 0; i < 4; i++) {
        game->shadow_tt[i] = game->tetromino[i] - max_dx;
    }
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
        check_line_made(game);
        game->saved = false; // Falling block can be saved again
        generate_new_tt(game, N_X / 2, N_Y);
    }
    update_shadow_coords(game);
}

void rotate_tt(game_t* game)
{
    // Don't rotate the O
    if (game->falling_tt != TETR_O) {
        int* dx_rotation = get_dx_rotation(game);
        move_tt(game, dx_rotation);
        free(dx_rotation);
        dx_rotation = NULL;
    }
}
