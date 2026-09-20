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

// perhaps some error handling for if south doesn't work
enum Direction : int {
    NORTH = 8,
    EAST = 1,
    SOUTH = -8,
    WEST = -1,

    NORTHEAST = 9,
    SOUTHEAST = -7,
    SOUTHWEST = -9,
    NORTHWEST = 7
};

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

// Cumulative file masking (useful for cleaning up bulk shifts)
constexpr std::array<u64, 8> leftcum_file_masks = {
    file_masks[FILE_A],
    file_masks[FILE_A] | file_masks[FILE_B],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C] | file_masks[FILE_D],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C] | file_masks[FILE_D] | file_masks[FILE_E],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C] | file_masks[FILE_D] | file_masks[FILE_E] | file_masks[FILE_F],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C] | file_masks[FILE_D] | file_masks[FILE_E] | file_masks[FILE_F] | file_masks[FILE_G],
    file_masks[FILE_A] | file_masks[FILE_B] | file_masks[FILE_C] | file_masks[FILE_D] | file_masks[FILE_E] | file_masks[FILE_F] | file_masks[FILE_G] | file_masks[FILE_H]
};

constexpr std::array<u64, 8> rightcum_file_masks = {
    file_masks[FILE_H],
    file_masks[FILE_H] | file_masks[FILE_G],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F] | file_masks[FILE_E],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F] | file_masks[FILE_E] | file_masks[FILE_D],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F] | file_masks[FILE_E] | file_masks[FILE_D] | file_masks[FILE_C],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F] | file_masks[FILE_E] | file_masks[FILE_D] | file_masks[FILE_C] | file_masks[FILE_B],
    file_masks[FILE_H] | file_masks[FILE_G] | file_masks[FILE_F] | file_masks[FILE_E] | file_masks[FILE_D] | file_masks[FILE_C] | file_masks[FILE_B] | file_masks[FILE_A]
};

// refactor some masks with this
constexpr u64 shift(u64 board, const Direction& dir, const int amt = 1) {
    switch(dir) {
        case NORTH:
            return board << (8 * amt);
            break;
        case SOUTH:
            return board >> (8 * amt);
            break;
        case EAST:
            return (board & ~rightcum_file_masks[amt - 1]) << amt;
            break;
        case WEST:
            return (board & ~leftcum_file_masks[amt - 1]) >> amt;
            break;
        case NORTHEAST:
            return (board & ~rightcum_file_masks[amt - 1]) << (9 * amt);
            break;
        case SOUTHEAST:
            return (board & ~rightcum_file_masks[amt - 1]) >> (7 * amt);
            break;
        case SOUTHWEST:
            return (board & ~leftcum_file_masks[amt - 1]) >> (9 * amt);
            break;
        case NORTHWEST:
            return (board & ~leftcum_file_masks[amt - 1]) << (7 * amt);
            break;
        default:
            return board;
    }
}

// Longest diagonals
const u64 DIAG_8_MASK = 9241421688590303745ULL; // Square a1 -> h8
const u64 ANTIDIAG_8_MASK = 72624976668147840ULL; // Square h1 -> a8

// 7 + rank - file
constexpr std::array<u64, 15> diagonal_masks = {
    shift(DIAG_8_MASK, EAST, 7),
    shift(DIAG_8_MASK, EAST, 6),
    shift(DIAG_8_MASK, EAST, 5),
    shift(DIAG_8_MASK, EAST, 4),
    shift(DIAG_8_MASK, EAST, 3),
    shift(DIAG_8_MASK, EAST, 2),
    shift(DIAG_8_MASK, EAST),
    DIAG_8_MASK,
    shift(DIAG_8_MASK, WEST),
    shift(DIAG_8_MASK, WEST, 2),
    shift(DIAG_8_MASK, WEST, 3),
    shift(DIAG_8_MASK, WEST, 4),
    shift(DIAG_8_MASK, WEST, 5),
    shift(DIAG_8_MASK, WEST, 6),
    shift(DIAG_8_MASK, WEST, 7),
};
// rank + file
constexpr std::array<u64, 15> antidiagonal_masks = {
    shift(ANTIDIAG_8_MASK, WEST, 7),
    shift(ANTIDIAG_8_MASK, WEST, 6),
    shift(ANTIDIAG_8_MASK, WEST, 5),
    shift(ANTIDIAG_8_MASK, WEST, 4),
    shift(ANTIDIAG_8_MASK, WEST, 3),
    shift(ANTIDIAG_8_MASK, WEST, 2),
    shift(ANTIDIAG_8_MASK, WEST),
    ANTIDIAG_8_MASK,
    shift(ANTIDIAG_8_MASK, EAST),
    shift(ANTIDIAG_8_MASK, EAST, 2),
    shift(ANTIDIAG_8_MASK, EAST, 3),
    shift(ANTIDIAG_8_MASK, EAST, 4),
    shift(ANTIDIAG_8_MASK, EAST, 5),
    shift(ANTIDIAG_8_MASK, EAST, 6),
    shift(ANTIDIAG_8_MASK, EAST, 7),
};

const u64 CORNER_SQUARES = 9295429630892703873ULL;
const u64 EDGE_SQUARES = (file_masks[FILE_A] | file_masks[FILE_H] | rank_masks[RANK_1] | rank_masks[RANK_8]) & ~(CORNER_SQUARES);
const u64 BOARD_EDGE = CORNER_SQUARES | EDGE_SQUARES;

enum CastleRights : std::uint8_t {
    WHITE_KINGSIDE = 0b0001,
    WHITE_QUEENSIDE = 0b0010,
    BLACK_KINGSIDE = 0b0100,
    BLACK_QUEENSIDE = 0b1000
};

enum MoveFlag : std::uint8_t {
    // regular moves
    QUIET_MOVE = 0b0000,
    DOUBLE_MOVE = 0b0001,
    // bit 2 set = castling
    CASTLE_KINGSIDE = 0b0010,
    CASTLE_QUEENSIDE = 0b0011,
    // bit 3 set = capture
    CAPTURE = 0b0100,
    EN_PASSANT = 0b0101,
    // gap of 6 and 7
    // bit 4 set = promotion
    PROMOTION_N = 0b1000,
    PROMOTION_B = 0b1001,
    PROMOTION_R = 0b1010,
    PROMOTION_Q = 0b1011,
    // set bit 3 and 4 for cap + promo
    CAPTURE_PROMO_N = 0b1100,
    CAPTURE_PROMO_B = 0b1101,
    CAPTURE_PROMO_R = 0b1110,
    CAPTURE_PROMO_Q = 0b1111
};

struct Move {
    Square source = NO_SQUARE;
    Square target = NO_SQUARE;
    Piece piece = NO_PIECE;
    MoveFlag flag;
};

struct MoveList {
    std::array<std::uint32_t, 256> moves{};
    int count = 0;

    void push(std::uint32_t move) {
        if(count < 256) {
            moves[count] = move;
            count++;
        }
    }

    std::uint32_t pop() {
        if(count > 0) {
            count--;
            return moves[count];
        }

        return 0;
    }
};