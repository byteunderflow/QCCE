cmake_minimum_required(VERSION 3.28)
project(QCCE C)

set(CMAKE_C_STANDARD 11)

add_executable(QCCE main.c
        include/types.h
        include/bitboard.h
        include/piece.h
        include/move.h
        include/position.h
        include/generation.h
        include/engine.h
        include/search.h
        include/evaluation.h
        include/interface.h
        include/memory.h
        include/assert.h
        include/error.h
        include/core.h
        source/piece.c
        source/position.c
        source/memory.c
        source/error.c
        source/assert.c
        source/core.c
        source/bitboard.c
        source/generation.c
        source/move.c
)