#include "encoder.hpp"

namespace encoder {
void print_move(u32 encoded_move) {
    Move move = decode_move(encoded_move);

    static constexpr std::array<std::string_view, 12> piece_names = {
        "Pawn(W)", "Knight(W)", "Bishop(W)", "Rook(W)", "Queen(W)", "King(W)",
        "Pawn(B)", "Knight(B)", "Bishop(B)", "Rook(B)", "Queen(B)", "King(B)"};
    static constexpr std::array<std::string_view, 16> flag_names = {
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
}  // namespace encoder
