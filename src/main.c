#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "./game.h"
#include "./rendering.h"
#include "./tetromino.h"

int main(int argc, char* argv[])
{
    // Set the seed to pick a tetromino
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Procedural",
        100, 100,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    // ###################################
    // # Create a new game_t game ########
    // ###################################
    game_t game;
    for (int i = 0; i < N_X * (N_Y + 4); i++) {
        game.board[i] = EMPTY; // Initialize it to emtpy
    }
    game.state = 1;
    generate_new_tt(&game, N_X / 2, N_Y);
    // ###################################

    // Manage the window
    float dt, t_0 = clock();
    SDL_Event e;
    while (game.state) {
        dt = .5;

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            // Quit
            case SDL_QUIT:
                game.state = 0;
                break;
            // Keys
            case SDL_KEYDOWN:
                // key down
                if (e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                    dt = .05;
                }
                // key up
                if (e.key.keysym.scancode == SDL_SCANCODE_UP) {
                    rotate_tt(&game);
                }
                // key left
                if (e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                    move_tt(&game, (int[]) { -1, -1, -1, -1 });
                }
                // key right
                if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                    move_tt(&game, (int[]) { 1, 1, 1, 1 });
                }
            }
        }
        // Move the tetromino every dt seconds
        if ((float)((float)clock() - t_0) / CLOCKS_PER_SEC >= dt) {
            move_tt(&game, (int[]) { -N_X, -N_X, -N_X, -N_X });
            t_0 = clock();
        }
        // Render the game
        SDL_SetRenderDrawColor(renderer, 250, 255, 255, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
    }

    // Finish the window
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("\n\nProgram terminated with exit (%d, %s)\n", argc, *argv);
    return EXIT_SUCCESS;
}
