#include <array>
#include <bit>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "debug.hpp"
#include "game.hpp"
#include "movegen.hpp"
#include "types.hpp"
#include "utils.hpp"

int main() {
    GameBoard board;
    board.init();
    board.load_from_fen(
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    movegen::init_all_pieces();

    debug::perft_test(board, 1);
    debug::perft_test(board, 2);
    debug::perft_test(board, 3);
    debug::perft_test(board, 4);
    debug::perft_test(board, 5);
    debug::perft_test(board, 6);
    debug::perft_test(board, 7);

    return 0;
}