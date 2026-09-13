#pragma once

#include "types.hpp"

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <bit>
#include <array>
#include <stdexcept>
#include <sstream>

void print_bitboard(u64 bitboard);

Piece char_to_piece(const char& c);