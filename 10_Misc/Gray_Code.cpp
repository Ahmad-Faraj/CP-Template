// Gray Code: the n-th Gray code (consecutive codes differ in exactly one bit) and its inverse.
// Use when: "list all 2^k bit strings changing one bit at a time", subsets by single changes.
// Usage: ll c = gray(i); ll i = rev_gray(c); list: for i in [0, 2^k) gray(i)
// Handles: all 64-bit patterns (negative values are treated as their unsigned bits).
// Time: gray O(1), rev_gray O(number of bits)
// Indexing: 0-based; gray(0) = 0, and gray(i) ^ gray(i - 1) = 1 << (lowest set bit of i)
// Note: use 1LL << k for the sequence length when k >= 31.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll gray(ll n) { return n ^ (ll)((unsigned long long)n >> 1); }

// Given g, restore n
ll rev_gray(ll g) {
    unsigned long long n = 0;
    for (unsigned long long x = g; x; x >>= 1) n ^= x;
    return (ll)n;
}
