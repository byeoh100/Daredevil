#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>

using u64 = std::uint64_t;

enum Color {
    WHITE,
    BLACK
};

enum PieceType {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN,
    NO_PIECE_TYPE
};

enum Piece {
    WHITE_KING,
    WHITE_QUEEN,
    WHITE_ROOK,
    WHITE_BISHOP,
    WHITE_KNIGHT,
    WHITE_PAWN,
    BLACK_KING,
    BLACK_QUEEN,
    BLACK_ROOK,
    BLACK_BISHOP,
    BLACK_KNIGHT,
    BLACK_PAWN,
    NO_PIECE
};

// rank:
// 7
// 6
// 5
// 4
// 3
// 2
// 1 *<------------------0b1 (square ZERO)
// 0 1 2 3 4 5 6 7 file

// move right = << 1;
// move up = << 8

enum File {
    FILE_A,
    FILE_B,
    FILE_C,
    FILE_D,
    FILE_E,
    FILE_F,
    FILE_G,
    FILE_H,
};

enum Rank {
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4,
    RANK_5,
    RANK_6,
    RANK_7,
    RANK_8,
};

enum CastleRights {
    WHITE_KINGSIDE,
    WHITE_QUEENSIDE,
    BLACK_KINGSIDE,
    BLACK_QUEENSIDE
};