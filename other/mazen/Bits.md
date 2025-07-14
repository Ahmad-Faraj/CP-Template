# Properties of Bitwise Operations

- $a | b = a \oplus b + a \& b$
- $a \oplus (a \& b) = (a | b) \oplus b$
- $b \oplus (a \& b) = (a | b) \oplus a$
- $(a \& b) \oplus (a | b) = a \oplus b$

## Addition:
- $a + b = a | b + a \& b$
- $a + b = a \oplus b + 2(a \& b)$

## Subtraction:
- $a - b = (a \oplus (a \& b)) - ((a | b) \oplus a)$
- $a - b = ((a | b) \oplus b) - ((a | b) \oplus a)$
- $a - b = (a \oplus (a \& b)) - (b \oplus (a \& b))$
- $a - b = ((a | b) \oplus b) - (b \oplus (a \& b))$

# Built-in Bitwise Functions in C++
- `__builtin_popcount(n)`: Returns the number of set bits in `n`.
- `__builtin_popcountll(n)`: Returns the number of set bits in `n` (for `long long`).
- `__builtin_ctz(n)`: Returns the number of trailing zeros in `n`.
- `__builtin_ctzll(n)`: Returns the number of trailing zeros in `n` (for `long long`).
- `__builtin_clz(n)`: Returns the number of leading zeros in `n`.
- `__builtin_clzll(n)`: Returns the number of leading zeros in `n` (for `long long`).
- `__builtin_ffs(n)`: Returns the index of the least significant set bit in `n`.
- `__builtin_ffsll(n)`: Returns the index of the least significant set bit in `n` (for `long long`).
- `__builtin_parity(n)`: Returns the parity of `n` (number of set bits modulo 2).
- `__builtin_parityll(n)`: Returns the parity of `n` (number of set bits modulo 2) (for `long long`).

## Alternative Representations:
- $a + b = a \oplus b + 2(a \& b)$
- $a + b = 2(a | b) - (a \oplus b)$
