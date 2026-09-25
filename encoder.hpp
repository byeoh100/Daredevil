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
#include <string_view>

#include "types.hpp"
#include "utils.hpp"

namespace encoder {
// encoding key bits:
// <---dead 8 bits---> | 16-23 | 12-15 | 6-11 | 0-5
//                       flags   piece   tgt    src
inline constexpr int SOURCE_SHIFT = 0;
inline constexpr int TARGET_SHIFT = 6;
inline constexpr int PIECE_SHIFT = 12;
inline constexpr int FLAG_SHIFT = 16;

inline constexpr u32 MASK_4B = (1 << 4) - 1;    // Piece
inline constexpr u32 MASK_6B = (1 << 6) - 1;    // Source/Target
inline constexpr u32 MASK_8B = (1 << 8) - 1;    // Flag

void print_move(u32 encoded_move);

constexpr square get_move_source(u32 move) {
    return static_cast<square>((move >> SOURCE_SHIFT) & (MASK_6B));
}
constexpr square get_move_target(u32 move) {
    return static_cast<square>((move >> TARGET_SHIFT) & (MASK_6B));
}
constexpr Piece get_move_piece(u32 move) {
    return static_cast<Piece>((move >> PIECE_SHIFT) & (MASK_4B));
}
constexpr MoveFlag get_move_flag(u32 move) {
    return static_cast<MoveFlag>((move >> FLAG_SHIFT) & (MASK_8B));
}

constexpr u32 encode_move(square source, square target, Piece piece, MoveFlag flag) {
    // insert error handling for empty move values
    u32 encoded_move = 0;
    encoded_move |= (static_cast<u32>(source)) << SOURCE_SHIFT;
    encoded_move |= (static_cast<u32>(target)) << TARGET_SHIFT;
    encoded_move |= (static_cast<u32>(piece)) << PIECE_SHIFT;
    encoded_move |= (static_cast<u32>(flag)) << FLAG_SHIFT;

    return encoded_move;
}

constexpr Move decode_move(u32 encoded_move) {
    Move decoded_move;

    decoded_move.source =
        static_cast<square>(encoded_move & (MASK_6B));
    encoded_move >>= 6;
    decoded_move.target = static_cast<square>(encoded_move & MASK_6B);
    encoded_move >>= 6;
    decoded_move.piece =
        static_cast<Piece>(encoded_move & MASK_4B);
    encoded_move >>= 4;
    decoded_move.flag =
        static_cast<MoveFlag>(encoded_move & MASK_8B);

    return decoded_move;
}
}  // namespace encoder
