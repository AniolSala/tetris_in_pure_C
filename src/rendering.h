#ifndef RENDERING_H_
#define RENDERING_H_

#define MOVE_RL 1
#define MOVE_RL_FAIL 2
#define ROTATE 3
#define ROTATE_FAIL 4
#define SOFT_DROP 5
#define HARD_DROP 6
#define SIMPLE_LINE 7
#define DOUBLE_LINE 8
#define TRIPLE_LINE 9
#define TETRIS 10
#define SAVE_PIECE 11
#define LEVEL_UP 12
#define PAUSE 13

void render_squares_sp(SDL_Renderer* renderer, int origin_x, int origin_y, char* text);
void render_game(SDL_Renderer* renderer, game_t* game);
bool load_files();
void close_files();
void playMusic();
void pauseMusic();
void playSound(int sound_type);

#endif
