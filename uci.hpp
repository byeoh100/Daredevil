#pragma once

#include <string>
#include <string_view>

#include "game.hpp"
#include "types.hpp"

namespace uci {
// Reads UCI commands from stdin until "quit" or end of input
void loop();

// Converts an encoded move to UCI long algebraic notation, e.g. "e2e4",
// "e7e8q"
std::string move_to_uci(u32 move);

// Finds the legal move matching a UCI move string; returns 0 if there is none
u32 uci_to_move(const GameBoard& game_board, std::string_view uci_move);
}  // namespace uci
