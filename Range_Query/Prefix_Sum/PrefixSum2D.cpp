/* Topic: 2D Prefix Sum and Difference Array
 * Description: Precomputes cumulative sums over a 2D matrix to allow O(1) time retrieval of
 *   sums for arbitrary rectangular subgrids. A foundational technique for matrix
 *   optimization problems.
 * Usage: PrefixSum2D<int> ps(n, m); ps.build(a); ps.query(r1, c1, r2, c2); Diff2D<int> ds(n, m); ds.add(r1, c1, r2, c2,
 * val); ds.build();
 */
#include "../../core.h"

template <typename T = int>
struct PrefixSum2D {
    int n, m;
    vector<vector<T>> prf;

    PrefixSum2D(int _n, int _m) {
        n = _n;
        m = _m;
        prf.assign(n + 1, vector<T>(m + 1, 0));
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N * M)
    void build(const vector<vector<T>>& a) {
        // 0-indexed matrix 'a' of size n x m
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) {
                prf[i][j] = a[i - 1][j - 1] + prf[i - 1][j] + prf[i][j - 1] - prf[i - 1][j - 1];
            }
        }
    }

    // 1-indexed query
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    T query(int r1, int c1, int r2, int c2) {
        if (r1 > r2) swap(r1, r2);
        if (c1 > c2) swap(c1, c2);
        return prf[r2][c2] - prf[r1 - 1][c2] - prf[r2][c1 - 1] + prf[r1 - 1][c1 - 1];
    }
};

template <typename T = int>
struct Diff2D {
    int n, m;
    vector<vector<T>> diff;

    Diff2D(int _n, int _m) {
        n = _n;
        m = _m;
        diff.assign(n + 2, vector<T>(m + 2, 0));
    }

    // 1-indexed update
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    void add(int r1, int c1, int r2, int c2, T val) {
        if (r1 > r2) swap(r1, r2);
        if (c1 > c2) swap(c1, c2);
        diff[r1][c1] += val;
        diff[r2 + 1][c1] -= val;
        diff[r1][c2 + 1] -= val;
        diff[r2 + 1][c2 + 1] += val;
    }

    // Time Complexity: O(N * M)
    // Space Complexity: O(N * M)
    void build() {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= m; j++) { diff[i][j] += diff[i - 1][j] + diff[i][j - 1] - diff[i - 1][j - 1]; }
        }
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    T get(int r, int c) { return diff[r][c]; }
};
