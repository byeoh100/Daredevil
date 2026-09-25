#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "encoder.hpp"
#include "game.hpp"
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

namespace debug {
u64 perft_driver(const GameBoard& game_board, int depth);
void perft_test(const GameBoard& game_board, int depth);
void perft_print(u32 encoded_move, u64 nodes);
}  // namespace debug