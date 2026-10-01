#include "uci.hpp"

namespace uci {
namespace {
constexpr std::string_view ENGINE_NAME = "Daredevil";
constexpr std::string_view ENGINE_AUTHOR = "byeoh100";
constexpr std::string_view START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Pseudo-legal moves from movegen, filtered through make_move's legality check
MoveList get_legal_move_list(const GameBoard& game_board) {
    MoveList pseudo_legal;
    movegen::generate_moves(game_board, pseudo_legal);

    MoveList legal;
    for (int i = 0; i < pseudo_legal.count; i++) {
        GameBoard copy = game_board;
        if (copy.make_move(pseudo_legal.moves[i]))
            legal.push(pseudo_legal.moves[i]);
    }
    return legal;
}

void handle_uci() {
    std::cout << "id name " << ENGINE_NAME << "\n";
    std::cout << "id author " << ENGINE_AUTHOR << "\n";
    std::cout << "uciok\n";
}

// position [startpos | fen <fen>] [moves <move> ...]
// The position is built on a copy and only replaces the current one if the
// whole command is valid, so a bad command leaves the previous position intact
void handle_position(GameBoard& game_board, std::istringstream& args) {
    GameBoard new_board = game_board;
    std::string token;
    args >> token;

    try {
        if (token == "startpos") {
            new_board.load_from_fen(START_FEN);
            args >> token;
        } else if (token == "fen") {
            std::string fen;
            int fields = 0;
            while (args >> token && token != "moves") {
                if (fields > 0) fen += " ";
                fen += token;
                fields++;
            }
            // Some GUIs send FENs without the move clocks
            if (fields == 4) fen += " 0 1";
            new_board.load_from_fen(fen);
        } else {
            std::cout << "info string expected startpos or fen\n";
            return;
        }
    } catch (const std::exception& e) {
        std::cout << "info string invalid position: " << e.what() << "\n";
        return;
    }

    if (token == "moves") {
        while (args >> token) {
            u32 move = uci_to_move(new_board, token);
            if (move == 0) {
                std::cout << "info string illegal move " << token << "\n";
                return;
            }
            (void)new_board.make_move(move);
        }
    }

    game_board = new_board;
}

// go perft <depth>: per-move node counts in the same format as Stockfish, so
// the output can be diffed against it when hunting movegen bugs
void handle_perft(const GameBoard& game_board, int depth) {
    MoveList legal = get_legal_move_list(game_board);
    u64 total_nodes = 0ULL;

    for (int i = 0; i < legal.count; i++) {
        GameBoard copy = game_board;
        (void)copy.make_move(legal.moves[i]);
        u64 nodes = (depth > 1) ? debug::perft_driver(copy, depth - 1) : 1ULL;
        total_nodes += nodes;
        std::cout << move_to_uci(legal.moves[i]) << ": " << nodes << "\n";
    }

    std::cout << "\nNodes searched: " << total_nodes << "\n";
}

// UCI score: "cp <centipawns>" or "mate <moves>", negative when the side to
// move is the one getting mated
std::string format_score(int score) {
    if (std::abs(score) > search::MATE_SCORE - 1000) {
        int moves = (search::MATE_SCORE - std::abs(score) + 1) / 2;
        return "mate " + std::to_string(score > 0 ? moves : -moves);
    }
    return "cp " + std::to_string(score);
}

void print_info(int depth, const search::SearchResult& result, u64 time_ms) {
    u64 nps = result.node_count * 1000 / std::max<u64>(time_ms, 1);
    std::cout << "info depth " << depth << " score "
              << format_score(result.evaluation) << " nodes " << result.node_count
              << " time " << time_ms << " nps " << nps << " pv "
              << move_to_uci(result.best_move) << "\n";
}

u64 elapsed_ms(std::chrono::steady_clock::time_point start) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - start)
        .count();
}

// Fixed positions for bench. Changing this list changes the bench signature,
// so only append to it deliberately.
constexpr std::array<std::string_view, 8> BENCH_FENS = {
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    "r1bq1rk1/pp2bppp/2n1pn2/2pp4/3P4/2PBPN2/PP1N1PPP/R1BQ1RK1 w - - 0 9",
    "6k1/5ppp/8/8/8/8/5PPP/3R2K1 w - - 0 1",
    "8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1",
};
constexpr int DEFAULT_BENCH_DEPTH = 5;

// bench [depth]: search every bench position to a fixed depth. The total node
// count is deterministic, so it identifies the search's behaviour exactly.
void handle_bench(std::istringstream& args) {
    int depth = DEFAULT_BENCH_DEPTH;
    args >> depth;

    u64 total_nodes = 0ULL;
    auto start = std::chrono::steady_clock::now();

    for (std::string_view fen : BENCH_FENS) {
        GameBoard game_board;
        game_board.load_from_fen(fen);

        search::SearchResult result = search::search_best(game_board, depth);
        total_nodes += result.node_count;
        std::cout << "bestmove " << move_to_uci(result.best_move) << "  nodes "
                  << result.node_count << "  " << fen << "\n";
    }

    u64 time_ms = elapsed_ms(start);
    std::cout << "\nNodes: " << total_nodes << "  Time: " << time_ms
              << " ms  NPS: " << total_nodes * 1000 / std::max<u64>(time_ms, 1)
              << "\n";
}

// go [wtime ..] [btime ..] [movetime ..] [depth ..] ... | go perft <depth>
void handle_go(const GameBoard& game_board, std::istringstream& args) {
    int depth = 6;

    std::string token;
    while (args >> token) {
        if (token == "perft") {
            int perft_depth = 1;
            args >> perft_depth;
            handle_perft(game_board, perft_depth);
            return;
        }
        if (token == "depth") args >> depth;
        // Time controls (wtime, btime, winc, binc, movetime) are ignored
        // until search has time management
    }

    auto start = std::chrono::steady_clock::now();
    search::SearchResult result = search::search_best(game_board, depth);
    u64 time_ms = elapsed_ms(start);

    // No legal moves: the position is already mate or stalemate
    if (result.best_move == 0) {
        std::cout << "bestmove 0000\n";
        return;
    }

    print_info(depth, result, time_ms);
    std::cout << "bestmove " << move_to_uci(result.best_move) << "\n";
}
}  // namespace

void loop() {
    // The GUI waits on each reply, so every line must be flushed immediately
    std::cout.setf(std::ios::unitbuf);

    GameBoard game_board;
    game_board.load_from_fen(START_FEN);

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream args(line);
        std::string command;
        args >> command;

        if (command == "uci") {
            handle_uci();
        } else if (command == "isready") {
            std::cout << "readyok\n";
        } else if (command == "ucinewgame") {
            game_board.load_from_fen(START_FEN);
        } else if (command == "position") {
            handle_position(game_board, args);
        } else if (command == "go") {
            handle_go(game_board, args);
        } else if (command == "stop") {
            // go is synchronous, so there is never a search to stop
        } else if (command == "bench") {
            handle_bench(args);
        } else if (command == "d") {
            game_board.print();
        } else if (command == "eval") {
            std::cout << "eval " << eval::evaluate(game_board)
                      << " (side to move)\n";
        } else if (command == "quit") {
            break;
        }
    }
}

std::string move_to_uci(u32 move) {
    std::string uci_move =
        utils::square_to_algebraic(encoder::get_move_source(move)) +
        utils::square_to_algebraic(encoder::get_move_target(move));

    MoveFlag flag = encoder::get_move_flag(move);
    if (flag & PROMOTION_BIT) uci_move += "nbrq"[flag & 0b0011];

    return uci_move;
}

u32 uci_to_move(const GameBoard& game_board, std::string_view uci_move) {
    MoveList legal = get_legal_move_list(game_board);
    for (int i = 0; i < legal.count; i++) {
        if (move_to_uci(legal.moves[i]) == uci_move) return legal.moves[i];
    }
    return 0;
}
}  // namespace uci