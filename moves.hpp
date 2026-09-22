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
#include <algorithm>

std::uint32_t encode_move(int source, int target, Piece piece, MoveFlag flag);
Move decode_move(std::uint32_t encoded_move);
void print_move(std::uint32_t encoded_move);

constexpr Square get_move_source(std::uint32_t move) {
    return static_cast<Square>(move & ((1 << 6) - 1));
}
constexpr Square get_move_target(std::uint32_t move) {
    return static_cast<Square>((move >> 6) & ((1 << 6) - 1));
}
constexpr Piece get_move_piece(std::uint32_t move) {
    return static_cast<Piece>((move >> 12) & ((1 << 4) - 1));
}
constexpr MoveFlag get_move_flag(std::uint32_t move) {
    return static_cast<MoveFlag>((move >> 16) & ((1 << 8) - 1));
}