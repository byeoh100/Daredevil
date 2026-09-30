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
    int score = 0;
    u64 node_count = 0ULL;
};

inline constexpr int INIT_ALPHA = -100000;
inline constexpr int INIT_BETA = 100000;
inline constexpr int MATE_SCORE = 32000;

// MVV LVA
// Indices K, Q, R, B, N, P
// Index [Victim][Attacker]
inline constexpr std::array<std::array<int, 6>, 6> capture_bonuses = {
};

inline constexpr int CAPTURE_OFFSET = 10000;
constexpr int score_capture(PieceType victim, PieceType attacker) {
    return CAPTURE_OFFSET + (victim * 10) - attacker;
}

int nega_max(const GameBoard& game_board, SearchState& state, int alpha, int beta, int depth, int ply);

SearchResult search_best(const GameBoard& game_board, int depth);

}  // namespace search