# Tetris

Implementation of Tetris game using pure C and the library SDL.

Controls:

* Left / Right keys: move left / right
* Up key: Hard drop
* Down key: Inrease speed
* Space key: Rotate
* S key: Save the current block

To build the game open the terminal in the Tetris folder and type:

> /path/to/Tetris $ cd build/

> /path/to/Tetris $ cmake ..

If no errors appeared, then type:

> /path/to/Tetris/ $ ./main

**NOTE**: To build the project you will need:

* gcc compiler
* [cmake](https://cmake.org/download/)
* [SDL2](https://www.libsdl.org/download-2.0.php) library
* [SDL2_gfx](http://www.ferzkopp.net/Software/SDL2_gfx/Docs/html/index.html),
[SDL2_mixer](https://www.libsdl.org/projects/SDL_mixer/) and
[SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

![tetris in C](./media/tetris.gif)

