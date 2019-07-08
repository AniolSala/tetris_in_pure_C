#include "./game.h"

#ifndef TETROMINO_H_
#define TETROMINO_H_

#define MOVE_LEFT -1
#define MOVE_RIGHT 1
#define MOVE_DOWN -N_X

void generate_new_tt(game_t* game, int x_pos, int y_pos);
void move_tt(game_t* game, int* dx);
void rotate_tt(game_t* game);

#endif