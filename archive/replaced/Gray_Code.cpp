#include <bits/stdc++.h>
using namespace std;

// two successive values differ in only one bit.
// property: g(i) = g(i - 1) ^ (1 << lsb(i))
int g(int n) { return n ^ (n >> 1); }

// Given g, restore n
int rev_g(int g) {
    int n = 0;
    for (; g; g >>= 1) n ^= g;
    return n;
}