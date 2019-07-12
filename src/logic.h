#ifndef LOGIC_H_
#define LOGIC_H_

void save_falling_tt(game_t* game);
void pause_game(game_t* game);
void new_game(game_t* game);
void check_player_loose(game_t* game);
int* get_lines_made(game_t* game, int* n_lines);
void check_lines_made(game_t* game);

#endif
