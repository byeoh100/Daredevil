#include "uci.hpp"

#include <exception>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

#include "debug.hpp"
#include "encoder.hpp"
#include "movegen.hpp"
#include "utils.hpp"

namespace uci {
namespace {
constexpr std::string_view ENGINE_NAME = "Daredevil";
constexpr std::string_view ENGINE_AUTHOR = "byeoh100";
constexpr std::string_view START_FEN =
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

std::mt19937 rng{std::random_device{}()};

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

// go [wtime ..] [btime ..] [movetime ..] [depth ..] ... | go perft <depth>
void handle_go(const GameBoard& game_board, std::istringstream& args) {
    std::string token;
    if (args >> token && token == "perft") {
        int depth = 1;
        args >> depth;
        handle_perft(game_board, depth);
        return;
    }

    // No search yet: every time control is ignored and a random legal move
    // is played
    MoveList legal = get_legal_move_list(game_board);
    if (legal.count == 0) {
        std::cout << "bestmove 0000\n";
        return;
    }

    std::uniform_int_distribution<int> pick(0, legal.count - 1);
    std::cout << "bestmove " << move_to_uci(legal.moves[pick(rng)]) << "\n";
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
        } else if (command == "d") {
            game_board.print();
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
