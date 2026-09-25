#include "movegen.hpp"

namespace movegen {
// All pre-generated tables are kept hidden
namespace {
std::array<bitboard, 64> king_attacks;

// array for rook attacks
// move possibilities corner/edge/interior:
// 4*2^12 + 6*2^11 + 36*2^10 sized = 102,400
std::array<bitboard, 102400> rook_attacks{};
std::array<bitboard, 64> rook_blockers{};
std::array<bitboard, 64> rook_blocker_bits{};
constexpr std::array<u64, 64> rook_magics{
    // hardcoded magics generated with init
    612489663141167106ULL,  36063982465110144ULL,    72066666367836162ULL,
    72066407857127680ULL,   10520410931393208336ULL, 36030998189965313ULL,
    72063091604586500ULL,   14051232214234964224ULL, 144255963146321952ULL,
    90353742410490112ULL,   1441293168271187968ULL,  36732553205651456ULL,
    2307109680969941120ULL, 577023771009879048ULL,   146929959351354376ULL,
    1153484455651583106ULL, 141287248363553ULL,      8101978203542782024ULL,
    72200531893944384ULL,   9799871272216170496ULL,  864973703045195776ULL,
    18155685820826112ULL,   2522020189391816770ULL,  3460666668945252417ULL,
    141173427552266ULL,     4679523639493768ULL,     145522876493078528ULL,
    4902274851316695048ULL, 9314156517232218368ULL,  562958544405512ULL,
    5066558170826756ULL,    11538258537797141668ULL, 141012374654048ULL,
    18023471669846016ULL,   1171499128518086788ULL,  10448360000328835328ULL,
    4688810404275556384ULL, 6090133342198440961ULL,  2307250710699317760ULL,
    9029329107288129ULL,    587860489933979684ULL,   585468157787914242ULL,
    9007757868957732ULL,    145277852919595016ULL,   1225542083495198736ULL,
    563019210032128ULL,     18023263365758981ULL,    36733586124177412ULL,
    127262423605314944ULL,  35186859311232ULL,       153124655075459200ULL,
    140943648358528ULL,     6958202178956165504ULL,  563018807412224ULL,
    1155331737543312384ULL, 4683884351044192384ULL,  9223653581188792386ULL,
    72093057651081730ULL,   175926156528833ULL,      168924637165003009ULL,
    2814767217510402ULL,    577023775542363142ULL,   565222260647940ULL,
    576461306499170374ULL};
std::array<int, 64> rook_offsets;

// array for bishop attacks
// move possibilities(bottom left of square) center(d4)/inner ring(c3)/outer
// ring(b2) + edge/corner: 4*2^9 + 12*2^7 + 44*2^5 + 4*2^6 = 5248
std::array<bitboard, 5248> bishop_attacks{};
std::array<bitboard, 64> bishop_blockers{};
std::array<bitboard, 64> bishop_blocker_bits{};
constexpr std::array<u64, 64> bishop_magics{
    722308776141258816ULL,   13836263130780532752ULL, 4516796185315344ULL,
    6830728872466432ULL,     723958175624103232ULL,   72356670059053056ULL,
    576746630159008264ULL,   1080952219526250496ULL,  4693041366526726657ULL,
    9513858783772934306ULL,  4611976392714911744ULL,  83580072018434ULL,
    92514141866240ULL,       10956136597825257600ULL, 2305862802873008326ULL,
    1154083155888710148ULL,  76631579656996928ULL,    145241191624484354ULL,
    454864215744716808ULL,   4629842288432971906ULL,  9260526873444630784ULL,
    35188677545984ULL,       19703559789478408ULL,    577023703348150532ULL,
    3448094771904778ULL,     624557510111385ULL,      1171015069111290882ULL,
    1197093351916032ULL,     2986273589436416ULL,     9809025810206573065ULL,
    595128263274336256ULL,   578998571173676080ULL,   2454748015754223808ULL,
    37192217897075712ULL,    74835552239714ULL,       2314870001825742976ULL,
    4504157974176256ULL,     466235890352384ULL,      1134702442923072ULL,
    919439150700167680ULL,   7892332046353408ULL,     9244216651375714304ULL,
    9227031282687455744ULL,  18586428157984896ULL,    578184941424083200ULL,
    11821966682804453920ULL, 1441726117902812168ULL,  4904424944174240128ULL,
    3468361266090741003ULL,  1225544387275784640ULL,  9367487826304176136ULL,
    293019988456112416ULL,   144132849150590993ULL,   185405282746385ULL,
    2596351643318502080ULL,  73262660998694192ULL,    1190357952530233344ULL,
    10415690842641408ULL,    9367487255062596608ULL,  2315976108530862080ULL,
    2305843046799018500ULL,  1233986316692357396ULL,  6927178376314651168ULL,
    1299297322957552128ULL};
std::array<int, 64> bishop_offsets;

std::array<bitboard, 64> knight_attacks;

std::array<bitboard, 64> white_pawn_attacks;
std::array<bitboard, 64> black_pawn_attacks;

// Helpers
// u64 gen_magic_candidate() {
//     std::random_device rd;
//     std::mt19937_64 gen(rd());
//     std::uniform_int_distribution<u64> dist(0,
//     std::numeric_limits<u64>::max());

//     return dist(gen) & dist(gen) & dist(gen);
// }

void init_rook_blockers() {
    for (square sq = 0; sq < 64; sq++) {
        int rank = sq / 8;
        int file = sq % 8;

        bitboard rook_board = 1ULL << sq;
        bitboard raw_attacks =
            (file_masks[file] | rank_masks[rank]) & ~(rook_board);
        bitboard blocker_mask;

        if (rook_board & CORNER_SQUARES) {
            blocker_mask = raw_attacks & EDGE_SQUARES;
        } else if (rook_board & EDGE_SQUARES) {
            blocker_mask = raw_attacks & ~(CORNER_SQUARES);
            if (rank == 0) blocker_mask &= ~rank_masks[RANK_8];
            if (rank == 7) blocker_mask &= ~rank_masks[RANK_1];
            if (file == 0) blocker_mask &= ~file_masks[FILE_H];
            if (file == 7) blocker_mask &= ~file_masks[FILE_A];
        } else {
            blocker_mask = raw_attacks & ~(EDGE_SQUARES);
        }

        rook_blockers[sq] = blocker_mask;
        rook_blocker_bits[sq] = 64 - std::popcount(blocker_mask);
    }
}

void init_rook_offsets() {
    int offset = 0;

    for (square sq = 0; sq < 64; sq++) {
        rook_offsets[sq] = offset;
        bitboard rook_board = 1ULL << sq;
        if (rook_board & CORNER_SQUARES) {
            offset += (1 << 12);
        } else if (rook_board & EDGE_SQUARES) {
            offset += (1 << 11);
        } else {
            offset += (1 << 10);
        }
    }
}

void init_bishop_offsets() {
    int offset = 0;
    constexpr bitboard outer_ring = (file_masks[FILE_B] | file_masks[FILE_G] |
                                     rank_masks[RANK_2] | rank_masks[RANK_7]) &
                                    ~(BOARD_EDGE);
    constexpr bitboard inner_ring = ((file_masks[FILE_C] | file_masks[FILE_F] |
                                      rank_masks[RANK_3] | rank_masks[RANK_6]) &
                                     ~(BOARD_EDGE)) &
                                    ~(outer_ring);
    constexpr bitboard center = (((file_masks[FILE_D] | file_masks[FILE_E] |
                                   rank_masks[RANK_4] | rank_masks[RANK_5]) &
                                  ~(BOARD_EDGE)) &
                                 ~(outer_ring)) &
                                ~(inner_ring);

    for (square sq = 0; sq < 64; sq++) {
        bishop_offsets[sq] = offset;
        bitboard bishop_board = 1ULL << sq;
        if (bishop_board & CORNER_SQUARES) {
            offset += (1 << 6);
        } else if (bishop_board & (EDGE_SQUARES | outer_ring)) {
            offset += (1 << 5);
        } else if (bishop_board & inner_ring) {
            offset += (1 << 7);
        } else if (bishop_board & center) {
            offset += (1 << 9);
        }
    }
}

void init_bishop_blockers() {
    for (square sq = 0; sq < 64; sq++) {
        int rank = sq / 8;
        int file = sq % 8;

        bitboard bishop_board = 1ULL << sq;
        int diag = 7 + rank - file;
        int antidiag = rank + file;

        bitboard raw_attacks =
            (diagonal_masks[diag] | antidiagonal_masks[antidiag]) & ~(bishop_board);

        bishop_blockers[sq] = raw_attacks & ~(CORNER_SQUARES) & ~(EDGE_SQUARES);
        bishop_blocker_bits[sq] = 64 - std::popcount(bishop_blockers[sq]);
    }
}

bitboard fire_ray(bitboard start, bitboard blocker, Direction dir) {
    bitboard ray = 0ULL;
    while (start) {
        start = types::shift(start, dir);
        ray |= start;
        if ((start | blocker) == blocker) break;
    }
    return ray;
}

}  // namespace

void init_all_pieces() {
    init_king_attacks_array();
    init_rook_attacks_array();
    init_bishop_attacks_array();
    init_knight_attacks_array();
    init_pawn_attacks_array();
}

void init_king_attacks_array() {
    for (square sq = 0; sq < 64; sq++) {
        bitboard single_king_board = (1ULL << sq);
        king_attacks[sq] = mask_king_attacks(single_king_board);
    }
}

bitboard get_king_attack(square sq) { return king_attacks[sq]; }

bitboard get_queen_attack(square sq, bitboard blocker_board) {
    return get_rook_attack(sq, blocker_board) |
           get_bishop_attack(sq, blocker_board);
}

bitboard raycast_rook_attacks(square sq, bitboard blocker_board) {
    int rank = sq / 8;
    int file = sq % 8;
    bitboard cross_check = rank_masks[rank] | file_masks[file];

    bitboard target_board = 1ULL << sq;
    if ((target_board | cross_check) != (blocker_board | cross_check)) {
        throw std::invalid_argument(
            "raycast_rook_attacks() - invalid blocker_board");
    }
    return (fire_ray(target_board, blocker_board, NORTH) |
            fire_ray(target_board, blocker_board, EAST) |
            fire_ray(target_board, blocker_board, SOUTH) |
            fire_ray(target_board, blocker_board, WEST));
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
    init_rook_blockers();
    auto write_bit_settings = [](bitboard mask, int bits) {
        bitboard output = 0ULL;
        while (mask) {
            square sq = utils::pop_lsb(mask);
            if (bits & 1) {
                output |= 1ULL << sq;
            }
            bits >>= 1;
        }

        return output;
    };

    for (square sq = 0; sq < 64; sq++) {
        bitboard blocker_mask = rook_blockers[sq];

        int relevant_bits = std::popcount(blocker_mask);
        // u64 candidate = gen_magic_candidate();
        int bit_settings = 0;

        while (bit_settings < (1 << relevant_bits)) {
            bitboard written_settings =
                write_bit_settings(blocker_mask, bit_settings);
            bitboard cast_rook_attacks =
                raycast_rook_attacks(sq, written_settings);

            // **USED FOR MAGIC GENERATION**
            // int idx = static_cast<int>(rook_offsets[sq] + ((candidate *
            // written_settings) >> (64 - relevant_bits))); bitboard attack_set
            // = rook_attacks[idx]; if(attack_set == 0ULL || attack_set ==
            // cast_rook_attacks) {
            //     rook_attacks[idx] = cast_rook_attacks;
            //     bit_settings++;
            // }
            // else {
            //     candidate = gen_magic_candidate();
            //     bit_settings = 0;
            //     std::fill(rook_attacks.begin() + rook_offsets[sq],
            //     rook_attacks.begin() + rook_offsets[sq] + (1 <<
            //     relevant_bits), 0ULL);
            // }

            int idx = static_cast<int>(
                rook_offsets[sq] +
                ((rook_magics[sq] * written_settings) >> (64 - relevant_bits)));
            rook_attacks[idx] = cast_rook_attacks;
            bit_settings++;
        }
        // rook_magics[sq] = candidate;
    }
}

bitboard get_rook_attack(square sq, bitboard blocker_board) {
    blocker_board &= rook_blockers[sq];
    int idx =
        (rook_offsets[sq] + ((rook_magics[sq] * blocker_board) >>
                             rook_blocker_bits[sq]));
    return rook_attacks[idx];
}

bitboard raycast_bishop_attacks(square sq, bitboard blocker_board) {
    int rank = sq / 8;
    int file = sq % 8;
    int diag = 7 + rank - file;
    int antidiag = rank + file;
    bitboard cross_check = diagonal_masks[diag] | antidiagonal_masks[antidiag];

    bitboard target_board = 1ULL << sq;
    if ((target_board | cross_check) != (blocker_board | cross_check)) {
        throw std::invalid_argument(
            "raycast_bishop_attacks() - invalid blocker_board");
    }
    return (fire_ray(target_board, blocker_board, NORTHEAST) |
            fire_ray(target_board, blocker_board, SOUTHEAST) |
            fire_ray(target_board, blocker_board, SOUTHWEST) |
            fire_ray(target_board, blocker_board, NORTHWEST));
}

void init_bishop_attacks_array() {
    init_bishop_offsets();
    init_bishop_blockers();
    auto write_bit_settings = [](bitboard mask, int bits) {
        bitboard output = 0ULL;
        while (mask) {
            square sq = utils::pop_lsb(mask);
            if (bits & 1) {
                output |= 1ULL << sq;
            }
            bits >>= 1;
        }

        return output;
    };

    for (square sq = 0; sq < 64; sq++) {
        bitboard blocker_mask = bishop_blockers[sq];

        int relevant_bits = std::popcount(blocker_mask);
        // u64 candidate = gen_magic_candidate();
        int bit_settings = 0;

        while (bit_settings < (1 << relevant_bits)) {
            bitboard written_settings =
                write_bit_settings(blocker_mask, bit_settings);
            bitboard cast_bishop_attacks =
                raycast_bishop_attacks(sq, written_settings);

            // **USED FOR MAGIC GENERATION**
            // int idx = static_cast<int>(bishop_offsets[sq] + ((candidate *
            // written_settings) >> (64 - relevant_bits))); bitboard attack_set
            // = bishop_attacks[idx]; if(attack_set == 0ULL || attack_set ==
            // cast_bishop_attacks) {
            //     bishop_attacks[idx] = cast_bishop_attacks;
            //     bit_settings++;
            // }
            // else {
            //     candidate = gen_magic_candidate();
            //     bit_settings = 0;
            //     std::fill(bishop_attacks.begin() + bishop_offsets[sq],
            //     bishop_attacks.begin() + bishop_offsets[sq] + (1 <<
            //     relevant_bits), 0ULL);
            // }

            int idx = static_cast<int>(
                bishop_offsets[sq] + ((bishop_magics[sq] * written_settings) >>
                                      (64 - relevant_bits)));
            bishop_attacks[idx] = cast_bishop_attacks;
            bit_settings++;
        }
        // bishop_magics[sq] = candidate;
    }
}

bitboard get_bishop_attack(square sq, bitboard blocker_board) {
    blocker_board &= bishop_blockers[sq];
    int idx = (bishop_offsets[sq] +
               ((bishop_magics[sq] * blocker_board) >>
                bishop_blocker_bits[sq]));
    return bishop_attacks[idx];
}

void init_knight_attacks_array() {
    for (square sq = 0; sq < 64; sq++) {
        bitboard single_knight_board = (1ULL << sq);
        knight_attacks[sq] = mask_knight_attacks(single_knight_board);
    }
}

bitboard get_knight_attack(square sq) { return knight_attacks[sq]; }

void init_pawn_attacks_array() {
    for (square sq = 0; sq < 64; sq++) {
        bitboard single_pawn_board = (1ULL << sq);
        white_pawn_attacks[sq] = mask_pawn_attacks(single_pawn_board, WHITE);
        black_pawn_attacks[sq] = mask_pawn_attacks(single_pawn_board, BLACK);
    }
}

bitboard get_pawn_attack(square sq, Color color) {
    if (color == WHITE) {
        return white_pawn_attacks[sq];
    } else {
        return black_pawn_attacks[sq];
    }
}

// --------------------------------
// @@@ IN-GAME MOVE GENERATION @@@
// --------------------------------

void generate_moves(const GameBoard& game_board, MoveList& move_list) {
    move_list.count = 0;

    bitboard blocker_board = game_board.get_occupancy();
    Color color = game_board.get_to_move();

    // Move encoding data
    square source;
    square target;

    // Get the pieces that can move
    auto [king, queen, rook, bishop, knight, pawn] =
        game_board.get_piece_set(color);

    auto friendly_pieces = (color == WHITE) ? game_board.get_white_board()
                                            : game_board.get_black_board();

    auto enemy_pieces = (color == WHITE) ? game_board.get_black_board()
                                         : game_board.get_white_board();

    // king already has explicit no move into check
    // handle illegal moves with take backs for now
    // if any king moves update the castling flags
    while (king) {
        Piece king_color = (color == WHITE) ? WHITE_KING : BLACK_KING;
        square home_square = (color == WHITE) ? E1 : E8;
        source = utils::pop_lsb(king);

        u8 castle_rights = game_board.get_castle_rights();
        int kingside_castle = (color == WHITE)
                                  ? (castle_rights & WHITE_KINGSIDE)
                                  : (castle_rights & BLACK_KINGSIDE);
        int queenside_castle = (color == WHITE)
                                   ? (castle_rights & WHITE_QUEENSIDE)
                                   : (castle_rights & BLACK_QUEENSIDE);
        bitboard kingside_mask =
            (1ULL << (source + EAST)) | (1ULL << (source + 2 * EAST));
        bitboard queenside_mask = (1ULL << (source + WEST)) |
                                  (1ULL << (source + 2 * WEST)) |
                                  (1ULL << (source + 3 * WEST));

        if (source == home_square) {
            if (kingside_castle && !is_sq_attacked(source, game_board, color) &&
                !is_sq_attacked(source + EAST, game_board, color) &&
                !is_sq_attacked(source + 2 * EAST, game_board, color) &&
                !(kingside_mask & blocker_board)
                // perhaps add rook validation
            )
                move_list.push(encoder::encode_move(
                    source, source + 2 * EAST, king_color, CASTLE_KINGSIDE));
            if (queenside_castle &&
                !is_sq_attacked(source, game_board, color) &&
                !is_sq_attacked(source + WEST, game_board, color) &&
                !is_sq_attacked(source + 2 * WEST, game_board, color) &&
                !(queenside_mask & blocker_board))
                move_list.push(encoder::encode_move(
                    source, source + 2 * WEST, king_color, CASTLE_QUEENSIDE));
        }

        bitboard curr_king_attacks = king_attacks[source];
        while (curr_king_attacks) {
            target = utils::pop_lsb(curr_king_attacks);
            if (!is_sq_attacked(target, game_board, color)) {
                if ((1ULL << target) & enemy_pieces) {
                    move_list.push(encoder::encode_move(source, target,
                                                        king_color, CAPTURE));
                } else if (!((1ULL << target) & friendly_pieces)) {
                    move_list.push(encoder::encode_move(
                        source, target, king_color, QUIET_MOVE));
                }
            }
        }
    }

    // slider moves straightforward
    while (queen) {
        Piece queen_color = (color == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;

        source = utils::pop_lsb(queen);
        bitboard curr_queen_attacks = get_queen_attack(source, blocker_board);
        while (curr_queen_attacks) {
            target = utils::pop_lsb(curr_queen_attacks);
            if ((1ULL << target) & enemy_pieces) {
                move_list.push(
                    encoder::encode_move(source, target, queen_color, CAPTURE));
            } else if (!((1ULL << target) & friendly_pieces)) {
                move_list.push(encoder::encode_move(source, target, queen_color,
                                                    QUIET_MOVE));
            }
        }
    }
    while (rook) {
        Piece rook_color = (color == WHITE) ? WHITE_ROOK : BLACK_ROOK;

        source = utils::pop_lsb(rook);
        bitboard curr_rook_attacks = get_rook_attack(source, blocker_board);
        while (curr_rook_attacks) {
            target = utils::pop_lsb(curr_rook_attacks);
            if ((1ULL << target) & enemy_pieces) {
                move_list.push(
                    encoder::encode_move(source, target, rook_color, CAPTURE));
            } else if (!((1ULL << target) & friendly_pieces)) {
                move_list.push(encoder::encode_move(source, target, rook_color,
                                                    QUIET_MOVE));
            }
        }
    }
    while (bishop) {
        Piece bishop_color = (color == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;

        source = utils::pop_lsb(bishop);
        bitboard curr_bishop_attacks = get_bishop_attack(source, blocker_board);
        while (curr_bishop_attacks) {
            target = utils::pop_lsb(curr_bishop_attacks);
            if ((1ULL << target) & enemy_pieces) {
                move_list.push(encoder::encode_move(source, target,
                                                    bishop_color, CAPTURE));
            } else if (!((1ULL << target) & friendly_pieces)) {
                move_list.push(encoder::encode_move(source, target,
                                                    bishop_color, QUIET_MOVE));
            }
        }
    }

    // knight straightforward
    while (knight) {
        Piece knight_color = (color == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;

        source = utils::pop_lsb(knight);
        bitboard curr_knight_attacks = knight_attacks[source];
        while (curr_knight_attacks) {
            target = utils::pop_lsb(curr_knight_attacks);
            if ((1ULL << target) & enemy_pieces) {
                move_list.push(encoder::encode_move(source, target,
                                                    knight_color, CAPTURE));
            } else if (!((1ULL << target) & friendly_pieces)) {
                move_list.push(encoder::encode_move(source, target,
                                                    knight_color, QUIET_MOVE));
            }
        }
    }

    // pawn moves
    // update en passant sq if double move
    // consider en passant sq if available
    while (pawn) {
        source = utils::pop_lsb(pawn);
        int push_amt;
        square double_lower;
        square double_upper;
        square promo_lower;
        square promo_upper;
        Piece pawn_color;

        if (color == WHITE) {
            push_amt = NORTH;
            double_lower = A2;
            double_upper = H2;
            promo_lower = A8;
            promo_upper = H8;
            pawn_color = WHITE_PAWN;
        } else {
            push_amt = SOUTH;
            double_lower = A7;
            double_upper = H7;
            promo_lower = A1;
            promo_upper = H1;
            pawn_color = BLACK_PAWN;
        }

        target = source + push_amt;

        if (!((1ULL << target) & blocker_board)) {
            // Promotion
            if (target >= promo_lower && target <= promo_upper) {
                for (MoveFlag promo :
                     {PROMOTION_N, PROMOTION_B, PROMOTION_R, PROMOTION_Q})
                    move_list.push(encoder::encode_move(source, target,
                                                        pawn_color, promo));
            }
            // Single push
            else {
                move_list.push(encoder::encode_move(source, target, pawn_color,
                                                    QUIET_MOVE));
            }

            // Double push
            if (source >= double_lower && source <= double_upper &&
                !((1ULL << (target + push_amt)) & blocker_board)) {
                move_list.push(encoder::encode_move(source, target + push_amt,
                                                    pawn_color, DOUBLE_MOVE));
            }
        }

        // Attacks
        bitboard curr_pawn_attacks = get_pawn_attack(source, color);
        while (curr_pawn_attacks) {
            target = utils::pop_lsb(curr_pawn_attacks);
            if ((1ULL << target) & enemy_pieces) {
                if (target >= promo_lower && target <= promo_upper) {
                    for (MoveFlag promo : {CAPTURE_PROMO_N, CAPTURE_PROMO_B,
                                           CAPTURE_PROMO_R, CAPTURE_PROMO_Q})
                        move_list.push(encoder::encode_move(source, target,
                                                            pawn_color, promo));
                } else {
                    move_list.push(encoder::encode_move(source, target,
                                                        pawn_color, CAPTURE));
                }
            } else if (game_board.get_en_passant_target() != NO_SQUARE &&
                       target == game_board.get_en_passant_target()) {
                move_list.push(encoder::encode_move(source, target, pawn_color,
                                                    EN_PASSANT));
            }
        }
    }
}

bool is_sq_attacked(square sq, const GameBoard& game_board, Color color) {
    bitboard blocker_board = game_board.get_occupancy();

    // Get the pieces attacking COLOR
    auto [king, queen, rook, bishop, knight, pawn] =
        game_board.get_piece_set(color == WHITE ? BLACK : WHITE);

    if (
        // rewrite to just index the array directly
        king_attacks[sq] & king ||
        get_queen_attack(sq, blocker_board) & queen ||
        get_rook_attack(sq, blocker_board) & rook ||
        get_bishop_attack(sq, blocker_board) & bishop ||
        knight_attacks[sq] & knight || get_pawn_attack(sq, color) & pawn)
        return true;

    return false;
}

bitboard get_legal_king_attack(square sq, const GameBoard& game_board,
                               Color color) {
    bitboard candidates = king_attacks[sq];
    bitboard legal = 0ULL;
    while (candidates) {
        square target = utils::pop_lsb(candidates);
        if (!is_sq_attacked(target, game_board, color)) legal |= 1ULL << target;
    }

    // missing logic for behind the king

    return legal;
}
}  // namespace movegen