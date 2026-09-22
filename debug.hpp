#pragma once

#include "types.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "moves.hpp"
#include "attacks.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <random>
#include <chrono>

u64 perft_driver(GameBoard game_board, int depth);
void perft_test(GameBoard game_board, int depth);
void perft_print(std::uint32_t encoded_move, u64 nodes);