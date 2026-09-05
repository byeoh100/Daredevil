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

// knight attacks
// make mask
// do for all 64
// store in array

// also maybe make a pawn push mask (>> 8 or << 8)
// should be trivial

namespace move_gen {
    u64 mask_pawn_attacks(u64 pawn_board, Color color);
    void init_pawn_attacks_arrays();
    std::array<u64, 64> get_pawn_attacks(Color color);

} // namespace move_gen