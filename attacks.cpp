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
        std::array<u64, 102400> rook_attacks{};
        std::array<u64, 64> rook_magics { // hardcoded magics generated with init
            612489663141167106ULL,
            36063982465110144ULL,
            72066666367836162ULL,
            72066407857127680ULL,
            10520410931393208336ULL,
            36030998189965313ULL,
            72063091604586500ULL,
            14051232214234964224ULL,
            144255963146321952ULL,
            90353742410490112ULL,
            1441293168271187968ULL,
            36732553205651456ULL,
            2307109680969941120ULL,
            577023771009879048ULL,
            146929959351354376ULL,
            1153484455651583106ULL,
            141287248363553ULL,
            8101978203542782024ULL,
            72200531893944384ULL,
            9799871272216170496ULL,
            864973703045195776ULL,
            18155685820826112ULL,
            2522020189391816770ULL,
            3460666668945252417ULL,
            141173427552266ULL,
            4679523639493768ULL,
            145522876493078528ULL,
            4902274851316695048ULL,
            9314156517232218368ULL,
            562958544405512ULL,
            5066558170826756ULL,
            11538258537797141668ULL,
            141012374654048ULL,
            18023471669846016ULL,
            1171499128518086788ULL,
            10448360000328835328ULL,
            4688810404275556384ULL,
            6090133342198440961ULL,
            2307250710699317760ULL,
            9029329107288129ULL,
            587860489933979684ULL,
            585468157787914242ULL,
            9007757868957732ULL,
            145277852919595016ULL,
            1225542083495198736ULL,
            563019210032128ULL,
            18023263365758981ULL,
            36733586124177412ULL,
            127262423605314944ULL,
            35186859311232ULL,
            153124655075459200ULL,
            140943648358528ULL,
            6958202178956165504ULL,
            563018807412224ULL,
            1155331737543312384ULL,
            4683884351044192384ULL,
            9223653581188792386ULL,
            72093057651081730ULL,
            175926156528833ULL,
            168924637165003009ULL,
            2814767217510402ULL,
            577023775542363142ULL,
            565222260647940ULL,
            576461306499170374ULL
        };
        std::array<int, 64> rook_offsets;

        // Helpers
        // u64 gen_magic_candidate() {
        //     std::random_device rd;
        //     std::mt19937_64 gen(rd());
        //     std::uniform_int_distribution<uint64_t> dist(0, std::numeric_limits<uint64_t>::max());

        //     return dist(gen) & dist(gen) & dist(gen);
        // }

        void init_rook_offsets() {
            int offset = 0;

            for(int sq = 0; sq < 64; sq++) {
                rook_offsets[sq] = offset;
                u64 rook_board = 1ULL << sq;
                if(rook_board & CORNER_SQUARES) {
                    offset += (1ULL << 12);
                }
                else if(rook_board & EDGE_SQUARES) {
                    offset += (1ULL << 11);
                }
                else {
                    offset += (1ULL << 10);
                }
            }
        }

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

    u64 get_rook_blocker_mask(Square sq) {
        // mask the raw row first - the edges
        // those are your blockers

        int rank = sq / 8;
        int file = sq % 8;

        u64 rook_board = 1ULL << sq;
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

        return blocker_mask;
    }

    u64 raycast_rook_attacks(Square sq, u64 blocker_board) {
        int rank = sq / 8;
        int file = sq % 8;
        u64 cross_check = rank_masks[rank] | file_masks[file];

        u64 target_board = 1ULL << sq;
        if((target_board | cross_check) != (blocker_board | cross_check)) {
            throw std::invalid_argument("raycast_rook_attacks() - invalid blocker_board");
        }

        auto fire_ray = [file](u64 start, const u64& blocker, const Direction& dir) {
            int local_file = file;
            u64 ray = 0ULL;
            while((start | blocker) != blocker) {
                if((local_file == 0 && dir == WEST) || (local_file == 7 && dir == EAST)) break;
                start = shift(start, dir);
                ray |= start;
                (dir == WEST) ? local_file-- : (dir == EAST) ? local_file++ : 0;
            }
            return ray;
        };

        return (
            fire_ray(target_board, blocker_board, NORTH)
            | fire_ray(target_board, blocker_board, EAST)
            | fire_ray(target_board, blocker_board, SOUTH)
            | fire_ray(target_board, blocker_board, WEST)
        );
    }

    // the bits are read as (b = blocker, given bit settings = 0 -> N - 1)
    // bits read smallest to greatest sq of blocker on a bitboard
    // rank:
    // 7
    // 6 11
    // 5 10
    // 4 9
    // 3 8
    // 2 7
    // 1 6
    // 0 b 0 1 2 3 4 5
    // \ 0 1 2 3 4 5 6 7 file
    // *bit settings for rook on a1*
    void init_rook_attacks_array() {
        init_rook_offsets();
        auto write_bit_settings = [](u64 mask, unsigned int bits) {
            u64 output = 0ULL;
            while(mask != 0U) {
                int lsb_index = std::countr_zero(mask);
                u64 write_bit = 1ULL << lsb_index;
                if((1U & bits) != 0) {
                    output |= write_bit;
                }
                bits >>= 1;
                mask &= (mask - 1);
            }

            return output;
        };

        for(int i = 0; i < 64; i++) {
            u64 blocker_mask = get_rook_blocker_mask(static_cast<Square>(i));

            int relevant_bits = std::popcount(blocker_mask);
            // u64 candidate = gen_magic_candidate();
            unsigned int bit_settings = 0U;

            while(bit_settings < (1U << relevant_bits)) {
                u64 written_settings = write_bit_settings(blocker_mask, bit_settings);
                u64 cast_rook_attacks = raycast_rook_attacks(static_cast<Square>(i), written_settings);

                // **USED FOR MAGIC GENERATION**
                // int idx = static_cast<int>(rook_offsets[i] + ((candidate * written_settings) >> (64 - relevant_bits)));
                // u64 attack_set = rook_attacks[idx];
                // if(attack_set == 0ULL || attack_set == cast_rook_attacks) {
                //     rook_attacks[idx] = cast_rook_attacks;
                //     bit_settings++;
                // }
                // else {
                //     candidate = gen_magic_candidate();
                //     bit_settings = 0U;
                //     std::fill(rook_attacks.begin() + rook_offsets[i], 
                //     rook_attacks.begin() + rook_offsets[i] + (1U << relevant_bits), 
                //     0ULL);
                // }

                int idx = static_cast<int>(rook_offsets[i] + ((rook_magics[i] * written_settings) >> (64 - relevant_bits)));
                rook_attacks[idx] = cast_rook_attacks;
                bit_settings++;
            }
            // rook_magics[i] = candidate;
        }
    }

    u64 index_rook_attacks(Square sq, u64 blocker_board) {
        blocker_board &= get_rook_blocker_mask(sq);
        int idx = static_cast<int>(rook_offsets[sq] + ((rook_magics[sq] * blocker_board) >> (64 - std::popcount(get_rook_blocker_mask(static_cast<Square>(sq)))))); 
        return rook_attacks[idx];
    }


} // namespace move_gen