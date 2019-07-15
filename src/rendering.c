#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#include "./game.h"
#include "./logic.h"
#include "./rendering.h"
#include "./tetromino.h"

// Bg colors:
const SDL_Color BG_COLOR = { .r = 255, .g = 255, .b = 255, .a = 255 };
const SDL_Color GRID_COLOR = { .r = 100, .g = 100, .b = 150, .a = 255 };
const SDL_Color SP_COLOR = { .r = 255, .g = 255, .b = 255, .a = 255 };
const SDL_Color BLACK = { .r = 0, .g = 0, .b = 0, .a = 0 };
// Tetrominos color
const SDL_Color S_COLOR = { .r = 0, .g = 255, .b = 0, .a = 255 };
const SDL_Color Z_COLOR = { .r = 255, .g = 0, .b = 0, .a = 255 };
const SDL_Color L_COLOR = { .r = 255, .g = 153, .b = 0, .a = 255 };
const SDL_Color J_COLOR = { .r = 0, .g = 0, .b = 255, .a = 255 };
const SDL_Color T_COLOR = { .r = 204, .g = 51, .b = 255, .a = 255 };
const SDL_Color O_COLOR = { .r = 255, .g = 255, .b = 0, .a = 255 };
const SDL_Color I_COLOR = { .r = 0, .g = 255, .b = 255, .a = 255 };
const SDL_Color SHADOW_COLOR = { .r = 200, .g = 200, .b = 200, .a = 80 };

// Titles
char title_next_tt[] = "Next block";
char title_saved_tt[] = "Stored block";
char score[] = "Score";

// Font
TTF_Font* Sans = NULL;

// Music and sounds
Mix_Music* gMusic = NULL;
Mix_Chunk* gMoveRL = NULL;
Mix_Chunk* gMoveRLFail = NULL;
Mix_Chunk* gRotate = NULL;
Mix_Chunk* gRotateFail = NULL;
Mix_Chunk* gSoftDrop = NULL;
Mix_Chunk* gHardDrop = NULL;
Mix_Chunk* gSimpleLine = NULL;
Mix_Chunk* gDoubleLine = NULL;
Mix_Chunk* gTripleLine = NULL;
Mix_Chunk* gTetris = NULL;
Mix_Chunk* gSavePiece = NULL;
Mix_Chunk* gPause = NULL;

void playSound(int sound_type)
{
    switch (sound_type) {
    case MOVE_RL:
        Mix_PlayChannel(-1, gMoveRL, 0);
        break;
    case MOVE_RL_FAIL:
        Mix_PlayChannel(-1, gMoveRLFail, 0);
        break;
    case ROTATE:
        Mix_PlayChannel(-1, gRotate, 0);
        break;
    case ROTATE_FAIL:
        Mix_PlayChannel(-1, gRotateFail, 0);
        break;
    case SOFT_DROP:
        Mix_PlayChannel(-1, gSoftDrop, 0);
        break;
    case HARD_DROP:
        Mix_PlayChannel(-1, gHardDrop, 0);
        break;
    case SIMPLE_LINE:
        Mix_PlayChannel(-1, gSimpleLine, 0);
        break;
    case DOUBLE_LINE:
        Mix_PlayChannel(-1, gSimpleLine, 0);
        break;
    case TRIPLE_LINE:
        Mix_PlayChannel(-1, gTripleLine, 0);
        break;
    case TETRIS:
        Mix_PlayChannel(-1, gTetris, 0);
        break;
    case SAVE_PIECE:
        Mix_PlayChannel(-1, gSavePiece, 0);
        break;
    case LEVEL_UP:
        Mix_PlayChannel(-1, gSavePiece, 0);
        break;
    case PAUSE:
        Mix_PlayChannel(-1, gSavePiece, 0);
        break;
    }
}

void playMusic()
{
    // Play music
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(gMusic, -1);
    } else {
    }
}

void pauseMusic()
{
    // Pause music
    if (Mix_PlayingMusic() == 1) {
        Mix_PausedMusic();
    } else {
        Mix_ResumeMusic();
    }
}

bool load_files()
{
    // Load font
    Sans = TTF_OpenFont("../src/media/dejavu/DejaVuSerif-BoldItalic.ttf", 40);
    if (!Sans) {
        printf("TTF_Font Error %s\n", TTF_GetError());
        return false;
    }

    // Load music and sounds
    gMusic = Mix_LoadMUS("../src/media/music_effects/tetris_theme.mp3");
    gMoveRL = Mix_LoadWAV("../src/media/music_effects/SFX_PieceMoveLR.wav");
    gMoveRLFail = Mix_LoadWAV("../src/media/music_effects/SFX_PieceTouchLR.wav");
    gRotate = Mix_LoadWAV("../src/media/music_effects/SFX_PieceRotateLR.wav");
    gRotateFail = Mix_LoadWAV("../src/media/music_effects/SFX_PieceRotateFail.wav");
    gSoftDrop = Mix_LoadWAV("../src/media/music_effects/SFX_PieceSoftDrop.wav");
    gHardDrop = Mix_LoadWAV("../src/media/music_effects/SFX_PieceHardDrop.wav");
    gSimpleLine = Mix_LoadWAV("../src/media/music_effects/SFX_SpecialLineClearSingle.wav");
    gDoubleLine = Mix_LoadWAV("../src/media/music_effects/SFX_SpecialLineClearDouble.wav");
    gTripleLine = Mix_LoadWAV("../src/media/music_effects/SFX_SpecialLineClearTriple.wav");
    gTetris = Mix_LoadWAV("../src/media/music_effects/SFX_SpecialTetris.wav");
    // gSavePiece = NULL;
    // gPause = NULL;

    if (!gMusic || !gMoveRL || !gMoveRLFail || !gRotate || !gRotateFail || !gSoftDrop || !gHardDrop || !gSimpleLine || !gDoubleLine || !gTetris) {
        // if (!gTetris) {
        char cwd[200];
        getcwd(cwd, sizeof(cwd));
        printf("Working directory: %s\n", cwd);
        printf("Failed to load media file! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    // Load sounds

    return true;
}

void close_files()
{
    TTF_CloseFont(Sans);
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
}

void render_lines_made(SDL_Renderer* renderer, game_t* game, int* lines, int n_lines)
{
    int t_0 = clock();
    for (int i = 0; i < N_X; i++) {
        int pos_x = CELL_WIDTH * (i % N_X);
        for (int j = 0; j < n_lines; j++) {
            int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (j / N_X);
            // Change the color
            boxRGBA(renderer,
                pos_x, pos_y,                            // Top left
                pos_x + CELL_WIDTH, pos_y + CELL_HEIGHT, // Top right
                255,
                255,
                255,
                255);

            // Remove the line
            while ((float)(clock() - t_0) / CLOCKS_PER_SEC > .01) {
                game->board[lines[j] + i] = game->board[lines[j] + N_X + i];
                break;
            }
            t_0 = clock();
        }
    }
}

const SDL_Color get_piece_color(int tt_type)
{
    SDL_Color piece_color;

    switch (tt_type) {
    case (TETR_S):
        piece_color = S_COLOR;
        break;
    case (TETR_Z):
        piece_color = Z_COLOR;
        break;
    case (TETR_L):
        piece_color = L_COLOR;
        break;
    case (TETR_J):
        piece_color = J_COLOR;
        break;
    case (TETR_T):
        piece_color = T_COLOR;
        break;
    case (TETR_O):
        piece_color = O_COLOR;
        break;
    case (TETR_I):
        piece_color = I_COLOR;
        break;
    };

    return piece_color;
}

void render_grid(SDL_Renderer* renderer, const SDL_Color* color)
{
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, 255);

    // Grid color
    for (int i = 1; i <= N_X; ++i) {
        for (int j = 1; j <= N_Y; ++j) {
            SDL_RenderDrawLine(renderer,
                i * CELL_WIDTH, 0,
                i * CELL_WIDTH, BOARD_HEIGHT);
            SDL_RenderDrawLine(renderer,
                0, j * CELL_HEIGHT,
                BOARD_WIDTH, j * CELL_HEIGHT);
        }
    }
}

void render_piece(SDL_Renderer* renderer, int pos_x, int pos_y, const SDL_Color* color)
{
    int red_size = 1;
    roundedBoxRGBA(renderer,
        pos_x, pos_y,                            // Top left
        pos_x + CELL_WIDTH, pos_y + CELL_HEIGHT, // Top right
        3,
        color->r,
        color->g,
        color->b,
        color->a);
    roundedRectangleRGBA(renderer,
        pos_x + red_size, pos_y + red_size,                            // Top left
        pos_x + CELL_WIDTH - red_size, pos_y + CELL_HEIGHT - red_size, // Top right
        3,
        GRID_COLOR.r,
        GRID_COLOR.g,
        GRID_COLOR.b,
        GRID_COLOR.a);
}

void render_shadow(SDL_Renderer* renderer, game_t* game)
{
    for (int i = 0; i < 4; i++) {
        int pos_x = CELL_WIDTH * (game->shadow_tt[i] % N_X);
        int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (game->shadow_tt[i] / N_X);
        render_piece(renderer, pos_x, pos_y, &SHADOW_COLOR);
    }
}

void render_text(SDL_Renderer* renderer, int pos_x, int pos_y, int w, int h, char* msg)
{
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, msg, BLACK);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = pos_x;
    Message_rect.y = pos_y;

    Message_rect.w = w;
    Message_rect.h = h;

    SDL_QueryTexture(Message, NULL, NULL, &pos_x, &pos_y);
    SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

    // Deallocation
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

void render_moving_tt(SDL_Renderer* renderer, game_t* game)
{
    SDL_Color falling_tt_color = get_piece_color(game->falling_tt);

    // Draw the four pieces of the tetromino
    for (int i = 0; i < 4; i++) {
        int pos_x = CELL_WIDTH * (game->tetromino[i] % N_X);
        int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (game->tetromino[i] / N_X);
        render_piece(renderer, pos_x, pos_y, &falling_tt_color);
    }
}

void render_board(SDL_Renderer* renderer, game_t* game)
{
    // Render the board with the tetrominos
    SDL_Color piece_color;
    int spot;

    for (int i = 0; i <= N_X * N_Y; i++) {
        // Check for non-emtpy spots
        if ((spot = game->board[i]) != EMPTY) {
            piece_color = get_piece_color(spot);
            int pos_x = CELL_WIDTH * (i % N_X);
            int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (i / N_X);
            render_piece(renderer, pos_x, pos_y, &piece_color);
        }
    }

    // Render the grid and the falling tetromino
    render_grid(renderer, &GRID_COLOR);
    render_moving_tt(renderer, game);
    render_shadow(renderer, game);
}

void render_squares_sp(SDL_Renderer* renderer, int origin_x, int origin_y, char* text)
{
    //
    render_text(renderer, origin_x + 30, origin_y - 30, 100, 20, text);
    roundedRectangleRGBA(renderer,
        origin_x, origin_y,                                    // Top left
        origin_x + 5 * CELL_WIDTH, origin_y + 5 * CELL_HEIGHT, // Top right
        20,
        GRID_COLOR.r,
        GRID_COLOR.g,
        GRID_COLOR.b,
        GRID_COLOR.a);
}

void render_tt_sp(SDL_Renderer* renderer, int origin_x, int origin_y, int type)
{
    int** pos = rel_pieces_pos(type);

    SDL_Color color = get_piece_color(type);
    int dx = 0;
    int dy = 0;
    switch (type) {
    case TETR_O:
        dx = CELL_WIDTH / 2;
        dy = -CELL_HEIGHT / 2;
        break;
    case TETR_I:
        dy = CELL_HEIGHT / 2;
        break;
    case TETR_L:
        dx = -CELL_HEIGHT / 2;
        break;
    case TETR_J:
        dx = CELL_HEIGHT / 2;
        break;
    }

    for (int i = 0; i < 4; i++) {
        render_piece(renderer,
            origin_x + CELL_WIDTH * (2 + pos[i][0]) + dx,  // x_piece_sp + CELL_WIDTH,
            origin_y + CELL_HEIGHT * (3 - pos[i][1]) + dy, //y_piece_sp - CELL_HEIGHT,
            &color);
    }
    for (int i = 0; i < 4; i++) {
        free(pos[i]);
        pos[i] = NULL;
    }
    free(pos);
    pos = NULL;
}

char* board_score_to_char(game_t* game)
{
    int size = 32;
    char* buffer = (char*)malloc(32 * sizeof(char));
    long unsigned ret = snprintf(buffer, size, "%lu", game->score);

    if (ret >= sizeof(buffer)) {
        printf("Resize the buffer\n");
    }
    return buffer;
}

void render_side_panel(SDL_Renderer* renderer, game_t* game)
{
    int origin_x = SCREEN_WIDTH - 175;
    int origin_y = 60;

    // Render the next tetromino box
    render_squares_sp(renderer, origin_x, origin_y, title_next_tt);
    render_squares_sp(renderer, origin_x, origin_y + 230, title_saved_tt);
    render_text(renderer, origin_x + 60, origin_y + 450, 40, 20, score);

    // Render pieces
    render_tt_sp(renderer, origin_x, origin_y, game->next_tt);
    render_tt_sp(renderer, origin_x, origin_y + 230, game->saved_tt);

    // Render score
    render_text(renderer, origin_x + 70, origin_y + 490, 35, 35, board_score_to_char(game));
}

void render_game(SDL_Renderer* renderer, game_t* game)
{
    // check_line_made(game);
    render_board(renderer, game);
    render_side_panel(renderer, game);
}
