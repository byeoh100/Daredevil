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

// rewrite some earlier logic with this
constexpr void pop_bit(u64& bitboard, Square sq) {
    bitboard &= ~(1ULL << sq);
}

constexpr int pop_lsb(u64& bitboard) {
    int index = std::countr_zero(bitboard);
    bitboard &= (bitboard - 1);
    return index;
}