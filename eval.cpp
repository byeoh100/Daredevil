#include "eval.hpp"

namespace eval {

int evaluate(const GameBoard& game_board) {
    int score = 0;
    int mg_score = 0;
    int eg_score = 0;
    int phase = 0;
    const auto pieces = game_board.get_piece_view();
    for (int i = 0; i < 12; i++) {
        Piece piece = static_cast<Piece>(i);
        PieceType type = piece_type(piece);
        bitboard piece_board = pieces[piece];
        bool is_white = (piece_color(piece) == WHITE);

        while (piece_board) {
            square sq = utils::pop_lsb(piece_board);
            square table_sq = is_white ? sq : sq ^ 56;

            int mg = mg_piece_values[type] + mg_piece_tables[type][table_sq];
            int eg = eg_piece_values[type] + eg_piece_tables[type][table_sq];
            mg_score += is_white ? mg : -mg;
            eg_score += is_white ? eg : -eg;

            phase += game_phase_values[type];
        }
    }
    if (phase > 24) phase = 24;
    score += (mg_score * phase + eg_score * (24 - phase)) / 24;

    return (game_board.get_to_move() == WHITE) ? score : -score;
}

}  // namespace eval