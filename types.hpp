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
// 1 
// 0 *<------------------0b1 (square ZERO)
// \ 0 1 2 3 4 5 6 7 file

// move right = << 1;
// move up = << 8

enum Square : int {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
    NO_SQUARE, // = 64
};

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

const u64 FILE_A_MASK = 0x0101010101010101;
// access with file_masks[File];
constexpr std::array<u64, 8> file_masks = {
    FILE_A_MASK,
    FILE_A_MASK << 1, // FILE B
    FILE_A_MASK << 2, // FILE C
    FILE_A_MASK << 3, // ...
    FILE_A_MASK << 4,
    FILE_A_MASK << 5,
    FILE_A_MASK << 6,
    FILE_A_MASK << 7, // FILE H
};

const u64 RANK_1_MASK = 0xff;
// access with rank_masks[Rank]
constexpr std::array<u64, 8> rank_masks = {
    RANK_1_MASK,
    RANK_1_MASK << 8,  // RANK 2
    RANK_1_MASK << 16, // RANK 3
    RANK_1_MASK << 24, // ...
    RANK_1_MASK << 32,
    RANK_1_MASK << 40,
    RANK_1_MASK << 48,
    RANK_1_MASK << 56, // RANK 8
};

const u64 CORNER_SQUARES = 9295429630892703873ULL;
const u64 EDGE_SQUARES = (file_masks[FILE_A] | file_masks[FILE_H] | rank_masks[RANK_1] | rank_masks[RANK_8]) & ~(CORNER_SQUARES);

enum CastleRights {
    WHITE_KINGSIDE,
    WHITE_QUEENSIDE,
    BLACK_KINGSIDE,
    BLACK_QUEENSIDE
};

