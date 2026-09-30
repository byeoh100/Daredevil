# Changelog
This changelog documents all notable additions and changes between each successive version of Daredevil.

## [Unreleased]

## [0.1.0] - 2026-09-29
Initial release.

### Added
* Board representation
  * Bitboard representation of pieces
  * Loading from FEN notation
  * Print functions for readability and debugging
* Move generation
  * Pre-calculated attack tables
    * Leaper pieces using basic bitwise logic
    * Slider pieces using magic bitboards, with hardcoded magics and a generator for new ones
  * Move encoding as integers
  * Legal move application with castling, en passant and promotions
  * Validated with perft against CPW results on five positions
* Evaluation
  * Material scoring
  * Piece-square tables (CPW Simplified Evaluation Function)
* Search
  * Negamax with alpha-beta pruning at a fixed depth (default 4)
  * Checkmate and stalemate detection, preferring the shortest mate
* UCI protocol
  * `uci`, `isready`, `ucinewgame`, `position`, `go`, `stop`, `quit`
  * Debug commands: `go perft <depth>`, `eval`, `d`
  * Tested with Cute Chess