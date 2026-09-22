#include "moves.hpp"

// encoding key bits:
// <---dead 8 bits---> | 16-23 | 12-15 | 6-11 | 0-5
//                       flags   piece   tgt    src
std::uint32_t encode_move(int source, int target, Piece piece, MoveFlag flag) {
    // insert error handling for empty move values
    std::uint32_t encoded_move = 0;
    encoded_move |= (static_cast<std::uint32_t>(source));
    encoded_move |= (static_cast<std::uint32_t>(target)) << 6;
    encoded_move |= (static_cast<std::uint32_t>(piece)) << 12;
    encoded_move |= (static_cast<std::uint32_t>(flag)) << 16;

    return encoded_move;
}


Move decode_move(std::uint32_t encoded_move) {
    Move decoded_move;
    
    decoded_move.source = static_cast<Square>(encoded_move & ((1 << 6) - 1)); // 6b mask
    encoded_move >>= 6;
    decoded_move.target = static_cast<Square>(encoded_move & ((1 << 6) - 1));
    encoded_move >>= 6;
    decoded_move.piece = static_cast<Piece>(encoded_move & ((1 << 4) - 1)); // 4b mask
    encoded_move >>= 4;
    decoded_move.flag = static_cast<MoveFlag>(encoded_move & ((1 << 8) - 1)); // 8b mask

    return decoded_move;
}

void print_move(std::uint32_t encoded_move) {
    Move move = decode_move(encoded_move);

    std::array<std::string, 12> piece_names = {"King(W)", "Queen(W)", "Rook(W)", "Bishop(W)", "Knight(W)", "Pawn(W)", "King(B)", "Queen(B)", "Rook(B)", "Bishop(B)", "Knight(B)", "Pawn(B)"};
    std::array<std::string, 16> flag_names = {
        "Quiet", "Double", "Castle Kingside", "Castle Queenside", "Capture", "En Passant", "", "",
        "Promotion Knight", "Promotion Bishop", "Promotion Rook", "Promotion Queen",
        "Capture Promotion Knight", "Capture Promotion Bishop", "Capture Promotion Rook", "Capture Promotion Queen",
    };

    char source_file = (static_cast<int>(move.source) % 8) + 'a';
    char source_rank = (static_cast<int>(move.source) / 8) + '1';
    char target_file = (static_cast<int>(move.target) % 8) + 'a';
    char target_rank = (static_cast<int>(move.target) / 8) + '1';
    std::string piece = piece_names[move.piece];
    std::string flag = flag_names[move.flag];

    std::cout << encoded_move << " | " << flag << " | " << piece << " | " << source_file << source_rank << " -> " << target_file << target_rank << "\n";

}