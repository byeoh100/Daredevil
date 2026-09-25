#include "moves.hpp"

#include <string_view>

#include "utils.hpp"

namespace moves {
// encoding key bits:
// <---dead 8 bits---> | 16-23 | 12-15 | 6-11 | 0-5
//                       flags   piece   tgt    src
u32 encode_move(square source, square target, Piece piece, MoveFlag flag) {
    // insert error handling for empty move values
    u32 encoded_move = 0;
    encoded_move |= (static_cast<u32>(source));
    encoded_move |= (static_cast<u32>(target)) << 6;
    encoded_move |= (static_cast<u32>(piece)) << 12;
    encoded_move |= (static_cast<u32>(flag)) << 16;

    return encoded_move;
}

Move decode_move(u32 encoded_move) {
    Move decoded_move;

    decoded_move.source =
        static_cast<square>(encoded_move & ((1 << 6) - 1));  // 6b mask
    encoded_move >>= 6;
    decoded_move.target = static_cast<square>(encoded_move & ((1 << 6) - 1));
    encoded_move >>= 6;
    decoded_move.piece =
        static_cast<Piece>(encoded_move & ((1 << 4) - 1));  // 4b mask
    encoded_move >>= 4;
    decoded_move.flag =
        static_cast<MoveFlag>(encoded_move & ((1 << 8) - 1));  // 8b mask

    return decoded_move;
}

void print_move(u32 encoded_move) {
    Move move = decode_move(encoded_move);

    constexpr std::array<std::string_view, 12> piece_names = {
        "King(W)", "Queen(W)", "Rook(W)", "Bishop(W)", "Knight(W)", "Pawn(W)",
        "King(B)", "Queen(B)", "Rook(B)", "Bishop(B)", "Knight(B)", "Pawn(B)"};
    constexpr std::array<std::string_view, 16> flag_names = {
        "Quiet",
        "Double",
        "Castle Kingside",
        "Castle Queenside",
        "Capture",
        "En Passant",
        "",
        "",
        "Promotion Knight",
        "Promotion Bishop",
        "Promotion Rook",
        "Promotion Queen",
        "Capture Promotion Knight",
        "Capture Promotion Bishop",
        "Capture Promotion Rook",
        "Capture Promotion Queen",
    };

    std::string_view piece = piece_names[move.piece];
    std::string_view flag = flag_names[move.flag];

    std::cout << encoded_move << " | " << flag << " | " << piece << " | "
              << utils::square_to_algebraic(move.source) << " -> "
              << utils::square_to_algebraic(move.target) << "\n";
}
}  // namespace moves
