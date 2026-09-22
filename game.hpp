#pragma once

#include "types.hpp"
#include "utils.hpp"
#include "moves.hpp"
#include "attacks.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>
#include <algorithm>
#include <tuple>

class GameBoard {
    private:
        Color to_move;
        std::array<u64, 12> all_pieces{0};
        uint8_t castle_rights;
        Square en_passant_target;
        int half_move;
        int full_move;
        u64 white_board;
        u64 black_board;
        u64 occupancy_board;

    public:
        Color get_to_move() const;

        void set_piece(Piece piece, Square board_idx);
        u64 get_piece(Piece piece) const;
        void clear_piece(Piece piece, Square sq);
        
        std::span<const u64> get_all_pieces_view() const;

        uint8_t get_castle_rights() const;
        void set_castle_rights(uint8_t cr);

        Square get_en_passant_target() const;

        void update_boards();
        u64 get_occupancy() const;
        u64 get_white_board() const;
        u64 get_black_board() const;
        std::tuple<u64, u64, u64, u64, u64, u64> get_white_pieces() const;
        std::tuple<u64, u64, u64, u64, u64, u64> get_black_pieces() const;

        bool make_move(std::uint32_t move);

        void init();
        void reset();
        void load_from_fen(std::string fen);

        void print();
        void print_pieces(); // make it print an individual piece?
        void print_fen_status();
};