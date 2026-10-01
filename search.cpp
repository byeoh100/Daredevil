#include "search.hpp"

namespace search {

namespace {
bool is_in_check(const GameBoard& game_board) {
    Color color = game_board.get_to_move();
    Piece king = (color == WHITE) ? WHITE_KING : BLACK_KING;
    square king_sq = std::countr_zero(game_board.get_piece(king));
    return movegen::is_sq_attacked(king_sq, game_board, color);
}

// temporary solution to grab a little more attacking information
// upgrade by tagging a check flag when we do fully-legal move gen
std::array<bitboard, 6> get_check_squares(const GameBoard& game_board) {
    Color us = game_board.get_to_move();
    Color them = (us == WHITE) ? BLACK : WHITE;
    square king_sq =
        std::countr_zero(game_board.get_piece(make_piece(KING, them)));
    bitboard occupancy = game_board.get_occupancy();

    std::array<bitboard, 6> check_squares{};
    // our pawn attacks king_sq exactly when their pawn on king_sq would attack
    // it
    check_squares[PAWN] = movegen::get_pawn_attack(king_sq, them);
    check_squares[KNIGHT] = movegen::get_knight_attack(king_sq);
    check_squares[BISHOP] = movegen::get_bishop_attack(king_sq, occupancy);
    check_squares[ROOK] = movegen::get_rook_attack(king_sq, occupancy);
    check_squares[QUEEN] = check_squares[BISHOP] | check_squares[ROOK];
    check_squares[KING] = 0ULL;  // a king can never give check
    return check_squares;
}

void pick_move(MoveList& move_list, std::array<int, 256>& scores, int start) {
    // lazy selection sort
    int best = start;
    for (int i = start + 1; i < move_list.count; i++) {
        if (scores[i] > scores[best]) best = i;
    }
    std::swap(move_list.moves[start], move_list.moves[best]);
    std::swap(scores[start], scores[best]);
}
}  // namespace

std::array<int, 256> score_moves(const GameBoard& game_board,
                                 const MoveList& move_list) {
    std::array<int, 256> scores;
    auto check_squares = get_check_squares(game_board);

    for (int i = 0; i < move_list.count; i++) {
        int score = 0;
        u32 move = move_list.moves[i];
        square target = encoder::get_move_target(move);
        PieceType attacker = piece_type(encoder::get_move_piece(move));
        MoveFlag flag = encoder::get_move_flag(move);

        u8 queen_bits = 0b0011;
        if (flag & PROMOTION_BIT) {
            if ((flag & queen_bits) == queen_bits) {
                if (!(flag & CAPTURE_BIT)) score += CAPTURE_BONUS;
                score += QUEEN * 10;
                attacker = QUEEN;
            } else {
                scores[i] = UNDERPROMOTION_SCORE;
                continue;
            }
        }

        if (flag & CAPTURE_BIT) {
            Piece target_piece = game_board.piece_on(target);
            // MVV LVA
            (flag == EN_PASSANT)
                ? score += score_capture(PAWN, PAWN)
                : score += score_capture(piece_type(target_piece), attacker);
        }

        if (check_squares[attacker] & (1ULL << target)) score += 4000;

        scores[i] = score;
    }

    return scores;
}

int quiescence(const GameBoard& game_board, SearchState& state, int alpha,
               int beta) {
    state.node_count++;
    int stand_pat = eval::evaluate(game_board);
    if (stand_pat >= beta) return beta;
    if (stand_pat > alpha) alpha = stand_pat;

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);
    auto scores = score_moves(game_board, move_list);

    for (int i = 0; i < move_list.count; i++) {
        pick_move(move_list, scores, i);
        if (scores[i] < CAPTURE_BONUS - 100) break;

        GameBoard copy = game_board;
        if (!copy.make_move(move_list.moves[i])) continue;

        int evaluation = -quiescence(copy, state, -beta, -alpha);

        if (evaluation >= beta) return beta;
        if (evaluation > alpha) alpha = evaluation;
    }

    return alpha;
}

int nega_max(const GameBoard& game_board, SearchState& state, int alpha,
             int beta, int depth, int ply) {
    if (depth == 0 || ply >= MAX_PLY)
        return quiescence(game_board, state, alpha, beta);
    if (depth == 0) return quiescence(game_board, state, alpha, beta);
    state.node_count++;

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);
    auto scores = score_moves(game_board, move_list);

    int legal_moves = 0;
    for (int i = 0; i < move_list.count; i++) {
        pick_move(move_list, scores, i);
        GameBoard copy = game_board;
        if (!copy.make_move(move_list.moves[i])) continue;
        legal_moves++;

        int extension = is_in_check(copy) ? 1 : 0;
        int evaluation = -nega_max(copy, state, -beta, -alpha,
                                   depth - 1 + extension, ply + 1);

        if (evaluation >= beta) return beta;
        if (evaluation > alpha) alpha = evaluation;
    }

    if (legal_moves == 0) {
        return is_in_check(game_board) ? -MATE_SCORE + ply : 0;
    }

    return alpha;
}

SearchResult search_best(const GameBoard& game_board, int depth) {
    SearchState state;

    u32 best = 0;
    int alpha = INIT_ALPHA;
    int beta = INIT_BETA;

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);
    auto scores = score_moves(game_board, move_list);

    for (int i = 0; i < move_list.count; i++) {
        pick_move(move_list, scores, i);
        GameBoard copy = game_board;
        if (!copy.make_move(move_list.moves[i])) continue;

        int extension = is_in_check(copy) ? 1 : 0;
        int evaluation =
            -nega_max(copy, state, -beta, -alpha, depth - 1 + extension, 1);
        if (evaluation > alpha) {
            alpha = evaluation;
            best = move_list.moves[i];
        }
    }

    return {best, alpha, state.node_count};
}

}  // namespace search