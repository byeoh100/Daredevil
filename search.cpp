// function called search_best
// this function returns the best move

// inside the function is the recursive negamax
// negamax takes gameboard, alpha, beta, white
// if white you are max evaling
// for each of the children you must recurse to get an eval
// copy board and send it down
// the score you get is the negative
// this is black move -> negate alpha/beta and swap
// if the score is the cutoff then prune because obviously black wouldn't allow
// if the score is alpha then we found a better move
// literally the same way we make decisions irl in chess

#include "search.hpp"

namespace search {

namespace {
bool is_in_check(const GameBoard& game_board) {
    Color color = game_board.get_to_move();
    Piece king = (color == WHITE) ? WHITE_KING : BLACK_KING;
    square king_sq = std::countr_zero(game_board.get_piece(king));
    return movegen::is_sq_attacked(king_sq, game_board, color);
}
}  // namespace

int nega_max(const GameBoard& game_board, int alpha, int beta, int depth,
             int ply) {
    if (depth == 0) return eval::evaluate(game_board);

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);

    int legal_moves = 0;
    for (int i = 0; i < move_list.count; i++) {
        GameBoard copy = game_board;
        if (!copy.make_move(move_list.moves[i])) continue;
        legal_moves++;

        int score = -nega_max(copy, -beta, -alpha, depth - 1, ply + 1);

        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }

    if (legal_moves == 0) {
        return is_in_check(game_board) ? -30000 + ply : 0;
    }

    return alpha;
}

u32 search_best(const GameBoard& game_board, int depth) {
    u32 best = 0;
    int alpha = -10000;
    int beta = 10000;

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);

    for (int i = 0; i < move_list.count; i++) {
        GameBoard copy = game_board;
        if (!copy.make_move(move_list.moves[i])) continue;

        int score = -nega_max(copy, -beta, -alpha, depth - 1, 1);
        if (score > alpha) {
            alpha = score;
            best = move_list.moves[i];
        }
    }

    return best;
}

}  // namespace search