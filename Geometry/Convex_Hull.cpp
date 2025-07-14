#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz(st) (int)st.size()
#define all(st) st.begin(), st.end()
const double EPS = 1E-9;
typedef complex<double> pt;
#define X real()
#define Y imag()

struct CH {
  private:
    double cross(pt a, pt b) { return a.X * b.Y - a.Y * b.X; }
    double ccw(pt a, pt b, pt c) { return cross(b - a, c - a); }

  public:
    vector<pt> hull_points(vector<pt> &pts) {
        int n = sz(pts);
        if (n <= 1) return pts;
        sort(pts.begin(), pts.end(), [](const pt &a, const pt &b) {
            if (a.X != b.X) return a.X < b.X;
            return a.Y < b.Y;
        });
        pts.erase(unique(pts.begin(), pts.end(),
                         [&](const pt &a, const pt &b) { return fabs(a.X - b.X) < EPS && fabs(a.Y - b.Y) < EPS; }),
                  pts.end());
        int n2 = sz(pts);
        if (n2 <= 1) return pts;

        vector<pt> h(2 * n2);
        int k = 0;

        // Build lower hull
        for (int i = 0; i < n2; i++) {
            while (k >= 2 && ccw(h[k - 2], h[k - 1], pts[i]) <= EPS) {
                --k;
            }
            h[k++] = pts[i];
        }
        // Build upper hull
        for (int i = n2 - 2, t = k + 1; i >= 0; --i) {
            while (k >= t && ccw(h[k - 2], h[k - 1], pts[i]) <= EPS) {
                --k;
            }
            h[k++] = pts[i];
        }
        // Remove duplicate end-point
        if (k > 1) --k;
        h.resize(k);
        return h;
    }
};