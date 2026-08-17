// Convex Hull: the smallest convex polygon containing a set of points, by monotone chain.
// Use when: "smallest fence around all points", hull perimeter or area, or discarding every interior point.
// Handles: duplicates, all-collinear input, n < 3, keeping or dropping points on a hull edge, exact polar sorting.
// Time: O(n log n)
// Indexing: 0-based; counter-clockwise from the leftmost-then-lowest point, which is not repeated at the end
// Note: exact in long long, no EPS. Coordinates up to 1e9 are safe; cross products overflow only beyond that.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct Point {
    T x, y;
    Point(T x = 0, T y = 0) : x(x), y(y) {}
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    bool operator<(const Point &p) const { return x != p.x ? x < p.x : y < p.y; }
};

template <typename T> T cross(const Point<T> &a, const Point<T> &b) { return a.x * b.y - a.y * b.x; }

// > 0 if a->b->c turns left, < 0 right, 0 collinear
template <typename T> T orient(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return cross(b - a, c - a);
}

// 0 for the upper half plane (including +x), 1 for the lower - the key to an exact angular sort
template <typename T> int half(const Point<T> &p) { return (p.y > 0 || (p.y == 0 && p.x >= 0)) ? 0 : 1; }

// orders points by angle counter-clockwise from the +x axis; ties break nearer-first
template <typename T> bool polar_cmp(const Point<T> &a, const Point<T> &b) {
    int ha = half(a), hb = half(b);
    if (ha != hb) return ha < hb;
    T c = cross(a, b);
    if (c != 0) return c > 0;
    return a.x * a.x + a.y * a.y < b.x * b.x + b.y * b.y;
}

template <typename T> void polar_sort(vector<Point<T>> &pts) { // about the origin
    sort(pts.begin(), pts.end(), polar_cmp<T>);
}

template <typename T> void polar_sort(vector<Point<T>> &pts, const Point<T> &about) {
    sort(pts.begin(), pts.end(),
         [&](const Point<T> &a, const Point<T> &b) { return polar_cmp(a - about, b - about); });
}

template <typename T = ll> vector<Point<T>> convex_hull(vector<Point<T>> pts, bool keep_collinear = false) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = (int)pts.size();
    if (n <= 2) return pts;

    bool all_collinear = true;
    for (int i = 2; i < n && all_collinear; i++)
        if (orient(pts[0], pts[1], pts[i]) != 0) all_collinear = false;
    if (all_collinear) return keep_collinear ? pts : vector<Point<T>>{pts.front(), pts.back()};

    vector<Point<T>> h(2 * n);
    auto bad = [&](int k, const Point<T> &p) { // p would not extend the chain to the left
        T o = orient(h[k - 2], h[k - 1], p);
        return keep_collinear ? o < 0 : o <= 0;
    };
    int k = 0;
    for (int i = 0; i < n; i++) { // lower chain, left to right
        while (k >= 2 && bad(k, pts[i])) k--;
        h[k++] = pts[i];
    }
    for (int i = n - 2, floor_k = k + 1; i >= 0; i--) { // upper chain, right back to the start
        while (k >= floor_k && bad(k, pts[i])) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1); // the first point is repeated at the end
    return h;
}

template <typename T> T hull_area2(const vector<Point<T>> &p) { // twice the area, so it stays exact
    T a = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) a += cross(p[i], p[(i + 1) % n]);
    return a < 0 ? -a : a;
}

double hull_perimeter(const vector<Point<ll>> &p) {
    double per = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) {
        Point<ll> d = p[(i + 1) % n] - p[i];
        per += hypot((double)d.x, (double)d.y);
    }
    return per;
}

// Standard problem: read n points, report the hull's vertex count and its vertices counter-clockwise
void solve() {
    int n;
    cin >> n;
    vector<Point<ll>> pts(n);
    for (auto &p : pts) cin >> p.x >> p.y;
    vector<Point<ll>> h = convex_hull(pts);
    cout << h.size() << '\n';
    for (auto &p : h) cout << p.x << ' ' << p.y << '\n';
}
