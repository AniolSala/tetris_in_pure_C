#include "./game.h"

#ifndef TETROMINO_H_
#define TETROMINO_H_

int** rel_pieces_pos(int tt_type);
void generate_new_tt(game_t* game, int x_pos, int y_pos);
void update_shadow_coords(game_t* game);
void hard_move(game_t* game);
void move_tt(game_t* game, int* dx);
void rotate_tt(game_t* game);

#endif
