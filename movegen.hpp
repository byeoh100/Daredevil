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

#include "encoder.hpp"
#include "game.hpp"
#include "types.hpp"
#include "utils.hpp"

class GameBoard;

namespace movegen {
// --------------------------------
// @@@ PRE-GAME MOVE GENERATION @@@
// --------------------------------
constexpr bitboard mask_king_attacks(const bitboard king_board) {
    return (
        types::shift(king_board, NORTH) | types::shift(king_board, NORTHEAST) |
        types::shift(king_board, EAST) | types::shift(king_board, SOUTHEAST) |
        types::shift(king_board, SOUTH) | types::shift(king_board, SOUTHWEST) |
        types::shift(king_board, WEST) | types::shift(king_board, NORTHWEST));
}
void init_king_attacks_array();
bitboard get_king_attack(square sq);

bitboard get_queen_attack(square sq, bitboard blocker_board);

// can combine a lot of rook/bishop logic
bitboard raycast_rook_attacks(square sq, bitboard blocker_board);
void init_rook_attacks_array();
bitboard get_rook_attack(square sq, bitboard blocker_board);

bitboard raycast_bishop_attacks(square sq, bitboard blocker_board);
void init_bishop_attacks_array();
bitboard get_bishop_attack(square sq, bitboard blocker_board);

constexpr bitboard mask_knight_attacks(const bitboard knight_board) {
    // travel out by two in all directions, then take a perpendicular step once

    bitboard north_two = types::shift(knight_board, NORTH, 2);
    bitboard south_two = types::shift(knight_board, SOUTH, 2);
    bitboard east_two = types::shift(knight_board, EAST, 2);
    bitboard west_two = types::shift(knight_board, WEST, 2);

    return (types::shift(north_two, EAST) | types::shift(north_two, WEST) |
            types::shift(south_two, EAST) | types::shift(south_two, WEST) |
            types::shift(east_two, NORTH) | types::shift(east_two, SOUTH) |
            types::shift(west_two, NORTH) | types::shift(west_two, SOUTH));
}
void init_knight_attacks_array();
bitboard get_knight_attack(square sq);

constexpr bitboard mask_pawn_attacks(const bitboard pawn_board, Color color) {
    if (color == WHITE) {
        return types::shift(pawn_board, NORTHWEST) |
               types::shift(pawn_board, NORTHEAST);
    } else {
        return types::shift(pawn_board, SOUTHWEST) |
               types::shift(pawn_board, SOUTHEAST);
    }
}
constexpr bitboard mask_pawn_quiets(bitboard pawn_board, Color color,
                          bitboard blocker_board) {
    if (color == WHITE) {
        pawn_board = types::shift(pawn_board, NORTH) & ~(blocker_board);
        bitboard double_move = types::shift(pawn_board, NORTH) &
                               ~(blocker_board) & (rank_masks[RANK_4]);
        return pawn_board | double_move;
    } else {
        pawn_board = types::shift(pawn_board, SOUTH) & ~(blocker_board);
        bitboard double_move = types::shift(pawn_board, SOUTH) &
                               ~(blocker_board) & (rank_masks[RANK_5]);
        return pawn_board | double_move;
    }
}
void init_pawn_attacks_array();
bitboard get_pawn_attack(square sq, Color color);

void init_all_pieces();

// --------------------------------
// @@@ IN-GAME MOVE GENERATION @@@
// --------------------------------
void generate_moves(const GameBoard& game_board, MoveList& move_list);
bool is_sq_attacked(square sq, const GameBoard& game_board, Color color);
bitboard get_legal_king_attack(square sq, const GameBoard& game_board,
                               Color color);
}  // namespace movegen
