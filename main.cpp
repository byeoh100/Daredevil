#include "types.hpp"
#include "game.hpp"
#include "attacks.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>


// NOTES
// - write a Makefile for now, but move to CMake later

// (long) each chess piece black and white | 2d -> 1d flatten 8x8 board to 64 and represent as a long
// the collection of every long consitutes the bit map

// get rank = sq / 8
// get bit = sq % 8
// (rank * 8) + file = sq

// task 1: print the starting state chess board [COMPLETE]
// task 2: implement piece logic

// dev_tool::

int main() {
    GameBoard my_board;
    my_board.reset();

    u64 pawn_masks = move_gen::mask_pawn_attacks(my_board.get_piece(Piece::WHITE_PAWN), Color::WHITE);
    print_bitboard(pawn_masks);

    return 0;
}