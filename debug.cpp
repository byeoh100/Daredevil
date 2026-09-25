#include "debug.hpp"

namespace debug {
u64 perft_driver(const GameBoard& game_board, int depth) {
    if (depth == 0) {
        return 1;
    }

    MoveList move_list;
    movegen::generate_moves(game_board, move_list);

    u64 nodes = 0ULL;

    for (int i = 0; i < move_list.count; i++) {
        GameBoard copy = game_board;

        u32 move = move_list.moves[i];
        if (!copy.make_move(move)) continue;

        nodes += perft_driver(copy, depth - 1);
    }

    return nodes;
}

void perft_test(const GameBoard& game_board, int depth) {
    MoveList move_list;
    movegen::generate_moves(game_board, move_list);

    u64 total_nodes = 0ULL;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < move_list.count; i++) {
        u32 move = move_list.moves[i];
        GameBoard copy = game_board;

        if (!copy.make_move(move)) continue;

        u64 nodes = perft_driver(copy, depth - 1);
        total_nodes += nodes;

        // perft_print(move, nodes);
        // std::cout << "nodes: " << nodes << "\n";
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    std::cout << "\nDepth: " << depth << "\n";
    std::cout << "Total nodes: " << total_nodes << "\n";
    std::cout << "Time: " << elapsed_ms << " ms\n";
}

void perft_print(u32 encoded_move, u64 nodes) {
    Move move = moves::decode_move(encoded_move);

    std::cout << utils::square_to_algebraic(move.source)
              << utils::square_to_algebraic(move.target) << " - " << nodes
              << "\n";
}
}  // namespace debug