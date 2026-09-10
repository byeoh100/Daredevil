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
    GameBoard b;
    b.init();
    b.set_piece(WHITE_PAWN, A7);
    b.set_piece(BLACK_BISHOP, E3);
    b.set_piece(BLACK_PAWN, D6);
    b.set_piece(WHITE_ROOK, A3);

    u64 a = move_gen::mask_rook_blockers(b.get_piece(WHITE_ROOK), b.get_occupancy());

    print_bitboard(a);

    return 0;
}