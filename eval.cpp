#include "eval.hpp"

namespace eval {

int evaluate(const GameBoard& game_board) {
    int score = 0;
    const auto pieces = game_board.get_piece_view();
    for (size_t i = 0; i < 12; i++) {
        int piece_count = 0;
        bitboard piece_board = pieces[i];

        while (piece_board) {
            square sq = utils::pop_lsb(piece_board);
            (i < 6) ? score += piece_tables[i][sq]
                    : score -=
                      piece_tables[i - 6]
                                  [sq ^ 56];  // flip the orientation to black
            piece_count++;
        }

        score += piece_count * piece_values[i];
    }

    return (game_board.get_to_move() == WHITE) ? score : -score;
}

}  // namespace eval

// note to self: update the makefile to describe the new search + eval