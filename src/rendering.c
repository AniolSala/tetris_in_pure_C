#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdbool.h>

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
char font[] = "/usr/share/fonts/truetype/dejavu/DejaVuSerif-BoldItalic.ttf";

SDL_Color get_piece_color(int tt_type)
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

void render_piece(SDL_Renderer* renderer, int pos_x, int pos_y, SDL_Color* color)
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
    SDL_Color color = SHADOW_COLOR;
    for (int i = 0; i < 4; i++) {
        int pos_x = CELL_WIDTH * (game->shadow_tt[i] % N_X);
        int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (game->shadow_tt[i] / N_X);
        render_piece(renderer, pos_x, pos_y, &color);
    }
}

void render_text(SDL_Renderer* renderer, int pos_x, int pos_y, int w, int h, char* msg)
{
    TTF_Font* Sans = TTF_OpenFont(font, 40);
    if (!Sans) {
        printf("Couldn't open the font\n");
    }
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
    TTF_CloseFont(Sans);
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
        // printf("Resize the buffer\n");
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
