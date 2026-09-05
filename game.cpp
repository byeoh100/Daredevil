#include "game.hpp"

void GameBoard::set_piece(Piece piece, Square idx) {
    u64 bit_piece = 1ULL << idx;
    for(u64 &board_piece : all_pieces) {
        // if a piece already exists at that spot remove it
        if(board_piece == (board_piece | bit_piece)) board_piece &= ~(bit_piece);
    }

    all_pieces[piece] |= bit_piece;
}

u64 GameBoard::get_piece(Piece piece) {
    return all_pieces[piece];
}

void GameBoard::print() {
    std::array<std::string, 12> piece_label = {"K", "Q", "R", "B", "N", "P", "k", "q", "r", "b", "n", "p"};

    std::vector<std::vector<std::string>> str_board = {
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"},
        {"_", "_", "_", "_", "_", "_", "_", "_"}
    };

    u64 bits = 0;

    for(size_t i = 0; i < 12; i++) {
        u64 board_state = all_pieces[i];
        bits |= board_state;
        while(board_state != 0ULL) {
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
    for(const auto &piece : all_pieces) {
        for(int rank = 7; rank >= 0; rank--) {
            for(int file = 0; file < 8; file++) {
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

    while(std::getline(ss, token, ' ')) {
        fen_tokens.push_back(token);
    }

    if(fen_tokens.size() != 6) throw std::length_error("Incorrect amount of FEN args.");

    int rank = 7;
    int file = 0;
    for(const char &c : fen_tokens[0]) {
        if(c == '/') {
            rank--;
            file = 0;
            continue;
        }

        if(std::isdigit(c)) {
            file += (c - '0');
        }
        else {
            Piece piece = char_to_piece(c);
            if(piece != Piece::NO_PIECE) {
                set_piece(piece, static_cast<Square>(rank * 8 + file));
                file++;
            }
        }
    }

    to_move = (fen_tokens[1] == "w") ? Color::WHITE : Color::BLACK;

    // castle_rights = 4 bits in an 8 bit num
    // <---dead 4 bits---> bit 3 | bit 2 | bit 1 | bit 0
    //                       q       k       Q       K
    castle_rights = 0;
    const std::string& castle_string = fen_tokens[2];
    if(castle_string != "-") {
        for(const char& c : castle_string) {
            if(c == 'K') castle_rights |= (1U);
            if(c == 'Q') castle_rights |= (1U << 1);
            if(c == 'k') castle_rights |= (1U << 2);
            if(c == 'q') castle_rights |= (1U << 3);
        }
    }

    // en passant = convert ASCII to Square (assuming lowercase)
    // a = 97 -> h = 104, nums are nums
    // square = rank * 8 + file
    const std::string& en_passant_string = fen_tokens[3];
    if(en_passant_string == "-") {
        en_passant_target = Square::NO_SQUARE;
    }
    else {
        char file_char = en_passant_string[0];
        char rank_char = en_passant_string[1];

        // need validation here
        en_passant_target = static_cast<Square>(((rank_char - '1') * 8) + (file_char - 'a'));
    }

    half_move = std::stoi(fen_tokens[4]);
    full_move = std::stoi(fen_tokens[5]);

}

void GameBoard::init() {
    std::string init_fen = "8/8/8/8/8/8/8/8 w KQkq - 0 1";

    load_from_fen(init_fen);
}

void GameBoard::reset() {
    std::string reset_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    load_from_fen(reset_fen);
}

void GameBoard::print_fen_status() {
    std::string castle_rights_string = "";
    if(to_move == (to_move & 1U)) castle_rights_string += "K";
    if(to_move == (to_move & 1U << 1)) castle_rights_string += "Q";
    if(to_move == (to_move & 1U << 2)) castle_rights_string += "k";
    if(to_move == (to_move & 1U << 3)) castle_rights_string += "q";

    std::string en_passant_target_string = "";
    if(en_passant_target == Square::NO_SQUARE) {
        en_passant_target_string = "-";
    }
    else {
        char file_char = (en_passant_target % 8) + 'a';
        char rank_char = (en_passant_target / 8) + '1';
        en_passant_target_string += file_char;
        en_passant_target_string += rank_char;
    }
    
    print();
    std::cout << "To move: " << ((to_move == Color::WHITE) ? "white" : "black");
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

// TODO FOR TMRW:
// get bits for FILE_A in types by manually getting them with print funcs
// use that to make the file_masks
// do the same for ranks to get rank_masks
// take that to create the pawn masks because you need to bound the left and right walls
// take the pawn masks and pregen the pawn attacks lookup table
// test