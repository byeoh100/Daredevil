#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "debug.hpp"
#include "game.hpp"
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

// NOTES
// - write a Makefile for now, but move to CMake later

// (long) each chess piece black and white | 2d -> 1d flatten 8x8 board to 64
// and represent as a long the collection of every long consitutes the bit map

// get rank = sq / 8
// get bit = sq % 8
// (rank * 8) + file = sq

// task 1: print the starting state chess board [COMPLETE]
// task 2: implement piece logic

// dev_tool::

int main() {
    GameBoard board;
    board.init();
    board.load_from_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    movegen::init_all_pieces();

    debug::perft_test(board, 1);
    debug::perft_test(board, 2);
    debug::perft_test(board, 3);
    debug::perft_test(board, 4);
    debug::perft_test(board, 5);
    debug::perft_test(board, 6);

    return 0;
}