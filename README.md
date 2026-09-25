# Daredevil
Daredevil is a UCI-targeted chess engine designed with a goal to focus on piece sacrifices and winning from materially deficit positions.

## References
Most, if not all, of the techniques implemented are based on the information provided or derived from [Chess Programming Wiki (CPW)](https://chessprogramming.org/). A reference engine called [Bit Board Chess (BBC)](https://github.com/maksimKorzh/bbc) was also used as a guideline for project structure and direction.

## Current Build State
* Move generation and board representation
  * Loading from FEN notation
  * Bitboard representation of pieces
    * Print functions for readability and debugging
  * Pre-calculated attack tables
    * Leaper pieces using basic bitwise logic
    * Slider pieces using magics
      * Hardcoded magics
      * Generator for unique magic values
  * Move encoding as integers
  * Move generation validation with performance test (perft) functions
  * UCI protocol
    * Tested with Cute Chess (random legal moves until search is added)

## Perft results
Daredevil's ability to generate correct moves has been validated through comparing perft results to the pre-determined results on [CPW](https://chessprogramming.org/Perft_Results).

The results of the most recent test on the game start position:
```
// ...

Depth: 5
Total nodes: 4865609
Time: 143 ms

Depth: 6
Total nodes: 119060324
Time: 2963 ms

Depth: 7
Total nodes: 3195901860
Time: 98671 ms
```
These give a calculation speed of about ~34 million nodes per second (nps).

## Quick Start
Requirements:
* GCC/G++ with C++23
* make

Make and run with:
```
make run
```