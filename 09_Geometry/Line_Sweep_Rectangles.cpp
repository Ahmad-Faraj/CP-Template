// Line Sweep Rectangles: area of the union of axis-aligned rectangles.
// Use when: "total area covered by n rectangles", overlapping posters/photos, union of boxes.
// Usage: Rect_Union ru; ll a = ru.area(rects);  // rects: vector<Rectangle>{x1, y1, x2, y2}
// Handles: no rectangles, zero-width/height rectangles, nesting, duplicates, negative coordinates.
// Time: O(n log n)
// Indexing: Rectangle(x1, y1, x2, y2) with x1 <= x2, y1 <= y2 (lower-left, upper-right corners)
// Note: |coordinates| <= 1e9 keeps the area inside long long. intersect() returns
//       Rectangle() (all zero) when two proper rectangles only touch or are apart.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Rectangle {
    ll x1, y1, x2, y2;
    Rectangle(ll X1 = 0, ll Y1 = 0, ll X2 = 0, ll Y2 = 0) : x1(X1), y1(Y1), x2(X2), y2(Y2) {}
    Rectangle intersect(const Rectangle &R) const {
        if (R.x1 >= x2 || R.x2 <= x1 || R.y1 >= y2 || R.y2 <= y1) return Rectangle();
        return Rectangle(max(x1, R.x1), max(y1, R.y1), min(x2, R.x2), min(y2, R.y2));
    }
};

struct Rect_Union {
    struct Node {
        int prob = 0;
        ll sum = 0, ans = 0;
    };
    vector<Node> tree;
    vector<ll> sorted, interval;

    void build(int x, int a, int b) {
        tree[x] = Node();
        if (a == b) {
            tree[x].sum = interval[a];
            return;
        }
        build(x * 2, a, (a + b) / 2);
        build(x * 2 + 1, (a + b) / 2 + 1, b);
        tree[x].sum = tree[x * 2].sum + tree[x * 2 + 1].sum;
    }
    ll ask(int x) { return tree[x].prob ? tree[x].sum : tree[x].ans; }
    void update(int x, int a, int b, int st, int en, int v) {
        if (st > b || en < a) return;
        if (a >= st && b <= en) {
            tree[x].prob += v;
            return;
        }
        update(x * 2, a, (a + b) / 2, st, en, v);
        update(x * 2 + 1, (a + b) / 2 + 1, b, st, en, v);
        tree[x].ans = ask(x * 2) + ask(x * 2 + 1);
    }

    ll area(const vector<Rectangle> &rects) {
        sorted.clear();
        for (auto &R : rects) sorted.push_back(R.y1), sorted.push_back(R.y2);
        sort(sorted.begin(), sorted.end());
        sorted.erase(unique(sorted.begin(), sorted.end()), sorted.end());
        int m = (int)sorted.size() - 1;
        if (m < 1) return 0;
        interval.assign(m + 1, 0);
        for (int j = 0; j < m; j++) interval[j + 1] = sorted[j + 1] - sorted[j];
        tree.assign(4 * m, Node());
        build(1, 1, m);

        vector<array<ll, 4>> sweep;
        for (auto &R : rects) {
            sweep.push_back({R.x1, R.y1, R.y2, 1});
            sweep.push_back({R.x2, R.y1, R.y2, -1});
        }
        sort(sweep.begin(), sweep.end(), [](auto &A, auto &B) { return A[0] < B[0]; });
        ll ans = 0, last = sweep[0][0];
        for (auto &e : sweep) {
            ans += (e[0] - last) * ask(1);
            last = e[0];
            int st = lower_bound(sorted.begin(), sorted.end(), e[1]) - sorted.begin() + 1;
            int en = lower_bound(sorted.begin(), sorted.end(), e[2]) - sorted.begin();
            update(1, 1, m, st, en, (int)e[3]);
        }
        return ans;
    }
};
