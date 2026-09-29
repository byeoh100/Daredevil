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

#include "types.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "eval.hpp"
#include "movegen.hpp"

namespace search {

int nega_max(const GameBoard& game_board, int depth);

u32 search_best(const GameBoard& game_board, int depth);

}   // namespace search