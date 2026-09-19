#include "types.hpp"
#include "game.hpp"
#include "attacks.hpp"
#include "utils.hpp"

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
    MoveList m;
    GameBoard b;
    b.init();
    b.load_from_fen("8/8/8/8/p1P5/8/1N6/8 w - - 0 1");
    move_gen::init_all_pieces();

    move_gen::generate_moves(b, m);

    for(auto move : m.moves) {
        if(move) {
            auto decoded_move = move_gen::decode_move(move);
            move_gen::print_move(decoded_move);
        }
    }

    return 0;
}