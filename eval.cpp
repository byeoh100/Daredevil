#include "eval.hpp"

namespace eval {

int evaluate(const GameBoard& game_board) {
    int score = 0;
    const auto pieces = game_board.get_piece_view();
    for (int i = 0; i < 12; i++) {
        Piece piece = static_cast<Piece>(i);
        bitboard piece_board = pieces[piece];

        while (piece_board) {
            square sq = utils::pop_lsb(piece_board);
            square table_sq = (piece_color(piece) == WHITE) ? sq : sq ^ 56;
            int sq_bonus = piece_tables[piece_type(piece)][table_sq];
            score += (piece_color(piece) == WHITE) ? sq_bonus : -sq_bonus;
            score += piece_values[piece];
        }
    }

    return (game_board.get_to_move() == WHITE) ? score : -score;
}

}  // namespace eval