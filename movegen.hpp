#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "game.hpp"
#include "moves.hpp"
#include "types.hpp"
#include "utils.hpp"

class GameBoard;

namespace movegen {
// --------------------------------
// @@@ PRE-GAME MOVE GENERATION @@@
// --------------------------------
bitboard mask_king_attacks(const bitboard king_board);
void init_king_attacks_array();
bitboard get_king_attack(square sq);

bitboard get_queen_attack(square sq, bitboard blocker_board);

// can combine a lot of rook/bishop logic
bitboard get_rook_blocker_mask(square sq);
bitboard raycast_rook_attacks(square sq, bitboard blocker_board);
void init_rook_attacks_array();
bitboard get_rook_attack(square sq, bitboard blocker_board);

bitboard get_bishop_blocker_mask(square sq);
bitboard raycast_bishop_attacks(square sq, bitboard blocker_board);
void init_bishop_attacks_array();
bitboard get_bishop_attack(square sq, bitboard blocker_board);

bitboard mask_knight_attacks(const bitboard knight_board);
void init_knight_attacks_array();
bitboard get_knight_attack(square sq);

bitboard mask_pawn_attacks(const bitboard pawn_board, Color color);
bitboard mask_pawn_quiets(bitboard pawn_board, Color color,
                          bitboard blocker_board);
void init_pawn_attacks_array();
bitboard get_pawn_attack(square sq, Color color);

void init_all_pieces();

// --------------------------------
// @@@ IN-GAME MOVE GENERATION @@@
// --------------------------------
void generate_moves(const GameBoard& game_board, MoveList& move_list);

bitboard get_side_attack(const GameBoard& game_board, Color color);
bool is_sq_attacked(square sq, const GameBoard& game_board, Color color);

bitboard get_legal_king_attack(square sq, const GameBoard& game_board,
                               Color color);
bool is_castle_squares_clear(u8 castle_bit, const GameBoard& game_board);
}  // namespace movegen
