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

#include "attacks.hpp"
#include "game.hpp"
#include "moves.hpp"
#include "types.hpp"
#include "utils.hpp"

u64 perft_driver(GameBoard game_board, int depth);
void perft_test(GameBoard game_board, int depth);
void perft_print(std::uint32_t encoded_move, u64 nodes);