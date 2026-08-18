// Prefix Sum 2D: rectangle sums on a fixed grid in O(1), after one O(n*m) build.
// Use when: the grid never changes and you only ask for sums - "sum of this rectangle", counting marks in a box.
// Handles: any rectangle, negative values, non-square grids, queries clipped to the grid, and empty ranges.
// Time: build O(n*m) | query O(1)
// Indexing: 1-based inclusive on both axes; the grid handed to the constructor is 0-based
// Note: if the grid CHANGES use Fenwick_Tree_2D.cpp. This is the faster option only while it stays fixed.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Prefix_Sum_2D {
    int n, m;
    vector<vector<ll>> pre;

    Prefix_Sum_2D(const vector<vector<ll>> &grid) {
        n = (int)grid.size();
        m = n ? (int)grid[0].size() : 0;
        pre.assign(n + 1, vector<ll>(m + 1, 0));
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++)
                pre[i][j] = grid[i - 1][j - 1] + pre[i - 1][j] + pre[i][j - 1] - pre[i - 1][j - 1];
    }

    ll sum(int x1, int y1, int x2, int y2) { // over the rectangle [x1..x2] by [y1..y2]
        x1 = max(x1, 1), y1 = max(y1, 1), x2 = min(x2, n), y2 = min(y2, m);
        if (x1 > x2 || y1 > y2) return 0;
        return pre[x2][y2] - pre[x1 - 1][y2] - pre[x2][y1 - 1] + pre[x1 - 1][y1 - 1];
    }

    ll get(int x, int y) { return sum(x, y, x, y); }
};

// Standard problem: an n by m grid, then q rectangle sum queries
void solve() {
    int n, m, q;
    cin >> n >> m >> q;
    vector<vector<ll>> grid(n, vector<ll>(m));
    for (auto &row : grid)
        for (ll &v : row) cin >> v;
    Prefix_Sum_2D pre(grid);
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << pre.sum(x1, y1, x2, y2) << '\n';
    }
}
