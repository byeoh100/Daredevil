#pragma once

#include "types.hpp"
#include "utils.hpp"
#include "game.hpp"
#include "moves.hpp"

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

class GameBoard;

namespace move_gen {
    // --------------------------------
    // @@@ PRE-GAME MOVE GENERATION @@@
    // --------------------------------
    u64 mask_pawn_attacks(const u64 pawn_board, Color color);
    u64 mask_pawn_quiets(u64 pawn_board, Color color, u64 blocker_board);
    void init_pawn_attacks_array();
    u64 get_pawn_attack(int sq, Color color);

    u64 mask_knight_attacks(const u64 knight_board);
    void init_knight_attacks_array();
    u64 get_knight_attack(int sq);

    u64 mask_king_attacks(const u64 king_board);
    void init_king_attacks_array();
    u64 get_king_attack(int sq);

    // can combine a lot of rook/bishop logic
    u64 get_rook_blocker_mask(int sq);
    u64 raycast_rook_attacks(int sq, u64 blocker_board);
    void init_rook_attacks_array();
    u64 index_rook_attacks(int sq, u64 blocker_board);

    u64 get_bishop_blocker_mask(int sq);
    u64 raycast_bishop_attacks(int sq, u64 blocker_board);
    void init_bishop_attacks_array();
    u64 index_bishop_attacks(int sq, u64 blocker_board);

    u64 index_queen_attacks(int sq, u64 blocker_board);

    void init_all_pieces();

    // --------------------------------
    // @@@ IN-GAME MOVE GENERATION @@@
    // --------------------------------
    void generate_moves(const GameBoard& game_board, MoveList& move_list);

    u64 get_side_attacks(const GameBoard& game_board, Color color);
    bool is_sq_attacked(int sq, const GameBoard& game_board, Color color);

    u64 get_legal_king_attacks(Square sq, const GameBoard& game_board, Color color);
    bool is_castle_squares_clear(std::uint8_t castle_bit, const GameBoard& game_board);
} // namespace move_gen