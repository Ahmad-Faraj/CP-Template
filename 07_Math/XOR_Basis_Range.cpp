// XOR Basis Range: an XOR basis over a[l..current end], answering for any suffix start l.
// Use when: "largest XOR of a subset of a[l..r]", queries sharing a right end (sort by r).
// Usage: XOR_Basis_Range<> b; b.add(a[i], i) for i <= r; then
//        b.max_xor_from(l), b.can_make_from(x, l);
// Handles: duplicates and zeros, any suffix start l, rank / reachability within that suffix.
// Time: add O(BITS) | every suffix query O(BITS)
// Indexing: 0-based positions, added in increasing order; queries take the suffix start l
// Note: this answers SUFFIX queries [l, current end). For arbitrary [l, r] add elements up to r,
//       then query with l.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll, int BITS = 62> struct XOR_Basis_Range {
    array<T, BITS> basis{};
    array<int, BITS> newest{};
    ll count = 0;

    XOR_Basis_Range() {
        basis.fill(0);
        newest.fill(-1);
    }

    // add a[pos]; positions must arrive in increasing order
    void add(T x, int pos) {
        count++;
        for (int i = BITS - 1; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;
            if (newest[i] < pos) {
                swap(basis[i], x);
                swap(newest[i], pos);
            }
            if (!basis[i]) {
                basis[i] = x;
                newest[i] = pos;
                return;
            }
            x ^= basis[i];
            if (!x) return;
        }
    }

    int rank_from(int l) const { // how many basis vectors are usable for the suffix starting at l
        int total = 0;
        for (int i = 0; i < BITS; i++)
            if (basis[i] && newest[i] >= l) total++;
        return total;
    }

    T max_xor_from(int l) const {
        T res = 0;
        for (int i = BITS - 1; i >= 0; i--)
            if (basis[i] && newest[i] >= l && (res ^ basis[i]) > res) res ^= basis[i];
        return res;
    }

    bool can_make_from(T x, int l) const {
        for (int i = BITS - 1; i >= 0; i--)
            if (((x >> i) & 1) && basis[i] && newest[i] >= l) x ^= basis[i];
        return x == 0;
    }

    ll distinct_count_from(int l) const { return 1LL << rank_from(l); }
};
