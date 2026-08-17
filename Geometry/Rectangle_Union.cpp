// Rectangle Union: the area covered by at least one of n axis-aligned rectangles, by sweeping x.
// Use when: "total area covered by these rectangles", overlapping stamps or posters, area of a union of boxes.
// Handles: overlaps, nesting, duplicates, touching edges, negative coordinates, n = 0. Axis-aligned only.
// Time: O(n log n)
// Indexing: 0-based; a rectangle is [x1, x2) x [y1, y2) with x1 < x2 and y1 < y2
// Note: areas reach 4e18 for coordinates at 1e9, so the total is long long and only just fits.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Rectangle_Union {
    struct Rect {
        ll x1, y1, x2, y2;
    };
    struct Event {
        ll x, y1, y2;
        int type; // +1 when the rectangle opens, -1 when it closes
    };

    vector<Rect> rects;

    void add(ll x1, ll y1, ll x2, ll y2) { // the rectangle [x1, x2) x [y1, y2)
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
        if (x1 == x2 || y1 == y2) return; // zero area, nothing to cover
        rects.push_back({x1, y1, x2, y2});
    }

    vector<ll> ys;      // compressed y coordinates
    vector<int> cover;  // how many rectangles fully cover this node's span
    vector<ll> covered; // length of this node's span that is covered

    void pull(int node, int l, int r) {
        if (cover[node] > 0)
            covered[node] = ys[r + 1] - ys[l];
        else if (l == r)
            covered[node] = 0;
        else
            covered[node] = covered[2 * node] + covered[2 * node + 1];
    }

    void update(int node, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr)
            cover[node] += val;
        else {
            int mid = (l + r) / 2;
            update(2 * node, l, mid, ql, qr, val);
            update(2 * node + 1, mid + 1, r, ql, qr, val);
        }
        pull(node, l, r);
    }

    ll area() { // total area covered by at least one rectangle
        if (rects.empty()) return 0;
        ys.clear();
        for (const Rect &r : rects) ys.push_back(r.y1), ys.push_back(r.y2);
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());
        int m = (int)ys.size() - 1; // number of elementary y strips
        if (m <= 0) return 0;

        vector<Event> events;
        for (const Rect &r : rects) {
            events.push_back({r.x1, r.y1, r.y2, +1});
            events.push_back({r.x2, r.y1, r.y2, -1});
        }
        sort(events.begin(), events.end(), [](const Event &a, const Event &b) { return a.x < b.x; });

        cover.assign(4 * m, 0);
        covered.assign(4 * m, 0);

        ll total = 0, last = events[0].x;
        for (const Event &e : events) {
            total += (e.x - last) * covered[1]; // the strip since the previous event
            last = e.x;
            int lo = (int)(lower_bound(ys.begin(), ys.end(), e.y1) - ys.begin());
            int hi = (int)(lower_bound(ys.begin(), ys.end(), e.y2) - ys.begin()) - 1;
            if (lo <= hi) update(1, 0, m - 1, lo, hi, e.type);
        }
        return total;
    }
};

// Standard problem: n axis-aligned rectangles by opposite corners - report the area of their union
void solve() {
    int n;
    cin >> n;
    Rectangle_Union ru;
    for (int i = 0; i < n; i++) {
        ll x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        ru.add(x1, y1, x2, y2);
    }
    cout << ru.area() << '\n';
}
