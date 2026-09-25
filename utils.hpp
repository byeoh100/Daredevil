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

namespace utils {
void print_bitboard(bitboard board);

Piece char_to_piece(char c);

std::string square_to_algebraic(square sq);
square algebraic_to_square(const std::string& algebraic);

constexpr void pop_bit(bitboard& board, square sq) { board &= ~(1ULL << sq); }

constexpr square pop_lsb(bitboard& board) {
    square index = std::countr_zero(board);
    board &= (board - 1);
    return index;
}
}  // namespace utils
