#include "utils.hpp"

Piece char_to_piece(char c) {
    switch(c) {
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