# Daredevil
Daredevil is a UCI-compliant chess engine designed with a goal to focus on piece sacrifices and winning from materially deficit positions.

## References
Most, if not all, of the techniques implemented are based on the information provided or derived from [Chess Programming Wiki (CPW)](https://chessprogramming.org/). A reference engine called [Bit Board Chess (BBC)](https://github.com/maksimKorzh/bbc) was also used as a guideline for project structure and direction.

## Current Build Direction
* Refine evaluation
  * Middlegame and endgame boards with a tapered evaluation function
* Refine search
  * Move ordering
  * Quiescence search

All prior updates to the engine can be found in `CHANGELOG.md`.

## Perft results
Daredevil's ability to generate correct moves has been validated through comparing perft results to the pre-determined results on [CPW](https://chessprogramming.org/Perft_Results).

The results of the most recent test on the game start position produced with `debug::perft_test()`:
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
These give a calculation speed of ~34 million nodes per second (nps).

## Quick Start
Requirements:
* GCC/G++ with C++23
* make

Make, run, and start with:
```
make run
position startpos
go depth 4
```

Daredevil is UCI-compliant, meaning it can be easily plugged into any open source chess GUI for a visual representation. [Cute Chess](https://github.com/cutechess/cutechess) is recommended.