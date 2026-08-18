// Fenwick Tree 2D: prefix and rectangle sums on a grid under point updates.
// Use when: "sum of this rectangle with updates", counting points in a box, 2D inversion-style counting.
// Handles: point add, point set, rectangle sum, negative values, and non-square grids.
// Time: build O(n*m) | add O(log n log m) | rectangle sum O(log n log m)
// Indexing: 1-based on both axes by default; the constructor takes the row and column counts
// Note: for RECTANGLE updates as well as rectangle queries you need four trees - use Segment_Tree_2D.cpp instead.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct Fenwick_Tree_2D {
    int n, m;
    vector<vector<T>> tree;

    Fenwick_Tree_2D(int n, int m) : n(n), m(m), tree(n + 1, vector<T>(m + 1, 0)) {}

    Fenwick_Tree_2D(const vector<vector<T>> &grid) // grid is 0-based, of size n by m
        : n((int)grid.size()), m(grid.empty() ? 0 : (int)grid[0].size()), tree(n + 1, vector<T>(m + 1, 0)) {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) add(i + 1, j + 1, grid[i][j]);
    }

    void add(int x, int y, T delta) { // a[x][y] += delta
        for (int i = x; i <= n; i += i & -i)
            for (int j = y; j <= m; j += j & -j) tree[i][j] += delta;
    }

    T prefix(int x, int y) { // sum over a[1..x][1..y]
        T total = 0;
        for (int i = x; i > 0; i -= i & -i)
            for (int j = y; j > 0; j -= j & -j) total += tree[i][j];
        return total;
    }

    T sum(int x1, int y1, int x2, int y2) { // sum over the rectangle [x1..x2] by [y1..y2]
        if (x1 > x2 || y1 > y2) return 0;
        return prefix(x2, y2) - prefix(x1 - 1, y2) - prefix(x2, y1 - 1) + prefix(x1 - 1, y1 - 1);
    }

    T get(int x, int y) { return sum(x, y, x, y); } // a[x][y]

    void set(int x, int y, T value) { add(x, y, value - get(x, y)); } // a[x][y] = value
};

// Standard problem: a grid, then queries that either set a cell or ask for a rectangle sum
void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    Fenwick_Tree_2D<ll> bit(n, m);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            ll v;
            cin >> v;
            bit.add(i, j, v);
        }
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y;
            ll v;
            cin >> x >> y >> v;
            bit.set(x, y, v);
        } else {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            cout << bit.sum(x1, y1, x2, y2) << '\n';
        }
    }
}
