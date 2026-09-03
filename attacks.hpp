#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>
#include <algorithm>

// pawn attacks
// make masks diagonal of left and right + up (white) or down (black) -> vector size 64
// this for SINGLE SQUARE lookup
// for ALL pawn attacks of a color, shift in BULK
// we do this with a GENERAL pawn mask that just shifts for ALL but you can apply
// to single

namespace move_gen {
    u64 mask_pawn_attacks(u64 pawn_board, Color color);
    void init_pawn_attacks_array();

} // namespace move_gen