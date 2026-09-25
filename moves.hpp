#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "types.hpp"

namespace moves {
u32 encode_move(square source, square target, Piece piece, MoveFlag flag);
Move decode_move(u32 encoded_move);
void print_move(u32 encoded_move);

constexpr square get_move_source(u32 move) {
    return static_cast<square>(move & ((1 << 6) - 1));
}
constexpr square get_move_target(u32 move) {
    return static_cast<square>((move >> 6) & ((1 << 6) - 1));
}
constexpr Piece get_move_piece(u32 move) {
    return static_cast<Piece>((move >> 12) & ((1 << 4) - 1));
}
constexpr MoveFlag get_move_flag(u32 move) {
    return static_cast<MoveFlag>((move >> 16) & ((1 << 8) - 1));
}
}  // namespace moves
