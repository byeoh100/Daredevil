#include "attacks.hpp"

namespace move_gen {
    // All pre-generated tables are kept hidden
    namespace {
        std::array<u64, 64> white_pawn_attacks;
        std::array<u64, 64> black_pawn_attacks;

        std::array<u64, 64> knight_attacks;

        std::array<u64, 64> king_attacks;

        // array for rook attacks
        //4*2^12 + 6*2^11 + 36*2^10 sized = 102,400

    } // namespace

    u64 mask_pawn_attacks(u64 pawn_board, Color color) {
        u64 left_attacks = 0;
        u64 right_attacks = 0;

        if(color == WHITE) {
            left_attacks = (pawn_board & ~(file_masks[FILE_A])) << 7;
            right_attacks = (pawn_board & ~(file_masks[FILE_H])) << 9;
        }
        else {
            left_attacks = (pawn_board & ~(file_masks[FILE_A])) >> 9;
            right_attacks = (pawn_board & ~(file_masks[FILE_H])) >> 7;
        }

        return left_attacks | right_attacks;
    }

    void init_pawn_attacks_array() {
        for(int i = 0; i < 64; i++) {
            if(i < 8 || i >= 56) {
                white_pawn_attacks[i] = 0;
                black_pawn_attacks[i] = 0;
            }
            else {
                u64 single_pawn_board = (1ULL << i);
                white_pawn_attacks[i] = mask_pawn_attacks(single_pawn_board, WHITE);
                black_pawn_attacks[i] = mask_pawn_attacks(single_pawn_board, BLACK);
            }
        }
    }

    u64 get_pawn_attack(Square idx, Color color) {
        if(color == WHITE) {
            return white_pawn_attacks[idx];
        }
        else {
            return black_pawn_attacks[idx];
        }
    }

    u64 mask_knight_attacks(u64 knight_board) {
        u64 file_ab = file_masks[FILE_A] | file_masks[FILE_B];
        u64 file_gh = file_masks[FILE_G] | file_masks[FILE_H];
        u64 rank_12 = rank_masks[RANK_1] | rank_masks[RANK_2];
        u64 rank_78 = rank_masks[RANK_7] | rank_masks[RANK_8];

        // n/e/s/w -> n2w1 = north 2 west 1 -> up 2 left 1 knight move
        // clockwise from top
        u64 n2w1 = (knight_board & ~(file_masks[FILE_A]) & ~(rank_78)) << 15;
        u64 n2e1 = (knight_board & ~(file_masks[FILE_H]) & ~(rank_78)) << 17;

        u64 n1e2 = (knight_board & ~(file_gh) & ~(rank_masks[RANK_8])) << 10;
        u64 s1e2 = (knight_board & ~(file_gh) & ~(rank_masks[RANK_1])) >> 6;

        u64 s2e1 = (knight_board & ~(file_masks[FILE_H]) & ~(rank_12)) >> 15;
        u64 s2w1 = (knight_board & ~(file_masks[FILE_A]) & ~(rank_12)) >> 17;

        u64 s1w2 = (knight_board & ~(file_ab) & ~(rank_masks[RANK_1])) >> 10;
        u64 n1w2 = (knight_board & ~(file_ab) & ~(rank_masks[RANK_8])) << 6;

        return (n2w1 | n2e1 | n1e2 | s1e2 | s2e1 | s2w1 | s1w2 | n1w2);
    }

    void init_knight_attacks_array() {
        for(int i = 0; i < 64; i++) {
            u64 single_knight_board = (1ULL << i);
            knight_attacks[i] = mask_knight_attacks(single_knight_board);
        }
    }

    u64 get_knight_attack(Square idx) {
        return knight_attacks[idx];
    }

    u64 mask_king_attacks(u64 king_board) {
        u64 north = (king_board & ~(rank_masks[RANK_8])) << 8;
        u64 north_east = (king_board & ~(rank_masks[RANK_8]) & ~(file_masks[FILE_H])) << 9;
        u64 east = (king_board & ~(file_masks[FILE_H])) << 1;
        u64 south_east = (king_board & ~(rank_masks[RANK_1]) & ~(file_masks[FILE_H])) >> 7;
        u64 south = (king_board & ~(rank_masks[RANK_1])) >> 8;
        u64 south_west = (king_board & ~(rank_masks[RANK_1]) & ~(file_masks[FILE_A])) >> 9;
        u64 west = (king_board & ~(file_masks[FILE_A])) >> 1;
        u64 north_west = (king_board & ~(rank_masks[RANK_8]) & ~(file_masks[FILE_A])) << 7;

        return (north | north_east | east | south_east | south | south_west | west | north_west);
    }

    void init_king_attacks_array() {
        for(int i = 0; i < 64; i++) {
            u64 single_king_board = (1ULL << i);
            king_attacks[i] = mask_king_attacks(single_king_board);
        }
    }

    u64 get_king_attack(Square idx) {
        return king_attacks[idx];
    }

    u64 mask_rook_blockers(u64 rook_board, u64 occupancy) {
        // mask the raw row first - the edges
        // then just mask on top of an occupancy board
        // those are your blockers

        if(!(std::has_single_bit(rook_board))) throw std::length_error("mask_rook_blockers() took more than 1 bit.");

        int bit_index = std::countr_zero(rook_board);
        int rank = bit_index / 8;
        int file = bit_index % 8;

        u64 raw_attacks = (file_masks[file] | rank_masks[rank]) & ~(rook_board);
        u64 blocker_mask;

        if(rook_board & CORNER_SQUARES) {
            blocker_mask = raw_attacks & EDGE_SQUARES;
        }
        else if(rook_board & EDGE_SQUARES) {
            blocker_mask = raw_attacks & ~(CORNER_SQUARES);
            if (rank == 0) blocker_mask &= ~rank_masks[RANK_8];
            if (rank == 7) blocker_mask &= ~rank_masks[RANK_1];
            if (file == 0) blocker_mask &= ~file_masks[FILE_H];
            if (file == 7) blocker_mask &= ~file_masks[FILE_A];
        }
        else {
            blocker_mask = raw_attacks & ~(EDGE_SQUARES);
        }

        return blocker_mask & occupancy;

    }

    // now do a raycast one (easy)
    // also an enum blocker masks one which basically turn all relevant rows + col
    // into an N bit number and count from 0 to max size and that is your different
    // bits within the actual blocker mask to change

} // namespace move_gen