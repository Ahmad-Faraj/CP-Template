/*
    [1] DESCRIPTION
    A 1-based Binary Indexed Tree (Fenwick Tree) supporting range updates
    and range sum queries using dual BIT arrays ($B_1$ and $B_2$). Serves as
    a lightweight, low-constant-factor alternative to a Lazy Segment Tree.

    [2] TIME & SPACE COMPLEXITY
    Build: O(N) or O(N log N)
    Update/Query: O(log N)
    Space: O(N)

    [3] LIMITS
    N <= 1e7 due to memory.
*/

#include "../../core.h"

// 1-based indexing
template <typename T = long long>
class Fenwick_Tree {
   private:
    vector<T> B1, B2;
    int n;

    void add_point(int idx, T v) {
        T v1 = v;
        T v2 = v * (idx - 1);
        for (int i = idx; i <= n; i += (i & -i)) {
            B1[i] += v1;
            B2[i] += v2;
        }
    }

   public:
    Fenwick_Tree() = default;

    // Initialize empty BIT with size n
    Fenwick_Tree(int n) : n(n) {
        B1.assign(n + 5, 0);
        B2.assign(n + 5, 0);
    }

    // Initialize and build BIT from 1-based array
    Fenwick_Tree(int n, const vector<T>& arr) : n(n) {
        B1.assign(n + 5, 0);
        B2.assign(n + 5, 0);
        for (int i = 1; i <= n; i++) add(i, i, arr[i]);
    }

    // Add v to range [L, R]
    void add(int L, int R, T v) {
        add_point(L, v);
        add_point(R + 1, -v);
    }

    // Single point add (convenience wrapper)
    void add(int idx, T v) { add(idx, idx, v); }

    // Set arr[idx] = val
    void assign(int idx, T val) {
        T prv = query(idx, idx);
        add(idx, idx, val - prv);
    }

    // Query prefix sum [1, idx]
    T query(int idx) {
        T sum_b1 = 0, sum_b2 = 0;
        for (int i = idx; i > 0; i -= (i & -i)) {
            sum_b1 += B1[i];
            sum_b2 += B2[i];
        }
        return sum_b1 * idx - sum_b2;
    }

    // Query range sum [L, R]
    T query(int L, int R) { return query(R) - query(L - 1); }
};