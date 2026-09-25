#include "debug.hpp"

u64 perft_driver(GameBoard game_board, int depth) {
    if (depth == 0) {
        return 1;
    }

    MoveList move_list;
    move_gen::generate_moves(game_board, move_list);

    // Not a bitboard, but need to hold large numbers
    u64 nodes = 0;

    for(int i = 0; i < move_list.count; i++) {
        GameBoard copy = game_board;

        std::uint32_t move = move_list.moves[i];
        if(!copy.make_move(move)) continue;

        nodes += perft_driver(copy, depth - 1);
    }

    return nodes;
}

void perft_test(GameBoard game_board, int depth) {
    MoveList move_list;
    move_gen::generate_moves(game_board, move_list);

    u64 total_nodes = 0;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < move_list.count; i++) {
        std::uint32_t move = move_list.moves[i];
        GameBoard copy = game_board;

        if (!copy.make_move(move)) continue;

        u64 nodes = perft_driver(copy, depth - 1);
        total_nodes += nodes;

        // perft_print(move);
        // std::cout << "nodes: " << nodes << "\n";
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms =
    std::chrono::duration_cast<std::chrono::milliseconds>(end -
    start).count();

    std::cout << "\nDepth: " << depth << "\n";
    std::cout << "Total nodes: " << total_nodes << "\n";
    std::cout << "Time: " << elapsed_ms << " ms\n";
}

void perft_print(std::uint32_t encoded_move, u64 nodes) {
    Move move = decode_move(encoded_move);

    char source_file = (static_cast<int>(move.source) % 8) + 'a';
    char source_rank = (static_cast<int>(move.source) / 8) + '1';
    char target_file = (static_cast<int>(move.target) % 8) + 'a';
    char target_rank = (static_cast<int>(move.target) / 8) + '1';

    std::cout << source_file << source_rank << target_file << target_rank
              << " - " << nodes << "\n";
}