// 2D Segment Tree: rectangle query with point assignment on a fixed-size grid.
// Use when: "sum/min/max of the submatrix (x1,y1)..(x2,y2)" with point updates, on a grid up to roughly 500x500.
// Handles: point assign, rectangle query, any associative merge. No lazy, no rectangle updates.
// Time: build O(n m) | update O(log n log m) | query O(log n log m) | memory O(16 n m)
// Indexing: 1-based for queries and updates; the vector given to the constructor is 0-based
// Note: change merge() and Default together - Default must be the identity element of merge().

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree2D {
    int n, m;
    int Default = 0; // must stay declared before t: t's initializer reads it
    vector<vector<int>> t;

    SegTree2D(int n, int m) : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)) {}
    SegTree2D(int n, int m, vector<vector<int>> &a) : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)) { build(a); }

    int merge(int a, int b) { return a + b; }

    void build_y(int vx, int lx, int rx, int vy, int ly, int ry, vector<vector<int>> &a) {
        if (ly == ry) {
            if (lx == rx) {
                if (lx <= (int)a.size() && ly <= (int)a[0].size()) t[vx][vy] = a[lx - 1][ly - 1];
            } else
                t[vx][vy] = merge(t[vx * 2][vy], t[vx * 2 + 1][vy]);
        } else {
            int my = (ly + ry) / 2;
            build_y(vx, lx, rx, vy * 2, ly, my, a);
            build_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, a);
            t[vx][vy] = merge(t[vx][vy * 2], t[vx][vy * 2 + 1]);
        }
    }

    void build_x(int vx, int lx, int rx, vector<vector<int>> &a) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            build_x(vx * 2, lx, mx, a);
            build_x(vx * 2 + 1, mx + 1, rx, a);
        }
        build_y(vx, lx, rx, 1, 1, m, a);
    }

    void build(vector<vector<int>> &a) { build_x(1, 1, n, a); }

    int query_y(int vx, int vy, int tly, int try_, int ly, int ry) {
        if (ly > ry) return Default;
        if (ly == tly && try_ == ry) return t[vx][vy];
        int tmy = (tly + try_) / 2;
        return merge(query_y(vx, vy * 2, tly, tmy, ly, min(ry, tmy)),
                     query_y(vx, vy * 2 + 1, tmy + 1, try_, max(ly, tmy + 1), ry));
    }

    int query_x(int vx, int tlx, int trx, int lx, int rx, int ly, int ry) {
        if (lx > rx) return Default;
        if (lx == tlx && trx == rx) return query_y(vx, 1, 1, m, ly, ry);
        int tmx = (tlx + trx) / 2;
        return merge(query_x(vx * 2, tlx, tmx, lx, min(rx, tmx), ly, ry),
                     query_x(vx * 2 + 1, tmx + 1, trx, max(lx, tmx + 1), rx, ly, ry));
    }

    int query(int lx, int rx, int ly, int ry) { return query_x(1, 1, n, lx, rx, ly, ry); } // rows lx..rx, cols ly..ry

    void update_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int new_val) {
        if (ly == ry) {
            if (lx == rx)
                t[vx][vy] = new_val;
            else
                t[vx][vy] = merge(t[vx * 2][vy], t[vx * 2 + 1][vy]);
        } else {
            int my = (ly + ry) / 2;
            if (y <= my)
                update_y(vx, lx, rx, vy * 2, ly, my, x, y, new_val);
            else
                update_y(vx, lx, rx, vy * 2 + 1, my + 1, ry, x, y, new_val);
            t[vx][vy] = merge(t[vx][vy * 2], t[vx][vy * 2 + 1]);
        }
    }

    void update_x(int vx, int lx, int rx, int x, int y, int new_val) {
        if (lx != rx) {
            int mx = (lx + rx) / 2;
            if (x <= mx)
                update_x(vx * 2, lx, mx, x, y, new_val);
            else
                update_x(vx * 2 + 1, mx + 1, rx, x, y, new_val);
        }
        update_y(vx, lx, rx, 1, 1, m, x, y, new_val);
    }

    void update(int x, int y, int new_val) { update_x(1, 1, n, x, y, new_val); } // a[x][y] = new_val
};

// Standard problem: type 1 assigns a cell, type 2 asks the sum of the rectangle (r1,c1)..(r2,c2)
void solve() {
    int n, m;
    cin >> n >> m;
    vector<vector<int>> a(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j) cin >> a[i][j];
    SegTree2D st(n, m, a);
    int q;
    cin >> q;
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x, y, new_val;
            cin >> x >> y >> new_val;
            st.update(x, y, new_val);
        } else {
            int r1, c1, r2, c2;
            cin >> r1 >> c1 >> r2 >> c2;
            cout << st.query(r1, r2, c1, c2) << '\n';
        }
    }
}
