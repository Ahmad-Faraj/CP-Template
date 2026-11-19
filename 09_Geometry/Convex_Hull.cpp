// Convex Hull: convex hull of floating-point points (complex<double>).
// Use when: "smallest convex polygon containing all points" and the coordinates are not integers.
// Usage: CH ch; vector<pt> h = ch.hull_points(pts);
// Handles: duplicates (within EPS), all points equal (returns 1 point), all collinear (2 ends).
// Not: collinear points on hull edges are dropped. Integer coordinates: use Convex_Hull_2 (exact).
// Time: O(n log n)
// Indexing: hull is counter-clockwise from the lowest-x (then lowest-y) point, no repeat
// Note: sorts pts in place. EPS = 1e-9 is absolute, so keep coordinates within ~1e4.

#include <bits/stdc++.h>
using namespace std;

using pt = complex<double>;

struct CH {
    static constexpr double EPS = 1e-9;

  private:
    double cross(pt a, pt b) { return a.real() * b.imag() - a.imag() * b.real(); }
    double ccw(pt a, pt b, pt c) { return cross(b - a, c - a); }

  public:
    vector<pt> hull_points(vector<pt> &pts) {
        int n = pts.size();
        if (n <= 1) return pts;
        sort(pts.begin(), pts.end(), [](const pt &a, const pt &b) {
            if (a.real() != b.real()) return a.real() < b.real();
            return a.imag() < b.imag();
        });
        pts.erase(unique(pts.begin(), pts.end(),
                         [&](const pt &a, const pt &b) {
                             return fabs(a.real() - b.real()) < EPS &&
                                    fabs(a.imag() - b.imag()) < EPS;
                         }),
                  pts.end());
        int n2 = pts.size();
        if (n2 <= 1) return pts;

        vector<pt> h(2 * n2);
        int k = 0;
        for (int i = 0; i < n2; i++) {
            while (k >= 2 && ccw(h[k - 2], h[k - 1], pts[i]) <= EPS) --k;
            h[k++] = pts[i];
        }
        for (int i = n2 - 2, t = k + 1; i >= 0; --i) {
            while (k >= t && ccw(h[k - 2], h[k - 1], pts[i]) <= EPS) --k;
            h[k++] = pts[i];
        }
        if (k > 1) --k;
        h.resize(k);
        return h;
    }
};
