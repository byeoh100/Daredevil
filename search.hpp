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

#include "eval.hpp"
#include "game.hpp"
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace search {

struct SearchState {
    u64 node_count = 0ULL;
};

struct SearchResult {
    u32 best_move = 0;
    int evaluation = 0;
    u64 node_count = 0ULL;
};

inline constexpr int MAX_PLY = 64;

inline constexpr int INIT_ALPHA = -100000;
inline constexpr int INIT_BETA = 100000;
inline constexpr int MATE_SCORE = 32000;
inline constexpr int UNDERPROMOTION_SCORE = -1;

// MVV LVA
inline constexpr int CAPTURE_BONUS = 10000;
constexpr int score_capture(PieceType victim, PieceType attacker) {
    return CAPTURE_BONUS + (victim * 10) - attacker;
}

std::array<int, 256> score_moves(const GameBoard& game_board,
                                 const MoveList& move_list);
int quiescence(const GameBoard& game_board, SearchState& state, int alpha,
               int beta);
int nega_max(const GameBoard& game_board, SearchState& state, int alpha,
             int beta, int depth, int ply);
SearchResult search_best(const GameBoard& game_board, int depth);

}  // namespace search