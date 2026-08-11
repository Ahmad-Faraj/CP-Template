/*
    [1] DESCRIPTION
    A 1-based 3D Binary Indexed Tree (Fenwick Tree) for point updates
    and 3D subvolume range sum queries in spatial data structures.

    [2] TIME & SPACE COMPLEXITY
    Build: O(N * M * P)
    Update/Query: O(log N * log M * log P)
    Space: O(N * M * P)

    [3] LIMITS
    N * M * P <= 1e7 due to memory constraints.
*/

#include "../../core.h"

// 1-based indexing
template <typename T = int>
class Fenwick_Tree_3D {
   private:
    vector<vector<vector<T>>> BIT;
    int n, m, p;

   public:
    Fenwick_Tree_3D() = default;

    // Initialize empty 3D BIT with dimensions n x m x p
    Fenwick_Tree_3D(int n, int m, int p) : n(n), m(m), p(p) {
        BIT.assign(n + 5, vector<vector<T>>(m + 5, vector<T>(p + 5, 0)));
    }

    // Initialize and build 2D BIT from 3D vector (0-based or 1-based source input)
    Fenwick_Tree_3D(int n, int m, int p, const vector<vector<vector<T>>>& arr) : n(n), m(m), p(p) {
        BIT.assign(n + 5, vector<T>(m + 5, 0));
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                for (int k = 1; k <= p; k++) add(i, j, arr[i][j]);
    }

    // Set arr[idx_x][idx_y][idx_z] = val
    void assign(int idx_x, int idx_y, int idx_z, T val) {
        T prv = query(idx_x, idx_y, idx_z, idx_x, idx_y, idx_z);
        add(idx_x, idx_y, idx_z, val - prv);
    }

    // Add v to arr[idx_x][idx_y][idx_z]
    void add(int idx_x, int idx_y, int idx_z, T v) {
        for (int i = idx_x; i <= n; i += (i & -i)) {
            for (int j = idx_y; j <= m; j += (j & -j)) {
                for (int k = idx_z; k <= p; k += (k & -k)) BIT[i][j][k] += v;
            }
        }
    }

    // Query prefix sum [1..idx_x][1..idx_y][1..idx_z]
    T query(int idx_x, int idx_y, int idx_z) {
        T ans = 0;
        for (int i = idx_x; i > 0; i -= (i & -i)) {
            for (int j = idx_y; j > 0; j -= (j & -j)) {
                for (int k = idx_z; k > 0; k -= (k & -k)) ans += BIT[i][j][k];
            }
        }

        return ans;
    }

    // Query 3D subvolume range sum [x1..x2][y1..y2][z1..z2]
    T query(int x1, int y1, int z1, int x2, int y2, int z2) {
        return query(x2, y2, z2) - query(x1 - 1, y2, z2) - query(x2, y1 - 1, z2) - query(x2, y2, z1 - 1) +
               query(x1 - 1, y1 - 1, z2) + query(x1 - 1, y2, z1 - 1) + query(x2, y1 - 1, z1 - 1) -
               query(x1 - 1, y1 - 1, z1 - 1);
    }
};