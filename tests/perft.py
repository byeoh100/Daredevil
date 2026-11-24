#!/usr/bin/env python3
"""Perft regression test for Daredevil.

Runs the engine binary over UCI ("position fen ...", "go perft N") for every
position and depth in an EPD perft suite and compares the reported leaf node
count with the known-correct value. Exits non-zero on any mismatch, crash,
timeout, or rejected position, so it can gate CI.

Usage: tests/perft.py [ENGINE] [--suite FILE] [--max-depth N] [--timeout SEC]
"""

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

NODES_RE = re.compile(r"^Nodes searched: (\d+)$", re.MULTILINE)
DEPTH_RE = re.compile(r"^D(\d+)\s+(\d+)$")


def parse_suite(path):
    """Returns a list of (line_number, fen, [(depth, expected_nodes), ...])."""
    entries = []
    for line_number, raw in enumerate(path.read_text().splitlines(), start=1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue

        fen, *fields = (part.strip() for part in line.split(";"))
        if len(fen.split()) not in (4, 6):
            sys.exit(f"{path}:{line_number}: malformed FEN: {fen!r}")

        checks = []
        for field in fields:
            match = DEPTH_RE.match(field)
            if not match:
                sys.exit(f"{path}:{line_number}: malformed field: {field!r}")
            checks.append((int(match.group(1)), int(match.group(2))))

        if not checks:
            sys.exit(f"{path}:{line_number}: no depths listed")
        entries.append((line_number, fen, checks))
    return entries


def run_perft(engine, fen, depth, timeout):
    """Returns (node_count or None, full engine output, error message)."""
    commands = f"position fen {fen}\ngo perft {depth}\nquit\n"
    try:
        result = subprocess.run(
            [engine],
            input=commands,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
    except subprocess.TimeoutExpired:
        return None, "", f"timed out after {timeout}s"

    output = result.stdout
    if result.returncode != 0:
        return None, output, f"engine exited with code {result.returncode}"

    # The engine keeps its previous position if a FEN is rejected, so any
    # info string here means the count would be for the wrong position
    if "info string" in output:
        return None, output, "engine rejected the position"

    counts = NODES_RE.findall(output)
    if len(counts) != 1:
        return None, output, "expected one 'Nodes searched' line"
    return int(counts[0]), output, ""


def main():
    repo_root = Path(__file__).resolve().parent.parent
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("engine", nargs="?", default=str(repo_root / "engine"))
    parser.add_argument("--suite", type=Path, default=repo_root / "tests" / "perft.epd")
    parser.add_argument("--max-depth", type=int, default=None,
                        help="skip checks deeper than this (for quick runs)")
    parser.add_argument("--timeout", type=float, default=300,
                        help="seconds allowed per perft call")
    args = parser.parse_args()

    if not Path(args.engine).is_file():
        sys.exit(f"engine binary not found: {args.engine} (run make first)")

    entries = parse_suite(args.suite)
    passed = failed = 0
    total_nodes = 0
    suite_start = time.monotonic()

    for line_number, fen, checks in entries:
        print(f"\n{fen}")
        for depth, expected in checks:
            if args.max_depth is not None and depth > args.max_depth:
                continue

            start = time.monotonic()
            nodes, output, error = run_perft(args.engine, fen, depth, args.timeout)
            elapsed = time.monotonic() - start

            if not error and nodes != expected:
                error = f"got {nodes}, expected {expected}"

            if error:
                failed += 1
                print(f"  FAIL  D{depth}  {error}  ({args.suite.name}:{line_number})")
                # Per-move breakdown, to diff against Stockfish's "go perft"
                for out_line in output.strip().splitlines():
                    print(f"        | {out_line}")
            else:
                passed += 1
                total_nodes += nodes
                print(f"  ok    D{depth}  {nodes:>12,}  {elapsed:7.2f}s")

    elapsed = time.monotonic() - suite_start
    print(f"\n{passed} passed, {failed} failed, "
          f"{total_nodes:,} nodes verified in {elapsed:.1f}s")

    if passed + failed == 0:
        print("no checks ran")
        return 1
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
