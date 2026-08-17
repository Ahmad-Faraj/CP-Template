// XOR Basis: a maximal independent set of values under XOR, so any reachable XOR is a combination of the basis.
// Use when: "largest XOR of a subset", "is x reachable", "k-th smallest reachable XOR", counting distinct XORs.
// Handles: duplicate and zero inputs, the empty subset, and counting how many subsets give a particular value.
// Time: add O(BITS) | every query O(BITS)
// Indexing: bit positions 0..BITS-1, most significant first
// Note: the number of DISTINCT reachable values is 2^rank; the number of SUBSETS giving one value is 2^(count - rank).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll, int BITS = 62> struct XOR_Basis {
    array<T, BITS> basis{}; // basis[i] is the vector whose leading bit is i, or 0
    int rank = 0;           // how many non-zero basis vectors there are
    ll count = 0;           // how many values have been added, duplicates included

    XOR_Basis() { basis.fill(0); }

    bool add(T x) { // true when x enlarged the basis
        count++;
        for (int i = BITS - 1; i >= 0; i--) {
            if (!((x >> i) & 1)) continue;
            if (!basis[i]) {
                basis[i] = x;
                rank++;
                return true;
            }
            x ^= basis[i];
        }
        return false; // x reduced to zero, so it was already reachable
    }

    T reduce(T x) const { // x with every basis leading bit cancelled out
        for (int i = BITS - 1; i >= 0; i--)
            if (((x >> i) & 1) && basis[i]) x ^= basis[i];
        return x;
    }

    bool can_make(T x) const { return reduce(x) == 0; }

    T max_xor(T start = 0) const { // the largest value reachable, optionally starting from `start`
        T res = start;
        for (int i = BITS - 1; i >= 0; i--)
            if (basis[i] && (res ^ basis[i]) > res) res ^= basis[i];
        return res;
    }

    T min_xor(T start = 0) const {
        T res = start;
        for (int i = BITS - 1; i >= 0; i--)
            if (basis[i] && (res ^ basis[i]) < res) res ^= basis[i];
        return res;
    }

    ll distinct_count() const { return 1LL << rank; } // how many different XOR values are reachable

    // how many SUBSETS of the added values XOR to x; 0 when x is unreachable
    ll subsets_making(T x) const { return can_make(x) ? (1LL << (count - rank)) : 0; }

    // the k-th smallest reachable value, k from 0; -1 when k is past the end
    T kth_smallest(ll k) const {
        if (k < 0 || k >= (1LL << rank)) return -1;
        // reduced row echelon form: clear each leading bit out of every other row,
        // which makes the rows independent bit by bit and so orderable by k's bits
        array<T, BITS> b = basis;
        for (int i = 0; i < BITS; i++) {
            if (!b[i]) continue;
            for (int j = i + 1; j < BITS; j++)
                if (b[j] && ((b[j] >> i) & 1)) b[j] ^= b[i];
        }
        vector<T> rows; // ascending by leading bit
        for (int i = 0; i < BITS; i++)
            if (b[i]) rows.push_back(b[i]);
        T res = 0;
        for (size_t i = 0; i < rows.size(); i++)
            if ((k >> i) & 1) res ^= rows[i];
        return res;
    }

    void merge(const XOR_Basis &other) { // the basis of the union
        for (int i = BITS - 1; i >= 0; i--)
            if (other.basis[i]) add(other.basis[i]);
    }
};

// Standard problem: n values - report the largest XOR of any subset, and how many distinct XORs exist
void solve() {
    int n;
    cin >> n;
    XOR_Basis<ll> b;
    for (int i = 0; i < n; i++) {
        ll x;
        cin >> x;
        b.add(x);
    }
    cout << b.max_xor() << ' ' << b.distinct_count() << '\n';
}
