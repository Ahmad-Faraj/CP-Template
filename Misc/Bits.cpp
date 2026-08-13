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

// Source : https://stackoverflow.com/questions/10670379/find-xor-of-all-numbers-in-a-given-range
// get xor range of numbers
// XOR ( a , a + 1 , a + 2 , ...... , b - 2 , b - 1 , b )
int xor_upto(int x) {
    if (x % 4 == 0) return x;
    if (x % 4 == 1) return 1;
    if (x % 4 == 2) return x + 1;
    return 0;
}

int xor_range(int l, int r) { return xor_upto(r) ^ xor_upto(l - 1); }
