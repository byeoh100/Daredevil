#pragma once

#include "types.hpp"
#include "utils.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>
#include <algorithm>

class GameBoard {
    private:
        Color to_move;
        std::array<u64, 12> all_pieces{};

    public:
        void set_piece(Piece piece, int board_idx);
        void reset();
        void load_from_fen(std::string fen);
        void print();
        void print_pieces();
};