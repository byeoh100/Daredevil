// #include "debug.hpp"

// u64 perft_driver(GameBoard game_board, int depth) {
//     if (depth == 0) {
//         return 1;
//     }

//     MoveList move_list;
//     move_gen::generate_moves(game_board, move_list);

//     // Not a bitboard, but need to hold large numbers
//     u64 nodes = 0;

//     for(int i = 0; i < move_list.count; i++) {
//         GameBoard copy = game_board;

//         std::uint32_t move = move_list.moves[i];
//         if(!copy.make_move(move)) continue;

//         nodes += perft_driver(copy, depth - 1);
//     }

//     return nodes;
// }

// void perft_test(GameBoard game_board, int depth) {
//     MoveList move_list;
//     move_gen::generate_moves(game_board, move_list);

//     u64 total_nodes = 0;

//     auto start = std::chrono::steady_clock::now();

//     for (int i = 0; i < move_list.count; i++) {
//         std::uint32_t move = move_list.moves[i];
//         GameBoard copy = game_board;

//         if (!copy.make_move(move)) continue;

//         u64 nodes = perft_driver(copy, depth - 1);
//         total_nodes += nodes;

//         // perft_print(move);
//         // std::cout << "nodes: " << nodes << "\n";
//     }

//     auto end = std::chrono::steady_clock::now();
//     auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//     std::cout << "\nDepth: " << depth << "\n";
//     std::cout << "Total nodes: " << total_nodes << "\n";
//     std::cout << "Time: " << elapsed_ms << " ms\n";
// }

#include "debug.hpp"

// global-ish move path for debugging — not thread safe, fine for single-threaded perft debugging
std::vector<std::uint32_t> g_debug_path;

void print_path() {
    std::cerr << "=== Move path to corruption ===\n";
    for (size_t i = 0; i < g_debug_path.size(); i++) {
        std::cerr << (i + 1) << ". ";
        print_move(g_debug_path[i]);
    }
    std::cerr << "================================\n";
}

bool kings_ok(const GameBoard& board) {
    return std::popcount(board.get_piece(WHITE_KING)) == 1
        && std::popcount(board.get_piece(BLACK_KING)) == 1;
}

u64 perft_driver(GameBoard game_board, int depth) {
    if (depth == 0) {
        return 1;
    }

    MoveList move_list;
    move_gen::generate_moves(game_board, move_list);
    if (move_list.count > 220) {
        std::cerr << "SUSPICIOUS MOVE COUNT: " << move_list.count << "\n";
        game_board.print();
        std::exit(1);
    }

    u64 nodes = 0;

    for(int i = 0; i < move_list.count; i++) {
        GameBoard copy = game_board;

        std::uint32_t move = move_list.moves[i];
        if(!copy.make_move(move)) continue;

        g_debug_path.push_back(move);

        if (!kings_ok(copy)) {
            std::cerr << "CORRUPTION DETECTED\n";
            print_path();
            copy.print();
            std::exit(1);
        }

        nodes += perft_driver(copy, depth - 1);

        g_debug_path.pop_back();
    }

    return nodes;
}

void perft_test(GameBoard game_board, int depth) {
    g_debug_path.clear();

    MoveList move_list;
    move_gen::generate_moves(game_board, move_list);

    u64 total_nodes = 0;

    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < move_list.count; i++) {
        std::uint32_t move = move_list.moves[i];
        GameBoard copy = game_board;

        if (!copy.make_move(move)) continue;

        g_debug_path.push_back(move);

        if (!kings_ok(copy)) {
            std::cerr << "CORRUPTION DETECTED at root\n";
            print_path();
            copy.print();
            std::exit(1);
        }

        u64 nodes = perft_driver(copy, depth - 1);
        total_nodes += nodes;

        g_debug_path.pop_back();

        // perft_print(move, nodes);
        // std::cout << "nodes: " << nodes << "\n";
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "\nDepth: " << depth << "\n";
    std::cout << "Total nodes: " << total_nodes << "\n";
    std::cout << "Time: " << elapsed_ms << " ms\n";
}

// keep
void perft_print(std::uint32_t encoded_move, u64 nodes) {
    Move move = decode_move(encoded_move);

    char source_file = (static_cast<int>(move.source) % 8) + 'a';
    char source_rank = (static_cast<int>(move.source) / 8) + '1';
    char target_file = (static_cast<int>(move.target) % 8) + 'a';
    char target_rank = (static_cast<int>(move.target) / 8) + '1';

    std::cout << source_file << source_rank << target_file << target_rank << " - " << nodes << "\n";

}