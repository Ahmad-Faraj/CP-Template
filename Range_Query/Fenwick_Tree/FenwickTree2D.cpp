/*
    [1] DESCRIPTION
    A 1-based 2D Binary Indexed Tree (Fenwick Tree) for point updates
    and subgrid range sum queries over a 2D matrix.

    [2] TIME & SPACE COMPLEXITY
    Build: O(N * M * log N * log M)
    Update/Query: O(log N * log M)
    Space: O(N * M)

    [3] LIMITS
    N * M <= 1e7 due to memory constraints.
*/

#include "../../core.h"

// 1-based indexing
template <typename T>
class Fenwick_Tree_2D {
   private:
    vector<vector<T>> BIT;
    int n, m;

   public:
    Fenwick_Tree_2D() = default;

    // Initialize empty BIT with dimensions n x m
    Fenwick_Tree_2D(int n, int m) : n(n), m(m) { BIT.assign(n + 5, vector<T>(m + 5, 0)); }

    // Initialize and build 2D BIT from 2D vector (0-based or 1-based source input)
    Fenwick_Tree_2D(int n, int m, const vector<vector<T>>& arr) : n(n), m(m) {
        BIT.assign(n + 5, vector<T>(m + 5, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) add(i, j, arr[i][j]);
        }
    }

    // Set arr[idx_x][idx_y] = val
    void assign(int idx_x, int idx_y, T val) {
        T prv = query(idx_x, idx_y, idx_x, idx_y);
        add(idx_x, idx_y, val - prv);
    }

    // Add v to arr[idx_x][idx_y]
    void add(int idx_x, int idx_y, T v) {
        for (int i = idx_x; i <= n; i += (i & -i)) {
            for (int j = idx_y; j <= m; j += (j & -j)) BIT[i][j] += v;
        }
    }

    // Query prefix sum [1..idx_x][1..idx_y]
    T query(int idx_x, int idx_y) {
        T ans = 0;
        for (int i = idx_x; i > 0; i -= (i & -i)) {
            for (int j = idx_y; j > 0; j -= (j & -j)) ans += BIT[i][j];
        }

        return ans;
    }

    // Query subgrid range sum [x1..x2][y1..y2]
    T query(int x1, int y1, int x2, int y2) {
        return query(x2, y2) - query(x1 - 1, y2) - query(x2, y1 - 1) + query(x1 - 1, y1 - 1);
    }
};