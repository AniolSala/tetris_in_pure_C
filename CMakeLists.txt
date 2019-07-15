cmake_minimum_required(VERSION 3.7)

project(Tetris)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(PkgConfig)
pkg_check_modules(SDL2_LIBS SDL2_gfx SDL2_mixer SDL2_ttf)
include_directories(${SDL2_LIBS_INCLUDE_DIRS})

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fdump-rtl-expand")

set(SRCS
    src/main.c
    src/logic.c
    src/rendering.c
    src/tetromino.c
    )

set(HEADERS
    src/logic.h
    src/rendering.h
    src/game.h
    src/tetromino.h
    )

add_executable(main ${SRCS} ${HEADERS})
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_LIBS_LIBRARIES})
