# Styles Guide
This defines the styling and conventions used by Daredevil.

## Auto-formatter style
This codebase uses the Google C++ style with 4-space indentation.

## Types
1. All unsigned integers will adopt the shorthand `u##` naming convention.
    * Ex. `u8`, `u16`, `u32`, `u64`

2. There will be 2 type aliases for unsigned 64-bit integers:
    * Values that are logically defined as bitboards should be named "bitboard"
    * Values that are logically defined as actual 64-bit numbers should be named "u64"

3. There will be 2 type aliases for integers:
    * Values that are logically defined as board squares should be named "square"
    * Values that are logically defined as actual integers should be plain int.

4. All header constants will be declared as inline constexpr.

5. All other compile-time constants and their derivatives will be declared as constexpr.

6. All small types will be taken by value and all read-only large types (containing multiple variables) will by taken by reference.

7. All 64-bit literals will be marked with the suffix `ULL`. This includes zeros.

8. All other literals will not be marked with a suffix.

## Idioms
1. When performing bitboard loops to zero, the condition `!= 0` will be omitted.
    * Ex. `while(some_bitboard)`

2. Bits will not be popped manually, but instead be popped through `pop_bit()` and `pop_lsb()`.

3. Any singular directional movement on a bitboard will use the `Direction` enumeration rather than literals unless necessary. Multiple moves in the same direction will multiply to the amount of moves to the `Direction.` Offsets will add the plain literal.
    * Ex. `push_amt = NORTH`, `two_moves = 2 * NORTH`

4. Square-to-algebraic notation will not be converted manually, by instead be converted through `square_to_algebraic()`.

5. The piece order will be king, queen, rook, bishop, knight, pawn as denoted in the `PieceType` enumeration.

## Naming
1. All getters will be lowercase and singular.

2. All functions not part of a class will be namespaced according to file name.
