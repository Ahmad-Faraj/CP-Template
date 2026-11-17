// SQRT Decomposition: range sum with point assignment, using sqrt(n) precomputed block sums.
// Use when: an operation a segment tree cannot merge but a block can summarize; a base to adapt.
// Usage: SqrtDecomp sd(a); sd.update(i, val); sd.query(l, r);
// Handles: point assign, range sum, negatives, n = 0. Swap the block aggregate for count/min/etc.
// Time: build O(n) | update O(1) | query O(sqrt n)
// Indexing: 0-based, ranges inclusive
// Note: for plain sum queries a Fenwick / segment tree is faster; this is the skeleton to modify.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SqrtDecomp {
    int n, SQ;
    vector<ll> arr, blk; // blk[b] = sum of arr over block b = [b * SQ, (b + 1) * SQ)

    SqrtDecomp(const vector<ll> &a) : n((int)a.size()), arr(a) {
        SQ = (int)sqrt(n) + 1; // block size, never 0
        blk.assign(n / SQ + 1, 0);
        for (int i = 0; i < n; i++) blk[i / SQ] += arr[i];
    }

    void update(int idx, ll val) { // arr[idx] = val
        blk[idx / SQ] += val - arr[idx];
        arr[idx] = val;
    }

    ll query(int l, int r) { // sum of arr[l .. r], 0 if l > r
        ll ret = 0;
        while (l <= r) {
            if (l % SQ == 0 && l + SQ - 1 <= r) ret += blk[l / SQ], l += SQ;
            else ret += arr[l++];
        }
        return ret;
    }
};
