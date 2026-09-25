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

#include "encoder.hpp"
#include "movegen.hpp"
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
    constexpr Color get_to_move() const { return to_move; }
    constexpr bitboard get_piece(Piece piece) const {
        return all_pieces[piece];
    }
    constexpr u8 get_castle_rights() const { return castle_rights; }
    constexpr square get_en_passant_target() const { return en_passant_target; }
    constexpr bitboard get_occupancy() const { return occupancy_board; }
    constexpr bitboard get_white_board() const { return white_board; }
    constexpr bitboard get_black_board() const { return black_board; }

    std::span<const bitboard> get_piece_view() const;
    void set_piece(Piece piece, square board_idx);
    void clear_piece(Piece piece, square sq);
    void set_castle_rights(u8 cr);

    void update_boards();
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