#include "attacks.hpp"

namespace move_gen {
    // All pre-generated tables are kept hidden
    namespace {
        std::array<u64, 64> white_pawn_attacks;
        std::array<u64, 64> black_pawn_attacks;
    } // namespace

    u64 mask_pawn_attacks(u64 pawn_board, Color color) {
        u64 left_attacks = 0;
        u64 right_attacks = 0;

        if(color == Color::WHITE) {
            left_attacks = (pawn_board & ~(file_masks[File::FILE_A])) << 7;
            right_attacks = (pawn_board & ~(file_masks[File::FILE_H])) << 9;
        }
        else {
            left_attacks = (pawn_board & ~(file_masks[File::FILE_A])) >> 9;
            right_attacks = (pawn_board & ~(file_masks[File::FILE_H])) >> 7;
        }

        return left_attacks | right_attacks;
    }

    void init_pawn_attacks_arrays() {
        for(int i = 0; i < 64; i++) {
            if(i < 8 || i >= 56) {
                white_pawn_attacks[i] = 0;
                black_pawn_attacks[i] = 0;
            }
            else {
                u64 single_pawn_board = (1ULL << i);
                white_pawn_attacks[i] = mask_pawn_attacks(single_pawn_board, Color::WHITE);
                black_pawn_attacks[i] = mask_pawn_attacks(single_pawn_board, Color::BLACK);
            }
        }
    }

    std::array<u64, 64> get_pawn_attacks(Color color) {
        if(color == Color::WHITE) {
            return white_pawn_attacks;
        }
        else {
            return black_pawn_attacks;
        }
    }

} // namespace move_gen