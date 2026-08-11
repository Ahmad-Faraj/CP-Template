/*
    [1] DESCRIPTION
    A 1-based Binary Indexed Tree (Fenwick Tree) for point updates,
    range updates, prefix queries, and binary lifting search.

    [2] TIME & SPACE COMPLEXITY
    Build: O(N log N)
    Update/Query: O(log N)
    Space: O(N)

    [3] LIMITS
    n <= 1e7 due to memory.
    lower_bound / upper_bound require non-negative values.
*/

#include "../../core.h"

// 1-based indexing
template <typename T>
class Fenwick_Tree {
   private:
    vector<T> BIT;
    int n;

   public:
    Fenwick_Tree() = default;

    // Initialize and build BIT from 1-based array
    Fenwick_Tree(int n, const vector<T>& arr) : n(n) {
        BIT.assign(n + 5, 0);
        for (int i = 1; i <= n; i++) add(i, arr[i]);
    }

    // Set arr[idx] = val
    void assign(int idx, T val) {
        T prv = query(idx, idx);
        add(idx, val - prv);
    }

    // Add v to arr[idx]
    void add(int idx, int v) {
        for (; idx <= n; idx += (idx & -idx)) BIT[idx] += v;
    }

    // Query prefix sum [1, idx]
    T query(int idx) {
        T ans = 0;
        for (int i = idx; i; i -= (i & -i)) ans += BIT[i];

        return ans;
    }

    // Query range sum [L, R]
    T query(const int L, const int R) { return query(R) - query(L - 1); }

    // Find first 1-based index with prefix sum >= x
    int lower_bound(int x) {
        int mask = 1, idx = 0;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask; mask >>= 1) {
            int nxt = idx + mask;
            if (nxt <= n && BIT[nxt] < x) {
                x -= BIT[nxt];
                idx = nxt;
            }
        }

        return idx + 1;
    }

    // Find first 1-based index with prefix sum > x
    int upper_bound(int x) {
        int mask = 1, idx = 0;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask; mask >>= 1) {
            int nxt = idx + mask;
            if (nxt <= n && BIT[nxt] <= x) {
                x -= BIT[nxt];
                idx = nxt;
            }
        }

        return idx + 1;
    }
};