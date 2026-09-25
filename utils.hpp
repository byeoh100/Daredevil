#pragma once

#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "types.hpp"

void print_bitboard(u64 bitboard);

Piece char_to_piece(const char& c);

// rewrite some earlier logic with this
constexpr void pop_bit(u64& bitboard, Square sq) { bitboard &= ~(1ULL << sq); }

constexpr int pop_lsb(u64& bitboard) {
    int index = std::countr_zero(bitboard);
    bitboard &= (bitboard - 1);
    return index;
}