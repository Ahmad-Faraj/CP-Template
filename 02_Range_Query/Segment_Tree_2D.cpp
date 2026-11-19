// Segment Tree 2D: rectangle sum (any associative merge) with point assignment on an n x m grid.
// Use when: "sum of the submatrix (x1, y1)..(x2, y2)" with point updates, grid up to ~1000 x 1000.
// Usage: SegTree2D st(n, m, a); st.update(x, y, v); st.query(x1, x2, y1, y2);
// Handles: point assign, rectangle query, empty ranges (x1 > x2 or y1 > y2 give Default).
// Not: rectangle updates, huge coordinates (see Segment_Tree_Dynamic_2D).
// Time: build O(n m) | update O(log n log m) | query O(log n log m) | memory 16 n m ints
// Indexing: 1-based rows/cols for query/update; the grid passed to the constructor is 0-based
// Note: query takes (row lo, row hi, col lo, col hi) - NOT (x1, y1, x2, y2).
// Note: change merge() and Default together; Default must be merge's identity.

#include <bits/stdc++.h>
using namespace std;

struct SegTree2D {
    int n, m;
    int Default = 0;
    vector<vector<int>> t;

    SegTree2D(int n, int m) : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)) {}
    SegTree2D(int n, int m, vector<vector<int>> &a)
        : n(n), m(m), t(4 * n, vector<int>(4 * m, Default)) {
        build(a);
    }

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

    // rows lx..rx, columns ly..ry
    int query(int lx, int rx, int ly, int ry) { return query_x(1, 1, n, lx, rx, ly, ry); }

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

    void update(int x, int y, int new_val) { update_x(1, 1, n, x, y, new_val); } // a[x][y] = v
};
