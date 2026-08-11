/*
    [1] DESCRIPTION
    A static 0-based 2D Sparse Table for rapid range queries over a 2D grid/matrix.
    - One_query: O(1) query using 4 overlapping power-of-2 subgrids.
      Requires Idempotency: merge(x, x) = x (Min, Max, GCD, Bitwise AND/OR).
    - query: O(log N * log M) query using 2D binary decomposition.
      Works for ALL associative operations (Sum, XOR, Min, Max, GCD).

    [2] TIME & SPACE COMPLEXITY
    Build: O(N * M * log N * log M)
    One_query (Idempotent): O(1)
    query (General Range): O(log N * log M)
    Space: O(N * M * log N * log M)

    [3] LIMITS
    Static data structure (No point/range updates supported).
    N, M <= 1000, LOGN, LOGM = 11 (Pre-allocated table capacity).
    0-based indexing
*/

#include "../../core.h"

const int N = 1005, M = 1005;
const int LOGN = 11, LOGM = 11;
int sparse[N][M][LOGN][LOGM];

// Merge function (change for max, gcd, XOR, etc.)
int merge(int x, int y) { return min(x, y); }

// Build 2D Sparse Table in O(N * M * log N * log M)
void build(const vector<vector<int>>& nums) {
    int n = nums.size();
    int m = nums[0].size();

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) sparse[i][j][0][0] = nums[i][j];

    // Precompute 2D intervals along row and column dimensions
    for (int kn = 0; kn < LOGN; kn++) {
        for (int km = 0; km < LOGM; km++) {
            if (kn + km == 0) continue;

            for (int i = 0; i + (1 << kn) <= n; i++) {
                for (int j = 0; j + (1 << km) <= m; j++) {
                    if (kn == 0) {
                        sparse[i][j][kn][km] =
                            merge(sparse[i][j][kn][km - 1], sparse[i][j + (1LL << (km - 1))][kn][km - 1]);
                    } else {
                        sparse[i][j][kn][km] =
                            merge(sparse[i][j][kn - 1][km], sparse[i + (1LL << (kn - 1))][j][kn - 1][km]);
                    }
                }
            }
        }
    }
}

// O(log N * log M) Range Query over [x1, y1] to [x2, y2] using non-overlapping tiles
int query(int x1, int y1, int x2, int y2) {
    int lenX = x2 - x1 + 1;
    int lenY = y2 - y1 + 1;
    int ans = INT_MAX;

    for (int i = LOGN - 1; i >= 0; i--) {
        if (lenX & (1LL << i)) {
            int curY = y1;
            for (int j = LOGM - 1; j >= 0; j--) {
                if (lenY & (1LL << j)) {
                    ans = merge(ans, sparse[x1][curY][i][j]);
                    curY += (1LL << j);
                }
            }
            x1 += (1LL << i);
        }
    }
    return ans;
}

// O(1) Range Query over [x1, y1] to [x2, y2] using 4 overlapping subgrids
// Requires Idempotency: merge(x, x) = x (Min, Max, GCD, Bitwise AND/OR)
int One_query(int x1, int y1, int x2, int y2) {
    int kx = 63 - __builtin_clzll(x2 - x1 + 1);
    int ky = 63 - __builtin_clzll(y2 - y1 + 1);

    int ans1 = merge(sparse[x1][y1][kx][ky], sparse[x2 - (1LL << kx) + 1][y1][kx][ky]);
    int ans2 =
        merge(sparse[x1][y2 - (1LL << ky) + 1][kx][ky], sparse[x2 - (1LL << kx) + 1][y2 - (1LL << ky) + 1][kx][ky]);

    return merge(ans1, ans2);
}
