// Gray Code: an ordering of 0..2^k-1 in which consecutive values differ in exactly one bit, and its inverse.
// Use when: "list all subsets changing one element at a time", tower of Hanoi moves, hardware-style encodings.
// Handles: any k up to 62, the round trip back to the index, and generating the whole sequence.
// Time: O(1) per conversion | O(2^k) to list the sequence
// Indexing: 0-based; gray(0) = 0, and gray(2^k - 1) differs from gray(0) in one bit too, so the cycle closes
// Note: use 1LL << k, never 1 << k - the second is undefined behaviour for k >= 31.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll gray(ll n) { return n ^ (n >> 1); } // the n-th Gray code

ll inverse_gray(ll g) { // which index produced this Gray code
    ll n = 0;
    for (; g; g >>= 1) n ^= g;
    return n;
}

int changed_bit(ll n) { return n == 0 ? -1 : __builtin_ctzll(n); } // the bit that flips from gray(n-1) to gray(n)

vector<ll> gray_sequence(int k) { // all 2^k Gray codes in order
    vector<ll> out(1LL << k);
    for (ll i = 0; i < (1LL << k); i++) out[i] = gray(i);
    return out;
}

// Standard problem: list the k-bit Gray code sequence, one binary string per line
void solve() {
    int k;
    cin >> k;
    for (ll g : gray_sequence(k)) {
        for (int b = k - 1; b >= 0; b--) cout << ((g >> b) & 1);
        cout << '\n';
    }
}
