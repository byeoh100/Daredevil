#include "game.hpp"

void GameBoard::set_piece(Piece piece, Square idx) {
    u64 bit_piece = 1ULL << idx;
    all_pieces[piece] |= bit_piece;
    update_boards();
}

u64 GameBoard::get_piece(Piece piece) const { return all_pieces[piece]; }

void GameBoard::clear_piece(Piece piece, Square sq) {
    all_pieces[piece] &= ~(1ULL << sq);
    update_boards();
}

std::span<const u64> GameBoard::get_all_pieces_view() const {
    return all_pieces;
}

void GameBoard::print() {
    std::array<std::string, 12> piece_label = {"K", "Q", "R", "B", "N", "P",
                                               "k", "q", "r", "b", "n", "p"};

    std::vector<std::vector<std::string>> str_board = {
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"}};

    u64 bits = 0;

    for (size_t i = 0; i < 12; i++) {
        u64 board_state = all_pieces[i];
        bits |= board_state;
        while (board_state != 0ULL) {
            int lsb_index = std::countr_zero(board_state);
            int rank = lsb_index / 8;
            int file = lsb_index % 8;

            str_board[rank][file] = piece_label[i];

            board_state &= (board_state - 1);
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
    for (const auto& piece : all_pieces) {
        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                int square = rank * 8 + file;

                if ((piece >> square) & 1ULL) {
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
void GameBoard::load_from_fen(std::string fen) {
    std::fill(std::begin(all_pieces), std::end(all_pieces), 0);

    std::stringstream ss(fen);
    std::string token;
    std::vector<std::string> fen_tokens;

    white_board = 0;
    black_board = 0;

    while (std::getline(ss, token, ' ')) {
        fen_tokens.push_back(token);
    }

    if (fen_tokens.size() != 6)
        throw std::length_error("Incorrect amount of FEN args.");

    int rank = 7;
    int file = 0;
    for (const char& c : fen_tokens[0]) {
        if (c == '/') {
            rank--;
            file = 0;
            continue;
        }

        if (std::isdigit(c)) {
            file += (c - '0');
        } else {
            Piece piece = char_to_piece(c);
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
        for (const char& c : castle_string) {
            if (c == 'K') castle_rights |= (1U);
            if (c == 'Q') castle_rights |= (1U << 1);
            if (c == 'k') castle_rights |= (1U << 2);
            if (c == 'q') castle_rights |= (1U << 3);
        }
    }

    // en passant = convert ASCII to Square (assuming lowercase)
    // a = 97 -> h = 104, nums are nums
    // square = rank * 8 + file
    const std::string& en_passant_string = fen_tokens[3];
    if (en_passant_string == "-") {
        en_passant_target = NO_SQUARE;
    } else {
        char file_char = en_passant_string[0];
        char rank_char = en_passant_string[1];

        // need validation here
        en_passant_target = ((rank_char - '1') * 8) + (file_char - 'a');
    }

    half_move = std::stoi(fen_tokens[4]);
    full_move = std::stoi(fen_tokens[5]);
}

void GameBoard::init() {
    std::string init_fen = "8/8/8/8/8/8/8/8 w KQkq - 0 1";

    load_from_fen(init_fen);
}

void GameBoard::reset() {
    std::string reset_fen =
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    load_from_fen(reset_fen);
}

void GameBoard::print_fen_status() {
    std::string castle_rights_string = "";
    if (to_move == (to_move & 1U)) castle_rights_string += "K";
    if (to_move == (to_move & 1U << 1)) castle_rights_string += "Q";
    if (to_move == (to_move & 1U << 2)) castle_rights_string += "k";
    if (to_move == (to_move & 1U << 3)) castle_rights_string += "q";

    std::string en_passant_target_string = "";
    if (en_passant_target == NO_SQUARE) {
        en_passant_target_string = "-";
    } else {
        char file_char = (en_passant_target % 8) + 'a';
        char rank_char = (en_passant_target / 8) + '1';
        en_passant_target_string += file_char;
        en_passant_target_string += rank_char;
    }

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

u64 GameBoard::get_occupancy() const { return occupancy_board; }

u64 GameBoard::get_white_board() const { return white_board; }
u64 GameBoard::get_black_board() const { return black_board; }

std::tuple<u64, u64, u64, u64, u64, u64> GameBoard::get_white_pieces() const {
    return {all_pieces[WHITE_PAWN],   all_pieces[WHITE_KNIGHT],
            all_pieces[WHITE_KING],   all_pieces[WHITE_ROOK],
            all_pieces[WHITE_BISHOP], all_pieces[WHITE_QUEEN]};
}

std::tuple<u64, u64, u64, u64, u64, u64> GameBoard::get_black_pieces() const {
    return {all_pieces[BLACK_PAWN],   all_pieces[BLACK_KNIGHT],
            all_pieces[BLACK_KING],   all_pieces[BLACK_ROOK],
            all_pieces[BLACK_BISHOP], all_pieces[BLACK_QUEEN]};
}

Color GameBoard::get_to_move() const { return to_move; }

Square GameBoard::get_en_passant_target() const { return en_passant_target; }

uint8_t GameBoard::get_castle_rights() const { return castle_rights; }

void GameBoard::set_castle_rights(uint8_t cr) { castle_rights = cr; }

bool GameBoard::make_move(std::uint32_t move) {
    GameBoard save = *this;

    Square source = get_move_source(move);
    Square target = get_move_target(move);
    Piece piece = get_move_piece(move);
    MoveFlag flag = get_move_flag(move);

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
        std::uint8_t piece_bits = flag & 0b0011;
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
            for (u64& board_piece : all_pieces) {
                board_piece &= ~(1ULL << target);
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

    int king_sq = (color == WHITE) ? std::countr_zero(all_pieces[WHITE_KING])
                                   : std::countr_zero(all_pieces[BLACK_KING]);

    if (move_gen::is_sq_attacked(king_sq, *this, color)) {
        *this = save;
        return false;
    }

    return true;
}