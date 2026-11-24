# Daredevil
[![Perft](https://github.com/byeoh100/bitmap-chess/actions/workflows/perft.yml/badge.svg)](https://github.com/byeoh100/bitmap-chess/actions/workflows/perft.yml)

Daredevil is a UCI-targeted chess engine designed with a goal to focus on piece sacrifices and winning from materially deficit positions.

## References
Most, if not all, of the techniques implemented are based on the information provided or derived from [Chess Programming Wiki (CPW)](https://chessprogramming.org/). A reference engine called [Bit Board Chess (BBC)](https://github.com/maksimKorzh/bbc) was also used as a guideline for project structure and direction.

## Current Build Direction
* Time management
  * Early stops to search
* Refining search
  * Iterative deepening

All prior updates to the engine can be found in `CHANGELOG.md`.

## Perft results
Daredevil's ability to generate correct moves has been validated through comparing perft results to the pre-determined results on [CPW](https://chessprogramming.org/Perft_Results).

This is enforced by CI: on every push and pull request, [GitHub Actions](.github/workflows/perft.yml) builds the engine and runs the [perft suite](tests/perft.epd), all six CPW reference positions plus the mirrored position 4, checking every depth up to 5-7 (over 800 million leaf nodes). Any mismatch fails the build.

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
These give a calculation speed of ~34 million nodes per second (nps).

## Quick Start
Requirements:
* GCC/G++ with C++23
* make

Make, run, and start with:
```
make run
```

Run the perft suite (requires Python 3):
```
make test
```

You can also manually run starting position perft tests with:
```
position startpos
go depth <depth>
```
