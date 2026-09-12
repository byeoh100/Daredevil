#pragma once

#include "types.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>

void print_bitboard(u64 bitboard);

Piece char_to_piece(const char& c);

// refactor some masks with this
constexpr u64 shift(u64 board, const Direction& dir) {
    switch(dir) {
        case NORTH:
            return board << 8;
            break;
        case EAST:
            return board << 1;
            break;
        case SOUTH:
            return board >> 8;
            break;
        case WEST:
            return board >> 1;
            break;
        case NORTHEAST:
            return board << 9;
            break;
        case SOUTHEAST:
            return board >> 7;
            break;
        case SOUTHWEST:
            return board >> 9;
            break;
        case NORTHWEST:
            return board << 7;
            break;
        default:
            return board;
            break;
    }
}