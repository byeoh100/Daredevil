#include "game.hpp"

void GameBoard::set_piece(Piece piece, square idx) {
    bitboard bit_piece = 1ULL << idx;
    all_pieces[piece] |= bit_piece;
    update_boards();
}

void GameBoard::clear_piece(Piece piece, square sq) {
    utils::pop_bit(all_pieces[piece], sq);
    update_boards();
}

std::span<const bitboard> GameBoard::get_piece_view() const {
    return all_pieces;
}

void GameBoard::print() {
    constexpr std::array<std::string_view, 12> piece_label = {
        "K", "Q", "R", "B", "N", "P", "k", "q", "r", "b", "n", "p"};

    std::vector<std::vector<std::string>> str_board = {
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"}};

    bitboard bits = 0ULL;

    for (size_t i = 0; i < 12; i++) {
        bitboard board_state = all_pieces[i];
        bits |= board_state;
        while (board_state) {
            square sq = utils::pop_lsb(board_state);
            int rank = sq / 8;
            int file = sq % 8;

            str_board[rank][file] = piece_label[i];
        }
    }

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            std::cout << str_board[rank][file] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
    std::cout << "bits: " << bits;
    std::cout << "\n";
}

void GameBoard::print_pieces() {
    for (bitboard piece : all_pieces) {
        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                square sq = rank * 8 + file;

                if ((piece >> sq) & 1ULL) {
                    std::cout << "1 ";
                } else {
                    std::cout << "_ ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "**********************";
        std::cout << "\n";
    }
}

// fen_tokens[i]:
// 0 = piece placement
// 1 = side to move
// 2 = castling rights
// 3 = en passant square
// 4 = halfmove clock
// 5 = fullmove clock
void GameBoard::load_from_fen(std::string_view fen) {
    std::fill(std::begin(all_pieces), std::end(all_pieces), 0ULL);

    std::stringstream ss{std::string(fen)};
    std::string token;
    std::vector<std::string> fen_tokens;

    white_board = 0ULL;
    black_board = 0ULL;

    while (std::getline(ss, token, ' ')) {
        fen_tokens.push_back(token);
    }

    if (fen_tokens.size() != 6)
        throw std::length_error("Incorrect amount of FEN args.");

    int rank = 7;
    int file = 0;
    for (char c : fen_tokens[0]) {
        if (c == '/') {
            rank--;
            file = 0;
            continue;
        }

        if (std::isdigit(c)) {
            file += (c - '0');
        } else {
            Piece piece = utils::char_to_piece(c);
            if (piece != NO_PIECE) {
                set_piece(piece, rank * 8 + file);
                file++;
            }
        }
    }

    to_move = (fen_tokens[1] == "w") ? WHITE : BLACK;

    // castle_rights = 4 bits in an 8 bit num
    // <---dead 4 bits---> bit 3 | bit 2 | bit 1 | bit 0
    //                       q       k       Q       K
    castle_rights = 0;
    const std::string& castle_string = fen_tokens[2];
    if (castle_string != "-") {
        for (char c : castle_string) {
            if (c == 'K') castle_rights |= WHITE_KINGSIDE;
            if (c == 'Q') castle_rights |= WHITE_QUEENSIDE;
            if (c == 'k') castle_rights |= BLACK_KINGSIDE;
            if (c == 'q') castle_rights |= BLACK_QUEENSIDE;
        }
    }

    // need validation here
    en_passant_target = utils::algebraic_to_square(fen_tokens[3]);

    half_move = std::stoi(fen_tokens[4]);
    full_move = std::stoi(fen_tokens[5]);
}

void GameBoard::init() {
    constexpr std::string_view init_fen = "8/8/8/8/8/8/8/8 w KQkq - 0 1";

    load_from_fen(init_fen);
}

void GameBoard::reset() {
    constexpr std::string_view reset_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    load_from_fen(reset_fen);
}

void GameBoard::print_fen_status() {
    std::string castle_rights_string = "";
    if (castle_rights & WHITE_KINGSIDE) castle_rights_string += "K";
    if (castle_rights & WHITE_QUEENSIDE) castle_rights_string += "Q";
    if (castle_rights & BLACK_KINGSIDE) castle_rights_string += "k";
    if (castle_rights & BLACK_QUEENSIDE) castle_rights_string += "q";
    if (castle_rights_string.empty()) castle_rights_string = "-";

    std::string en_passant_target_string =
        utils::square_to_algebraic(en_passant_target);

    print();
    std::cout << "To move: " << ((to_move == WHITE) ? "white" : "black");
    std::cout << "\n";

    std::cout << "Castling rights: " << castle_rights_string;
    std::cout << "\n";

    std::cout << "En passant square: " << en_passant_target_string;
    std::cout << "\n";

    std::cout << "Half move clock: " << half_move;
    std::cout << "\n";

    std::cout << "Full move clock: " << full_move;
    std::cout << "\n";
}

void GameBoard::update_boards() {
    white_board = 0ULL;
    black_board = 0ULL;
    for (int piece = 0; piece < 6; piece++) white_board |= all_pieces[piece];
    for (int piece = 6; piece < 12; piece++) black_board |= all_pieces[piece];
    occupancy_board = white_board | black_board;
}

std::tuple<bitboard, bitboard, bitboard, bitboard, bitboard, bitboard>
GameBoard::get_piece_set(Color color) const {
    if (color == WHITE) {
        return {all_pieces[WHITE_KING],   all_pieces[WHITE_QUEEN],
                all_pieces[WHITE_ROOK],   all_pieces[WHITE_BISHOP],
                all_pieces[WHITE_KNIGHT], all_pieces[WHITE_PAWN]};
    } else {
        return {all_pieces[BLACK_KING],   all_pieces[BLACK_QUEEN],
                all_pieces[BLACK_ROOK],   all_pieces[BLACK_BISHOP],
                all_pieces[BLACK_KNIGHT], all_pieces[BLACK_PAWN]};
    }
}

void GameBoard::set_castle_rights(u8 cr) { castle_rights = cr; }

bool GameBoard::make_move(u32 move) {
    GameBoard save = *this;

    square source = encoder::get_move_source(move);
    square target = encoder::get_move_target(move);
    Piece piece = encoder::get_move_piece(move);
    MoveFlag flag = encoder::get_move_flag(move);

    // for promotions
    Piece new_piece = piece;

    en_passant_target = NO_SQUARE;
    half_move++;
    if (to_move == BLACK) full_move++;

    if (flag == CASTLE_KINGSIDE) {
        // manually moving rooks and clear castle right
        if (to_move == WHITE) {
            clear_piece(WHITE_ROOK, H1);
            set_piece(WHITE_ROOK, F1);
        } else {
            clear_piece(BLACK_ROOK, H8);
            set_piece(BLACK_ROOK, F8);
        }
    } else if (flag == CASTLE_QUEENSIDE) {
        if (to_move == WHITE) {
            clear_piece(WHITE_ROOK, A1);
            set_piece(WHITE_ROOK, D1);
        } else {
            clear_piece(BLACK_ROOK, A8);
            set_piece(BLACK_ROOK, D8);
        }
    }

    if (flag & PROMOTION_BIT) {
        u8 piece_bits = flag & 0b0011;
        switch (piece_bits) {
            case 0:
                new_piece = (to_move == WHITE) ? WHITE_KNIGHT : BLACK_KNIGHT;
                break;
            case 1:
                new_piece = (to_move == WHITE) ? WHITE_BISHOP : BLACK_BISHOP;
                break;
            case 2:
                new_piece = (to_move == WHITE) ? WHITE_ROOK : BLACK_ROOK;
                break;
            case 3:
                new_piece = (to_move == WHITE) ? WHITE_QUEEN : BLACK_QUEEN;
                break;
        }
    }

    if (flag & CAPTURE_BIT) {
        // handle en passant
        if (flag == EN_PASSANT) {
            (to_move == WHITE) ? clear_piece(BLACK_PAWN, target + SOUTH)
                               : clear_piece(WHITE_PAWN, target + NORTH);
        } else {
            // remove captured piece
            for (bitboard& board_piece : all_pieces) {
                utils::pop_bit(board_piece, target);
            }
        }
        half_move = 0;
    }

    if (piece == WHITE_PAWN || piece == BLACK_PAWN) {
        if (flag == DOUBLE_MOVE)
            en_passant_target =
                (piece == WHITE_PAWN) ? source + NORTH : source + SOUTH;

        half_move = 0;
    }

    // move the castle rights clearing from the castling branch
    if (piece == WHITE_KING &&
        (castle_rights & (WHITE_KINGSIDE | WHITE_QUEENSIDE)))
        castle_rights &= ~(WHITE_KINGSIDE) & ~(WHITE_QUEENSIDE);
    if (piece == BLACK_KING &&
        (castle_rights & (BLACK_KINGSIDE | BLACK_QUEENSIDE)))
        castle_rights &= ~(BLACK_KINGSIDE) & ~(BLACK_QUEENSIDE);

    // did a home square rook move
    if (piece == WHITE_ROOK) {
        if (source == H1) castle_rights &= ~WHITE_KINGSIDE;
        if (source == A1) castle_rights &= ~WHITE_QUEENSIDE;
    } else if (piece == BLACK_ROOK) {
        if (source == H8) castle_rights &= ~BLACK_KINGSIDE;
        if (source == A8) castle_rights &= ~BLACK_QUEENSIDE;
    }

    // were any home square rooks recaptured
    if (target == H1) castle_rights &= ~WHITE_KINGSIDE;
    if (target == A1) castle_rights &= ~WHITE_QUEENSIDE;
    if (target == H8) castle_rights &= ~BLACK_KINGSIDE;
    if (target == A8) castle_rights &= ~BLACK_QUEENSIDE;

    clear_piece(piece, source);
    set_piece(new_piece, target);

    Color color = to_move;
    to_move = (to_move == WHITE) ? BLACK : WHITE;

    square king_sq = (color == WHITE)
                         ? std::countr_zero(all_pieces[WHITE_KING])
                         : std::countr_zero(all_pieces[BLACK_KING]);

    if (movegen::is_sq_attacked(king_sq, *this, color)) {
        *this = save;
        return false;
    }

    return true;
}