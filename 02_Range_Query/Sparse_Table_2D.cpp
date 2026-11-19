// Sparse Table 2D: max over any submatrix of a static grid in O(1).
// Use when: "max/min/gcd of the submatrix (x1,y1)..(x2,y2)", no updates, very many queries.
// Usage: SparseTable2D<int> st(grid); st.query(x1, y1, x2, y2);
// Handles: idempotent merges only (max, min, gcd, and, or). Static grid, no updates.
// Time: build O(n m log n log m) | query O(1) | memory O(n m log n log m)
// Indexing: 0-based, rectangles inclusive (x = row, y = column); grid must be non-empty
// Note: 500 x 500 ints take ~65 MB. To change the operation, edit merge() only.

#include <bits/stdc++.h>
using namespace std;

template <typename T> struct SparseTable2D {
    vector<vector<vector<vector<T>>>> st;

    T merge(T a, T b) { return max(a, b); }

    SparseTable2D(const vector<vector<T>> &grid) {
        int n = (int)grid.size(), m = (int)grid[0].size();
        int LN = __lg(n) + 1, LM = __lg(m) + 1;
        st.assign(LN, vector<vector<vector<T>>>(LM));
        for (int a = 0; a < LN; a++)
            for (int b = 0; b < LM; b++) {
                st[a][b].assign(n - (1 << a) + 1, vector<T>(m - (1 << b) + 1));
                for (int i = 0; i + (1 << a) <= n; i++)
                    for (int j = 0; j + (1 << b) <= m; j++) {
                        if (a + b == 0) st[a][b][i][j] = grid[i][j];
                        else if (!a)
                            st[a][b][i][j] =
                                merge(st[a][b - 1][i][j], st[a][b - 1][i][j + (1 << (b - 1))]);
                        else
                            st[a][b][i][j] =
                                merge(st[a - 1][b][i][j], st[a - 1][b][i + (1 << (a - 1))][j]);
                    }
            }
    }

    T query(int x1, int y1, int x2, int y2) { // rows [x1, x2] x cols [y1, y2]
        x2++, y2++;
        int a = __lg(x2 - x1), b = __lg(y2 - y1);
        auto &t = st[a][b];
        return merge(merge(t[x1][y1], t[x2 - (1 << a)][y1]),
                     merge(t[x1][y2 - (1 << b)], t[x2 - (1 << a)][y2 - (1 << b)]));
    }
};
