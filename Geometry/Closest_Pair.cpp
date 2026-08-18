// Closest Pair: the two nearest points among n, by sweeping x and keeping a window ordered by y.
// Use when: "the two closest points", a minimum-distance threshold, or detecting duplicate positions.
// Handles: duplicate points (distance 0), collinear input, n < 2, and negative coordinates.
// Time: O(n log n)
// Indexing: 0-based; the result carries the indices into the ORIGINAL vector, not the sorted one
// Note: the distance comes back SQUARED so it stays exact in long long. Take the square root only when printing.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;
    int id;
};

struct Closest_Pair_Result {
    ll squared_distance; // exact; sqrt only for output
    int first, second;   // indices into the original vector, or -1 when n < 2
};

Closest_Pair_Result closest_pair(vector<Point> pts) {
    int n = (int)pts.size();
    if (n < 2) return {LLONG_MAX, -1, -1};
    for (int i = 0; i < n; i++) pts[i].id = i;
    sort(pts.begin(), pts.end(), [](const Point &a, const Point &b) { return a.x != b.x ? a.x < b.x : a.y < b.y; });

    auto dist2 = [](const Point &a, const Point &b) {
        ll dx = a.x - b.x, dy = a.y - b.y;
        return dx * dx + dy * dy;
    };

    // ordered by y so the candidates near a point's height sit together.
    // multiset, NOT set: two identical points must both stay, or duplicates vanish.
    auto by_y = [](const Point &a, const Point &b) { return a.y != b.y ? a.y < b.y : a.x < b.x; };
    multiset<Point, decltype(by_y)> window(by_y);

    Closest_Pair_Result best{dist2(pts[0], pts[1]), pts[0].id, pts[1].id}; // start finite, so no overflow below
    window.insert(pts[0]);
    int left = 0;
    for (int i = 1; i < n; i++) {
        // drop points too far back in x to ever help; compare squared, so no sqrt and no overflow
        while (left < i) {
            ll dx = pts[i].x - pts[left].x;
            if (dx * dx <= best.squared_distance) break;
            window.erase(window.find(pts[left++]));
        }
        ll reach = (ll)sqrtl((long double)best.squared_distance) + 2; // round up, then verify below
        while (reach > 0 && (reach - 1) * (reach - 1) >= best.squared_distance) reach--;

        // probe with the extreme x, since the comparator orders by (y, x)
        auto lo = window.lower_bound({LLONG_MIN, pts[i].y - reach, 0});
        auto hi = window.upper_bound({LLONG_MAX, pts[i].y + reach, 0});
        for (auto it = lo; it != hi; ++it) {
            ll d = dist2(*it, pts[i]);
            if (d < best.squared_distance) best = {d, it->id, pts[i].id};
        }
        window.insert(pts[i]);
    }
    if (best.first > best.second) swap(best.first, best.second);
    return best;
}

// convenience: build from plain coordinate pairs
Closest_Pair_Result closest_pair(const vector<pair<ll, ll>> &xy) {
    vector<Point> pts;
    for (auto [x, y] : xy) pts.push_back({x, y, 0});
    return closest_pair(pts);
}

// Standard problem: n points - report the smallest distance between any two of them
void solve() {
    int n;
    cin >> n;
    vector<pair<ll, ll>> pts(n);
    for (auto &[x, y] : pts) cin >> x >> y;
    Closest_Pair_Result r = closest_pair(pts);
    cout << fixed << setprecision(6) << sqrtl((long double)r.squared_distance) << '\n';
}
