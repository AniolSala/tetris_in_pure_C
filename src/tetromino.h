#include "./game.h"

#ifndef TETROMINO_H_
#define TETROMINO_H_

#define MOVE_LEFT -1
#define MOVE_RIGHT 1
#define MOVE_DOWN -N_X

int** rel_pieces_pos(int tt_type);
void generate_new_tt(game_t* game, int x_pos, int y_pos);
bool check_move(game_t* game, int* dx);
void move_tt(game_t* game, int* dx);
void rotate_tt(game_t* game);

#endif
