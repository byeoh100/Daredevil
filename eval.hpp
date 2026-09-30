#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "game.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace eval {

// K, Q, R, B, N, P, k, q, r, b, n, p
// no guard against NO_PIECE
inline constexpr std::array<int, 12> piece_values = {
    10000, 900, 500, 350, 300, 100, -10000, -900, -500, -350, -300, -100};

// clang-format off
// Tables pulled directly from https://chessprogramming.org/Simplified_Evaluation_Function
// Eventually upgrade to PeSTO tables with tapered eval
inline constexpr std::array<int, 64> pawn_table = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5,-10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};

inline constexpr std::array<int, 64> knight_table = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

inline constexpr std::array<int, 64> bishop_table = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

inline constexpr std::array<int, 64> rook_table = {
      0,  0,  0,  5,  5,  0,  0,  0,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
     -5,  0,  0,  0,  0,  0,  0, -5,
      5, 10, 10, 10, 10, 10, 10,  5,
      0,  0,  0,  0,  0,  0,  0,  0
};

inline constexpr std::array<int, 64> queen_table = {
    -20,-10,-10, -5, -5,-10,-10,-20,
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

// this will be mg/eg soon, just wait for basic implementation
inline constexpr std::array<int, 64> king_table = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

// inline constexpr std::array<int, 64> eg_king_table = {
//     -50,-30,-30,-30,-30,-30,-30,-50,
//     -30,-30,  0,  0,  0,  0,-30,-30,
//     -30,-10, 20, 30, 30, 20,-10,-30,
//     -30,-10, 30, 40, 40, 30,-10,-30,
//     -30,-10, 30, 40, 40, 30,-10,-30,
//     -30,-10, 20, 30, 30, 20,-10,-30,
//     -30,-20,-10,  0,  0,-10,-20,-30,
//     -50,-40,-30,-20,-20,-30,-40,-50
// };
// clang-format on
inline constexpr std::array<std::array<int, 64>, 6> piece_tables = {
    king_table,   queen_table,  rook_table,
    bishop_table, knight_table, pawn_table};

int evaluate(const GameBoard& game_board);

}  // namespace eval