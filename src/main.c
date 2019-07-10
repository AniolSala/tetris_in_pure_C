#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "./game.h"
#include "./logic.h"
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
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("Tetris",
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

    // Init the game
    game_t game;
    new_game(&game);

    // Manage the window
    float t_0, dt, max_dt = .5;
    int levels_increased = 0;
    int threshold = 1500;

    SDL_Event e;
    t_0 = clock();
    while (game.state) {
        dt = max_dt;
        if (game.score / threshold > (unsigned long)levels_increased && max_dt >= 0) {
            levels_increased += game.score / threshold;
            // Let's make 10 levels
            max_dt -= .5 / 10;
        }

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            // Quit
            case SDL_QUIT:
                game.state = 0;
                break;
            // Keys
            case SDL_KEYDOWN:
                // key down
                if (!game.pause) {

                    if (e.key.keysym.scancode == SDL_SCANCODE_UP) {
                        // Hard drop
                        hard_move(&game);
                        t_0 = clock(); // Let move one position
                        dt = .25;
                    }
                    // key up
                    if (e.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                        rotate_tt(&game);
                    }
                    if (e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
                        dt = 0;
                    }
                    // key left
                    if (e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
                        move_tt(&game, (int[]) { -1, -1, -1, -1 });
                    }
                    // key right
                    if (e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
                        move_tt(&game, (int[]) { 1, 1, 1, 1 });
                    }
                    // key S
                    if (e.key.keysym.scancode == SDL_SCANCODE_S) {
                        save_falling_tt(&game);
                    }
                }
                // key `space`
                if (e.key.keysym.scancode == SDL_SCANCODE_P) {
                    pause_game(&game);
                }
            }
        }
        // Move the tetromino every dt seconds
        if (!game.pause && (float)((float)clock() - t_0) / CLOCKS_PER_SEC >= dt) {
            move_tt(&game, (int[]) { -N_X, -N_X, -N_X, -N_X });
            t_0 = clock();

            // Check for the game status
            check_player_loose(&game);
        }
        // Render the game
        SDL_SetRenderDrawColor(renderer, 250, 255, 255, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
    }

    // Finish the window
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    printf("\n\nProgram terminated with exit (%d, %s)\n", argc, *argv);
    return EXIT_SUCCESS;
}
