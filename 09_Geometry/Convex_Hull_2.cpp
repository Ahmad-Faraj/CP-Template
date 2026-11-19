// Convex Hull 2: convex hull of integer points, exact arithmetic, no epsilons.
// Use when: "smallest convex polygon containing all points", fence/perimeter, integer coordinates.
// Usage: vector<Point> h = convexHull(pts);
// Handles: duplicates, all points identical (returns 1 point), all collinear (returns 2 ends).
// Not: collinear points on hull edges are dropped, not kept.
// Time: O(n log n)
// Indexing: hull is counter-clockwise, starting at the lowest (then leftmost) point, no repeat
// Note: |coordinates| <= 1e9 keeps cross and squared distances inside long long.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Point {
    ll x, y;
    Point(ll x = 0, ll y = 0) : x(x), y(y) {}
    bool operator<(const Point &p) const { return x < p.x || (x == p.x && y < p.y); }
    bool operator==(const Point &p) const { return x == p.x && y == p.y; }
    Point operator-(const Point &p) const { return Point(x - p.x, y - p.y); }
};

// 2 times the signed area of triangle O, A, B (> 0 when O -> A -> B turns left)
ll cross(const Point &O, const Point &A, const Point &B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

bool ccw(const Point &p, const Point &q, const Point &r) { return cross(p, q, r) > 0; }

long double dist(const Point &p1, const Point &p2) {
    return hypotl((long double)(p1.x - p2.x), (long double)(p1.y - p2.y));
}

ll dist2(const Point &p1, const Point &p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

vector<Point> convexHull(vector<Point> points) {
    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());
    int n = points.size();
    if (n <= 1) return points;

    int basePoint = 0;
    for (int i = 1; i < n; i++)
        if (points[i].y < points[basePoint].y ||
            (points[i].y == points[basePoint].y && points[i].x < points[basePoint].x))
            basePoint = i;
    swap(points[0], points[basePoint]);

    Point p0 = points[0];
    sort(points.begin() + 1, points.end(), [&p0](const Point &p1, const Point &p2) {
        ll c = cross(p0, p1, p2);
        if (c != 0) return c > 0;
        return dist2(p0, p1) < dist2(p0, p2);
    });

    vector<Point> hull = {points[0], points[1]};
    for (int i = 2; i < n; i++) {
        while (hull.size() >= 2 && !ccw(hull[hull.size() - 2], hull.back(), points[i]))
            hull.pop_back();
        hull.push_back(points[i]);
    }
    return hull;
}
