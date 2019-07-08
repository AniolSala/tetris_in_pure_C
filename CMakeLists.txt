cmake_minimum_required(VERSION 3.7)

project(procedural C)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(PkgConfig)
pkg_check_modules(SDL2_GFX SDL2_gfx)
include_directories(${SDL2_GFX_INCLUDE_DIRS})

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -fdump-rtl-expand")

set(SRCS
    src/main.c
    # logic.c
    src/rendering.c
    src/tetromino.c
    )

set(HEADERS
    #     logic.h
    src/rendering.h
    src/game.h
    src/tetromino.h
    )

add_executable(main ${SRCS} ${HEADERS})
target_link_libraries(main ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARIES})
