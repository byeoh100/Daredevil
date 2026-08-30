#include "game.hpp"

void GameBoard::set_piece(Piece piece, int idx) {
    u64 bit_piece = 1ULL << idx;
    for(u64 &board_piece : all_pieces) {
        // if a piece already exists at that spot remove it
        if(board_piece == (board_piece | bit_piece)) board_piece &= ~(bit_piece);
    }

    all_pieces[piece] |= bit_piece;
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

    for(size_t i = 0; i < 12; i++) {
        u64 board_state = all_pieces[i];
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
}

void GameBoard::print_pieces() {
    for(const auto &piece : all_pieces) {
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
                set_piece(piece, rank * 8 + file);
                file++;
            }
        }
    }

    // fen 1-5
}

void GameBoard::reset() {
    std::string reset_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    load_from_fen(reset_fen);
}