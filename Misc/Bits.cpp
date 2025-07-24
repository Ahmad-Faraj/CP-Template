#include <bits/stdc++.h>
using namespace std;

// bit manipulation
#define set_bit(n, i) ((n) | (1 << (i)))
#define clear_bit(n, i) ((n) & ~(1 << (i)))
#define toggle_bit(n, i) ((n) ^ (1 << (i)))
#define check_bit(n, i) ((n) & (1 << (i)))
#define count_bits(n) __builtin_popcount(n)
#define count_bitsll(n) __builtin_popcountll(n)
#define count_set_bits(n) __builtin_popcount(n)
#define count_set_bitsll(n) __builtin_popcountll(n)
#define count_zero_bits(n) (sizeof(n) * 8 - __builtin_popcount(n))
#define count_zero_bitsll(n) (sizeof(n) * 8 - __builtin_popcountll(n))