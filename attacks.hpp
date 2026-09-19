#pragma once

#include "types.hpp"
#include "utils.hpp"
#include "game.hpp"

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

namespace move_gen {
    // --------------------------------
    // @@@ PRE-GAME MOVE GENERATION @@@
    // --------------------------------
    u64 mask_pawn_attacks(const u64 pawn_board, Color color);
    u64 mask_pawn_quiets(u64 pawn_board, Color color, u64 blocker_board);
    void init_pawn_attacks_array();
    u64 get_pawn_attack(Square idx, Color color);

    u64 mask_knight_attacks(const u64 knight_board);
    void init_knight_attacks_array();
    u64 get_knight_attack(Square idx);

    u64 mask_king_attacks(const u64 king_board);
    void init_king_attacks_array();
    u64 get_king_attack(Square idx);

    // can combine a lot of rook/bishop logic
    u64 get_rook_blocker_mask(Square sq);
    u64 raycast_rook_attacks(Square sq, u64 blocker_board);
    void init_rook_attacks_array();
    u64 index_rook_attacks(Square sq, u64 blocker_board);

    u64 get_bishop_blocker_mask(Square sq);
    u64 raycast_bishop_attacks(Square sq, u64 blocker_board);
    void init_bishop_attacks_array();
    u64 index_bishop_attacks(Square sq, u64 blocker_board);

    u64 index_queen_attacks(Square sq, u64 blocker_board);

    void init_all_pieces();

    // --------------------------------
    // @@@ IN-GAME MOVE GENERATION @@@
    // --------------------------------
    void generate_moves(const GameBoard& game_board, MoveList& move_list);
    u64 get_side_attacks(const GameBoard& game_board, Color color);
    bool is_sq_attacked(Square sq, const GameBoard& game_board, Color color);

    u64 get_legal_king_attacks(Square sq, const GameBoard& game_board, Color color);
    u64 get_castling(Square sq, const GameBoard& game_board, Color color);

    std::uint32_t encode_move(int source, int target, Piece piece, MoveFlag flag);
    Move decode_move(std::uint32_t encoded_move);
    void print_move(const Move& move);

    constexpr Square get_move_source(std::uint32_t move) {
        return static_cast<Square>(move & ((1 << 6) - 1));
    }
    constexpr Square get_move_target(std::uint32_t move) {
        return static_cast<Square>((move >> 6) & ((1 << 6) - 1));
    }
    constexpr Piece get_move_piece(std::uint32_t move) {
        return static_cast<Piece>((move >> 12) & ((1 << 4) - 1));
    }
    constexpr MoveFlag get_move_flag(std::uint32_t move) {
        return static_cast<MoveFlag>((move >> 16) & ((1 << 8) - 1));
    }

} // namespace move_gen