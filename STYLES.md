# Styles Guide
This defines the styling and conventions used by Daredevil.

## Auto-formatter style
This codebase uses the Google C++ style with 4-space indentation, as configured in `.clang-format`. Run it on every file before committing:
```
find . -type f \( -name "*.cpp" -o -name "*.hpp" \) | xargs clang-format -style=file -i
```

## Types
1. All unsigned integers will adopt the shorthand `u##` naming convention.
    * Ex. `u8`, `u16`, `u32`, `u64`

2. Type aliases are lowercase, to signal that they are still primitives. Enumerations, structs, and classes are capitalized.
    * Ex. `bitboard`, `square`, `u64` vs. `Piece`, `MoveFlag`, `GameBoard`

3. There will be 2 type aliases for unsigned 64-bit integers:
    * Values that are logically defined as bitboards should be typed `bitboard`
    * Values that are logically defined as actual 64-bit numbers (magic numbers, node counts) should be typed `u64`

4. There will be 2 types for integers:
    * Values that are logically defined as board squares should be typed `square`
    * Values that are logically defined as actual integers should be plain `int`

5. Because aliases are lowercase, variables will not be named after a type alias.
    * Ex. `bitboard board`, `square sq`, not `bitboard bitboard`

6. All header constants will be declared as `inline constexpr`.

7. All other compile-time constants and their derivatives will be declared as `constexpr`.

8. All small types will be taken by value and all read-only large types (containing multiple variables) will be taken by const reference.
    * Strings that are only read are taken as `std::string_view` by value.

9. All 64-bit literals will be marked with the suffix `ULL`. This includes zeros.

10. All other literals will not be marked with a suffix.

## Idioms
1. When performing bitboard loops to zero, the condition `!= 0` will be omitted.
    * Ex. `while (some_bitboard)`

2. Bits will not be popped manually, but instead be popped through `utils::pop_bit()` and `utils::pop_lsb()`.

3. Any movement on the board will use the `Direction` enumeration rather than literals unless necessary. Multiple moves in the same direction will multiply the `Direction` by the number of moves. Non-movement offsets (table offsets, `rank * 8 + file` indexing) will use plain literals.
    * Ex. `push_amt = NORTH`, `two_moves = 2 * NORTH`, `offset += 1 << 12`

4. Squares will not be converted to or from algebraic notation manually, but instead be converted through `utils::square_to_algebraic()` and `utils::algebraic_to_square()`.

5. The piece order will be king, queen, rook, bishop, knight, pawn, as denoted in the `PieceType` enumeration. This applies to enumerations, lookup tables, tuples and their bindings, move generation, and the order of code sections.

## Naming
1. All functions and variables will be snake_case. Constants and enumeration values will be SCREAMING_SNAKE_CASE.

2. All getters will be prefixed with `get_` and singular. A getter that returns a collection is named for the collection as one thing.
    * Ex. `get_rook_attack()`, `get_piece_set()`, `get_piece_view()`
    * Established singular terms are kept as-is, e.g. `get_castle_rights()` (castling rights are one value).

3. All functions not part of a class will be namespaced according to file name.
    * Ex. `movegen::generate_moves()`, `utils::pop_lsb()`, `types::shift()`
