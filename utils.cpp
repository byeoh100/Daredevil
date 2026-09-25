#include "utils.hpp"

namespace utils {
void print_bitboard(bitboard board) {
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            square sq = rank * 8 + file;

            if ((board >> sq) & 1ULL) {
                std::cout << "1 ";
            } else {
                std::cout << "_ ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Piece char_to_piece(char c) {
    switch (c) {
        case 'k':
            return BLACK_KING;
            break;
        case 'q':
            return BLACK_QUEEN;
            break;
        case 'r':
            return BLACK_ROOK;
            break;
        case 'b':
            return BLACK_BISHOP;
            break;
        case 'n':
            return BLACK_KNIGHT;
            break;
        case 'p':
            return BLACK_PAWN;
            break;
        case 'K':
            return WHITE_KING;
            break;
        case 'Q':
            return WHITE_QUEEN;
            break;
        case 'R':
            return WHITE_ROOK;
            break;
        case 'B':
            return WHITE_BISHOP;
            break;
        case 'N':
            return WHITE_KNIGHT;
            break;
        case 'P':
            return WHITE_PAWN;
            break;
        default:
            return NO_PIECE;
    }
}

std::string square_to_algebraic(square sq) {
    if (sq == NO_SQUARE) return "-";

    std::string algebraic;
    algebraic += static_cast<char>('a' + sq % 8);
    algebraic += static_cast<char>('1' + sq / 8);
    return algebraic;
}

square algebraic_to_square(const std::string& algebraic) {
    if (algebraic == "-") return NO_SQUARE;

    int file = algebraic[0] - 'a';
    int rank = algebraic[1] - '1';
    return rank * 8 + file;
}
}  // namespace utils
