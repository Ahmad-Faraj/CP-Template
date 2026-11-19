// XOR Basis: insert values, then ask which XORs of subsets are reachable, max, k-th, counts.
// Use when: "largest XOR of a subset", "is x reachable", "k-th smallest XOR", distinct XORs.
// Usage: XOR_Basis<> b; b.add(x); b.can_make(x); b.max_xor(); b.kth_smallest(k);
//        b.distinct_count();
// Handles: duplicate and zero inputs, the empty subset, counting how many subsets give a value.
// Time: add O(BITS) | every query O(BITS)
// Indexing: bit positions 0..BITS-1, most significant first
// Note: values must be < 2^BITS (2^62 by default). subsets_making overflows once
//       count - rank >= 63.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll, int BITS = 62> struct XOR_Basis {
    array<T, BITS> basis{};
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
        return false;
    }

    T reduce(T x) const { // x with every basis leading bit cancelled out
        for (int i = BITS - 1; i >= 0; i--)
            if (((x >> i) & 1) && basis[i]) x ^= basis[i];
        return x;
    }

    bool can_make(T x) const { return reduce(x) == 0; }

    // reachable by a NON-EMPTY subset: 0 needs a dependent value (count > rank)
    bool can_make(T x, bool exclude_empty) const {
        if (exclude_empty && x == 0) return count > rank;
        return can_make(x);
    }

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

    ll distinct_count() const { return 1LL << rank; }

    // how many SUBSETS of the added values XOR to x; 0 when x is unreachable
    ll subsets_making(T x) const { return can_make(x) ? (1LL << (count - rank)) : 0; }

    // the k-th smallest reachable value, k from 0; -1 when k is past the end
    T kth_smallest(ll k) const {
        if (k < 0 || k >= (1LL << rank)) return -1;
        array<T, BITS> b = basis;
        for (int i = 0; i < BITS; i++) {
            if (!b[i]) continue;
            for (int j = i + 1; j < BITS; j++)
                if (b[j] && ((b[j] >> i) & 1)) b[j] ^= b[i];
        }
        vector<T> rows;
        for (int i = 0; i < BITS; i++)
            if (b[i]) rows.push_back(b[i]);
        T res = 0;
        for (size_t i = 0; i < rows.size(); i++)
            if ((k >> i) & 1) res ^= rows[i];
        return res;
    }

    // the k-th largest reachable value, k from 0; -1 when k is past the end
    T kth_largest(ll k) const {
        if (k < 0 || k >= (1LL << rank)) return -1;
        return kth_smallest((1LL << rank) - k - 1);
    }

    // how many DISTINCT reachable values are strictly less than x
    T count_lt(T x) const {
        if (x < 0) return 0;
        T ans = 0;
        T cnt = ((T)1 << rank);
        T mask = 0;
        for (int i = BITS - 1; i >= 0; i--) {
            if (basis[i]) {
                if ((x >> i) & 1) {
                    ans += (cnt >> 1);
                    if (!(mask >> i & 1)) mask ^= basis[i];
                } else {
                    if (mask >> i & 1) mask ^= basis[i];
                }
                cnt >>= 1;
            } else {
                if (((x >> i) & 1) != ((mask >> i) & 1)) {
                    if ((x >> i) & 1) return ans + cnt;
                    else return ans;
                }
            }
        }
        return ans;
    }

    void clear() { basis.fill(0), rank = 0, count = 0; }

    void merge(const XOR_Basis &other) { // the basis of the union
        for (int i = BITS - 1; i >= 0; i--)
            if (other.basis[i]) add(other.basis[i]);
    }
};
