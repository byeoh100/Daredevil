# Daredevil
Daredevil is a UCI-compliant chess engine designed to focus on piece sacrifices and winning from materially deficit positions.

## References
Most, if not all, of the techniques implemented are based on the information provided or derived from [Chess Programming Wiki (CPW)](https://chessprogramming.org/). A reference engine called [Bit Board Chess (BBC)](https://github.com/maksimKorzh/bbc) was also used as a guideline for project structure and direction.

## Current Build State
* Move generation and board representation
  * Bitboard representation of pieces
    * Print functions for readability and debugging
  * Pre-calculated attack tables
    * Leaper pieces using basic bitwise logic
    * Slider pieces using magics
      * Hardcoded magics
      * Generator for unique magic values
  * Move encoding as integers
  * Move generation validation with performance test (perft) functions

## Perft results
Daredevil's ability to generate correct moves have been validated through comparing perft results to the pre-determined results on [CPW](https://chessprogramming.org/Perft_Results).

The results of the most recent test on the game start position:
```
// Forgoing depth 1-4 for timing purposes (correct still).

Depth: 5
Total nodes: 4865609
Time: 222 ms

Depth: 6
Total nodes: 119060324
Time: 5346 ms

Depth: 7
Total nodes: 3195901860
Time: 142340 ms

Depth: 8
Total nodes: 84998978956
Time: 4078753 ms
```
These give a calculation speed of about ~22 million nodes per second (nps).

## Quick Start
Requirements:
* GCC/G++ with C++23
* make

Make and run with:
```
make run
```