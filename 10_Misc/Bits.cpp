// Bits: set / clear / toggle / test bit i, and count set or zero bits, for int and long long.
// Use when: bitmask code, "is element i in the mask", popcount of a value.
// Usage: m = set_bit(m, i); if (check_bit(m, i)) ...; count_bits(m); count_zero_bits(m)
// Handles: int (bits 0..31) and long long (bits 0..63), negative values (two's complement).
// Time: O(1)
// Indexing: bit 0 is the least significant
// Note: the shifted 1 has the type of n, so pass a long long mask for bits >= 31.
//       count_zero_bits counts zeros over the whole width of the type (32 or 64).

#include <bits/stdc++.h>
using namespace std;

template <class T> T set_bit(T n, int i) { return n | (T(1) << i); }
template <class T> T clear_bit(T n, int i) { return n & ~(T(1) << i); }
template <class T> T toggle_bit(T n, int i) { return n ^ (T(1) << i); }
template <class T> bool check_bit(T n, int i) { return (n >> i) & 1; }
template <class T> int count_bits(T n) { return __builtin_popcountll((make_unsigned_t<T>)n); }
template <class T> int count_zero_bits(T n) { return (int)sizeof(T) * 8 - count_bits(n); }
