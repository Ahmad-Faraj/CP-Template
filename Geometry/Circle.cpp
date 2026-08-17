// Circle: intersections with lines and other circles, overlap area, circumcircle, and the minimum enclosing circle.
// Use when: "do the circles overlap and by how much", "where does the line cut the circle", smallest enclosing circle.
// Handles: tangency, nested and identical circles, collinear circumcircle input, duplicate points in the MEC.
// Time: O(1) each, except min_enclosing_circle which is O(n) expected
// Indexing: 0-based
// Note: identical circles share infinitely many points - circle_circle_intersection returns {} and sets same = true.

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);
typedef complex<double> pt;
#define X real()
#define Y imag()

double dot(pt a, pt b) { return (conj(a) * b).X; }
double cross(pt a, pt b) { return (conj(a) * b).Y; }

struct circle {
    pt c;
    double r;
    circle(pt c = pt(0, 0), double r = 0) : c(c), r(r) {}
    double area() const { return PI * r * r; }
    double circumference() const { return 2 * PI * r; }
};

// 0 outside, 1 on the rim, 2 strictly inside
int point_in_circle(const circle &o, pt p) {
    double d = abs(p - o.c);
    if (fabs(d - o.r) < EPS) return 1;
    return d < o.r ? 2 : 0;
}

// where the infinite line a-b meets the circle: 0, 1 (tangent) or 2 points
vector<pt> circle_line_intersection(const circle &o, pt a, pt b) {
    if (abs(b - a) < EPS) return point_in_circle(o, a) == 1 ? vector<pt>{a} : vector<pt>{};
    pt d = b - a;
    double A = dot(d, d), B = 2 * dot(d, a - o.c), C = dot(a - o.c, a - o.c) - o.r * o.r;
    double det = B * B - 4 * A * C;
    if (det < -EPS) return {};
    if (det < EPS) return {a + d * (-B / (2 * A))};
    det = sqrt(det);
    return {a + d * ((-B - det) / (2 * A)), a + d * ((-B + det) / (2 * A))};
}

vector<pt> circle_segment_intersection(const circle &o, pt a, pt b) { // only the hits inside the segment
    vector<pt> out;
    for (pt p : circle_line_intersection(o, a, b)) {
        double t = abs(b - a) < EPS ? 0 : dot(p - a, b - a) / dot(b - a, b - a);
        if (t >= -EPS && t <= 1 + EPS) out.push_back(p);
    }
    return out;
}

// 0, 1 or 2 points. same is set when the two circles are identical, which has infinitely many.
vector<pt> circle_circle_intersection(const circle &o1, const circle &o2, bool &same) {
    same = false;
    double d = abs(o2.c - o1.c);
    if (d < EPS && fabs(o1.r - o2.r) < EPS) {
        if (o1.r < EPS) return {o1.c}; // two identical zero-radius circles are one point
        same = true;
        return {};
    }
    if (d > o1.r + o2.r + EPS) return {};        // too far apart
    if (d < fabs(o1.r - o2.r) - EPS) return {};  // one strictly inside the other
    double a = (d * d + o1.r * o1.r - o2.r * o2.r) / (2 * d);
    double h2 = o1.r * o1.r - a * a;
    pt dir = (o2.c - o1.c) / d;
    pt mid = o1.c + dir * a;
    if (h2 < EPS) return {mid}; // tangent
    pt off = pt(-dir.Y, dir.X) * sqrt(h2);
    return {mid - off, mid + off};
}

double circle_circle_area(const circle &o1, const circle &o2) { // area of the overlap
    double d = abs(o2.c - o1.c);
    if (d >= o1.r + o2.r - EPS) return 0;
    if (d <= fabs(o1.r - o2.r) + EPS) {
        double rmin = min(o1.r, o2.r);
        return PI * rmin * rmin;
    }
    double c1 = clamp((d * d + o1.r * o1.r - o2.r * o2.r) / (2 * d * o1.r), -1.0, 1.0);
    double c2 = clamp((d * d + o2.r * o2.r - o1.r * o1.r) / (2 * d * o2.r), -1.0, 1.0);
    double al = 2 * acos(c1), be = 2 * acos(c2);
    return 0.5 * o1.r * o1.r * (al - sin(al)) + 0.5 * o2.r * o2.r * (be - sin(be));
}

// the circle through three points; radius 0 and centre a when they are collinear
circle circumcircle(pt a, pt b, pt c) {
    pt ba = b - a, ca = c - a;
    double d = 2 * cross(ba, ca);
    if (fabs(d) < EPS) return circle(a, 0);
    pt cen = a + pt(dot(ba, ba) * ca.Y - dot(ca, ca) * ba.Y, dot(ca, ca) * ba.X - dot(ba, ba) * ca.X) / d;
    return circle(cen, abs(cen - a));
}

circle circle_from_diameter(pt a, pt b) { return circle((a + b) / 2.0, abs(b - a) / 2); }

// the smallest circle containing every point
circle min_enclosing_circle(vector<pt> p) {
    static mt19937 rng(20260817);
    shuffle(p.begin(), p.end(), rng);
    int n = (int)p.size();
    if (n == 0) return circle();
    circle o(p[0], 0);
    auto has = [](const circle &o, pt q) { return abs(q - o.c) <= o.r + 1e-7; };
    for (int i = 1; i < n; i++) {
        if (has(o, p[i])) continue;
        o = circle(p[i], 0);
        for (int j = 0; j < i; j++) {
            if (has(o, p[j])) continue;
            o = circle_from_diameter(p[i], p[j]);
            for (int k = 0; k < j; k++)
                if (!has(o, p[k])) o = circumcircle(p[i], p[j], p[k]);
        }
    }
    return o;
}

// Standard problem: two circles per test case - report their overlap area and their intersection points
void solve() {
    int t;
    cin >> t;
    cout << fixed << setprecision(6);
    while (t--) {
        double x1, y1, r1, x2, y2, r2;
        cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
        circle a(pt(x1, y1), r1), b(pt(x2, y2), r2);
        bool same;
        vector<pt> hit = circle_circle_intersection(a, b, same);
        cout << circle_circle_area(a, b) << ' ' << (same ? -1 : (int)hit.size());
        for (pt p : hit) cout << "  (" << p.X << ", " << p.Y << ')';
        cout << '\n';
    }
}
