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
#include <random>

// also maybe make a pawn push mask (>> 8 or << 8)
// should be trivial

// next we need to do magic bitboards
// generate the perfect hash
// perfect hash diag (bishop) / horiz + vert (rook)

namespace move_gen {
    u64 mask_pawn_attacks(u64 pawn_board, Color color);
    u64 mask_pawn_quiets();
    void init_pawn_attacks_array();
    u64 get_pawn_attack(Square idx, Color color);

    u64 mask_knight_attacks(u64 knight_board);
    void init_knight_attacks_array();
    u64 get_knight_attack(Square idx);

    u64 mask_king_attacks(u64 king_board);
    void init_king_attacks_array();
    u64 get_king_attack(Square idx);

    // cleanup -> make things const
    u64 get_rook_blocker_mask(Square sq);
    u64 raycast_rook_attacks(Square sq, u64 blocker_board);
    // potential high order function that executes on all rook blockers on a square
    // for now we can just give it a square and tell it to enumerate all combos
    // maybe can extend to be rook/bishop overload
    void init_rook_attacks_array();
    u64 index_rook_attacks(Square sq, u64 blocker_board);

    u64 get_bishop_blocker_mask(Square sq);
    u64 raycast_bishop_attacks(Square sq, u64 blocker_board);
    void init_bishop_attacks_array();
    u64 index_bishop_attacks(Square sq, u64 blocker_board);

    u64 index_queen_attacks(Square sq, u64 blocker_board);

} // namespace move_gen