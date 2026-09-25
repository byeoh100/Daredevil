#pragma once

#include <algorithm>
#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <span>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "movegen.hpp"
#include "moves.hpp"
#include "types.hpp"
#include "utils.hpp"

class GameBoard {
private:
    Color to_move;
    std::array<bitboard, 12> all_pieces{0ULL};
    u8 castle_rights;
    square en_passant_target;
    int half_move;
    int full_move;
    bitboard white_board;
    bitboard black_board;
    bitboard occupancy_board;

public:
    Color get_to_move() const;

    void set_piece(Piece piece, square board_idx);
    bitboard get_piece(Piece piece) const;
    void clear_piece(Piece piece, square sq);

    std::span<const bitboard> get_piece_view() const;

    u8 get_castle_rights() const;
    void set_castle_rights(u8 cr);

    square get_en_passant_target() const;

    void update_boards();
    bitboard get_occupancy() const;
    bitboard get_white_board() const;
    bitboard get_black_board() const;
    std::tuple<bitboard, bitboard, bitboard, bitboard, bitboard, bitboard>
    get_piece_set(Color color) const;

    bool make_move(u32 move);

    void init();
    void reset();
    void load_from_fen(std::string_view fen);

    void print();
    void print_pieces();  // make it print an individual piece?
    void print_fen_status();
};