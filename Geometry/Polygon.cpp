// Polygon: area, perimeter, convexity, point location and lattice-point counts for a simple polygon.
// Use when: "area of this polygon", "is the point inside", "how many lattice points does it contain", "is it convex".
// Handles: any simple polygon, clockwise or counter-clockwise, collinear vertices. Not self-intersecting ones.
// Time: O(n) each, except in_polygon which is O(n) per query
// Indexing: 0-based, vertices in order, the first vertex NOT repeated at the end
// Note: exact in long long - area2 is twice the area so it stays integral. To cut a polygon by a line see Geometry.cpp.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct Point {
    T x, y;
    Point(T x = 0, T y = 0) : x(x), y(y) {}
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
};

template <typename T> T cross(const Point<T> &a, const Point<T> &b) { return a.x * b.y - a.y * b.x; }

template <typename T> T orient(const Point<T> &a, const Point<T> &b, const Point<T> &c) {
    return cross(b - a, c - a);
}

template <typename T> bool on_segment(const Point<T> &a, const Point<T> &b, const Point<T> &p) {
    return orient(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y &&
           p.y <= max(a.y, b.y);
}

template <typename T> T area2(const vector<Point<T>> &p) { // twice the area, exact and non-negative
    T a = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) a += cross(p[i], p[(i + 1) % n]);
    return a < 0 ? -a : a;
}

template <typename T> T signed_area2(const vector<Point<T>> &p) { // > 0 counter-clockwise, < 0 clockwise
    T a = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) a += cross(p[i], p[(i + 1) % n]);
    return a;
}

template <typename T> double area(const vector<Point<T>> &p) { return area2(p) / 2.0; }

template <typename T> double perimeter(const vector<Point<T>> &p) {
    double per = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) {
        Point<T> d = p[(i + 1) % n] - p[i];
        per += hypot((double)d.x, (double)d.y);
    }
    return per;
}

template <typename T> bool is_convex(const vector<Point<T>> &p) { // collinear vertices are allowed
    int n = (int)p.size();
    if (n < 3) return false;
    int sign = 0;
    for (int i = 0; i < n; i++) {
        T o = orient(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        if (o == 0) continue;
        int s = o > 0 ? 1 : -1;
        if (sign == 0)
            sign = s;
        else if (sign != s)
            return false;
    }
    return true;
}

// 0 outside, 1 on the boundary, 2 strictly inside
template <typename T> int in_polygon(const vector<Point<T>> &p, const Point<T> &q) {
    int n = (int)p.size(), crossings = 0;
    for (int i = 0; i < n; i++) {
        const Point<T> &a = p[i], &b = p[(i + 1) % n];
        if (on_segment(a, b, q)) return 1;
        if ((a.y > q.y) != (b.y > q.y)) { // the edge straddles the ray's height
            T o = orient(a, b, q);
            if ((o > 0) == (b.y > a.y)) crossings ^= 1;
        }
    }
    return crossings ? 2 : 0;
}

template <typename T> T boundary_lattice_points(const vector<Point<T>> &p) { // lattice points on the outline
    T total = 0;
    for (size_t i = 0, n = p.size(); i < n; i++) {
        Point<T> d = p[(i + 1) % n] - p[i];
        total += __gcd(d.x < 0 ? -d.x : d.x, d.y < 0 ? -d.y : d.y);
    }
    return total;
}

// Pick's theorem: lattice points strictly inside. Vertices must be integral.
template <typename T> T interior_lattice_points(const vector<Point<T>> &p) {
    return (area2(p) - boundary_lattice_points(p) + 2) / 2;
}

template <typename T> Point<double> centroid(const vector<Point<T>> &p) { // centre of mass of the filled polygon
    int n = (int)p.size();
    double a = 0, cx = 0, cy = 0;
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        double c = (double)cross(p[i], p[j]);
        a += c;
        cx += (p[i].x + p[j].x) * c;
        cy += (p[i].y + p[j].y) * c;
    }
    if (fabs(a) < 1e-12) { // degenerate: fall back to the midpoint of the extremes
        double lx = p[0].x, hx = p[0].x, ly = p[0].y, hy = p[0].y;
        for (auto &q : p) lx = min(lx, (double)q.x), hx = max(hx, (double)q.x), ly = min(ly, (double)q.y),
                          hy = max(hy, (double)q.y);
        return Point<double>((lx + hx) / 2, (ly + hy) / 2);
    }
    return Point<double>(cx / (3 * a), cy / (3 * a));
}

// Standard problem: CSES Polygon Lattice Points - the lattice points strictly inside, then those on the boundary
void solve() {
    int n;
    cin >> n;
    vector<Point<ll>> p(n);
    for (auto &q : p) cin >> q.x >> q.y;
    cout << interior_lattice_points(p) << ' ' << boundary_lattice_points(p) << '\n';
}
