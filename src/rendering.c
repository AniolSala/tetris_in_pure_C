#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
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
// Tetrominos color
const SDL_Color S_COLOR = { .r = 0, .g = 255, .b = 0, .a = 255 };
const SDL_Color Z_COLOR = { .r = 255, .g = 0, .b = 0, .a = 255 };
const SDL_Color L_COLOR = { .r = 255, .g = 153, .b = 0, .a = 255 };
const SDL_Color J_COLOR = { .r = 0, .g = 0, .b = 255, .a = 255 };
const SDL_Color T_COLOR = { .r = 204, .g = 51, .b = 255, .a = 255 };
const SDL_Color O_COLOR = { .r = 255, .g = 255, .b = 0, .a = 255 };
const SDL_Color I_COLOR = { .r = 0, .g = 255, .b = 255, .a = 255 };

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

void render_piece(SDL_Renderer* renderer, SDL_Color* color, int pos_x, int pos_y)
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

void render_moving_tt(SDL_Renderer* renderer, game_t* game)
{
    SDL_Color falling_tt_color = get_piece_color(game->falling_tt);

    // Draw the four pieces of the tetromino
    for (int i = 0; i < 4; i++) {
        int pos_x = CELL_WIDTH * (game->tetromino[i] % N_X);
        int pos_y = BOARD_HEIGHT - CELL_HEIGHT - CELL_HEIGHT * (game->tetromino[i] / N_X);
        render_piece(renderer, &falling_tt_color, pos_x, pos_y);
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
            render_piece(renderer, &piece_color, pos_x, pos_y);
        }
    }

    // Render the grid and the falling tetromino
    render_grid(renderer, &GRID_COLOR);
    render_moving_tt(renderer, game);
}

void render_side_panel()
{
    //
}

void render_game(SDL_Renderer* renderer, game_t* game)
{
    check_line_made(game);
    render_board(renderer, game);
    render_side_panel();
}
