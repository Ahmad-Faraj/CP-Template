// Geometry: the floating-point core - points, vectors, lines, segments, angles, triangles and distances.
// Use when: anything analytic - rotate or reflect a point, where two lines meet, distance to a segment, an angle.
// Handles: exact-ish comparisons through dcmp/EPS, vertical and parallel lines, degenerate (zero-length) segments.
// Time: O(1) per operation, except cut_polygon which is O(n)
// Indexing: 0-based; a line is stored by direction and offset, not by two points
// Note: for hulls, polygon area and lattice counts use Convex_Hull.cpp / Polygon.cpp - those are exact in long long.

#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);
typedef complex<double> pt;
#define X real()
#define Y imag()

int dcmp(double a, double b) { return fabs(a - b) < EPS ? 0 : (a < b ? -1 : 1); }
int sign(double v) { return v > EPS ? 1 : (v < -EPS ? -1 : 0); }

double dot(pt a, pt b) { return (conj(a) * b).X; }
double cross(pt a, pt b) { return (conj(a) * b).Y; }
double sq(pt a) { return dot(a, a); }
double length(pt a) { return abs(a); }
double dist(pt a, pt b) { return abs(a - b); }

// > 0 if a->b->c turns left, < 0 right, 0 collinear
double orient(pt a, pt b, pt c) { return cross(b - a, c - a); }
int ccw(pt a, pt b, pt c) { return sign(orient(a, b, c)); }
bool collinear(pt a, pt b, pt c) { return sign(orient(a, b, c)) == 0; }

pt perp(pt a) { return pt(-a.Y, a.X); }                                       // rotate 90 degrees counter-clockwise
pt rotate_by(pt a, double theta) { return a * polar(1.0, theta); }            // about the origin
pt rotate_about(pt a, pt about, double theta) { return (a - about) * polar(1.0, theta) + about; }
pt scale_about(pt a, pt about, double factor) { return about + (a - about) * factor; }
pt reflect_about(pt a, pt p, pt q) { return conj((a - p) / (q - p)) * (q - p) + p; } // across the line p-q
pt midpoint(pt a, pt b) { return (a + b) / 2.0; }

double deg_to_rad(double d) { return d * PI / 180.0; }
double rad_to_deg(double r) { return r * 180.0 / PI; }
double normalize_angle(double t) { // into [0, 2*PI)
    t = fmod(t, 2 * PI);
    return t < 0 ? t + 2 * PI : t;
}
double angle_of(pt a) { return normalize_angle(arg(a)); }                 // of the vector a, from the +x axis
double angle_between(pt a, pt b) { // unsigned angle between two vectors, in [0, PI]
    double la = abs(a), lb = abs(b);
    if (la < EPS || lb < EPS) return 0;
    return acos(clamp(dot(a, b) / (la * lb), -1.0, 1.0));
}
double angle_at(pt a, pt o, pt b) { return angle_between(a - o, b - o); }  // the angle a-o-b, in [0, PI]
double oriented_angle(pt a, pt o, pt b) { // counter-clockwise from o->a to o->b, in [0, 2*PI)
    return normalize_angle(arg(b - o) - arg(a - o));
}

double triangle_area(pt a, pt b, pt c) { return fabs(orient(a, b, c)) / 2.0; }
double triangle_area_sides(double a, double b, double c) { // Heron
    double s = (a + b + c) / 2.0;
    return sqrt(max(0.0, s * (s - a) * (s - b) * (s - c)));
}
double triangle_area_medians(double m1, double m2, double m3) {
    return triangle_area_sides(m1, m2, m3) * 4.0 / 3.0;
}
double side_from_sides_angle(double b, double c, double A) { // the side opposite angle A, by the cosine rule
    return sqrt(max(0.0, b * b + c * c - 2 * b * c * cos(A)));
}
double angle_from_sides(double a, double b, double c) { // the angle opposite a, by the cosine rule
    if (b < EPS || c < EPS) return 0;
    return acos(clamp((b * b + c * c - a * a) / (2 * b * c), -1.0, 1.0));
}

struct line {
    pt v;    // direction
    double c; // side(p) == 0 exactly on the line
    line() : v(0), c(0) {}
    line(pt a, pt b) : v(b - a), c(cross(v, a)) {} // the line through a and b
    line(pt dir, double c) : v(dir), c(c) {}

    double side(pt p) { return cross(v, p) - c; }
    double dist(pt p) { return abs(v) < EPS ? abs(p) : fabs(side(p)) / abs(v); }
    line perp_through(pt p) { return line(p, p + perp(v)); }
    pt proj(pt p) { return p - perp(v) * side(p) / sq(v); }         // nearest point on the line
    pt reflect(pt p) { return p - perp(v) * 2.0 * side(p) / sq(v); }
    void shift_left(double d) { c += d * abs(v); }                  // move the line d to its left
    bool cmp_proj(pt p, pt q) { return dot(v, p) < dot(v, q); }     // order two points along the line
};

bool line_parallel(line l1, line l2) { return sign(cross(l1.v, l2.v)) == 0; }
bool line_perpendicular(line l1, line l2) { return sign(dot(l1.v, l2.v)) == 0; }

bool line_intersect(line l1, line l2, pt &out) { // false if parallel or identical
    double d = cross(l1.v, l2.v);
    if (sign(d) == 0) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

bool on_ray(pt a, pt b, pt p) { // p on the ray from a through b
    if (abs(b - a) < EPS) return abs(p - a) < EPS;
    return collinear(a, b, p) && dcmp(dot(b - a, p - a), 0) >= 0;
}
bool on_segment(pt a, pt b, pt p) {
    if (abs(b - a) < EPS) return abs(p - a) < EPS; // a zero-length segment holds only its own point
    if (!collinear(a, b, p)) return false;
    return dcmp(dot(p - a, b - a), 0) >= 0 && dcmp(dot(p - b, a - b), 0) >= 0;
}

// the intersection of segments a-b and c-d: empty, one point, or the two ends of the shared piece
vector<pt> segment_intersect(pt a, pt b, pt c, pt d) {
    double oa = orient(c, d, a), ob = orient(c, d, b), oc = orient(a, b, c), od = orient(a, b, d);
    if (sign(oa) * sign(ob) < 0 && sign(oc) * sign(od) < 0) return {(a * ob - b * oa) / (ob - oa)};
    set<pair<double, double>> found;
    if (on_segment(c, d, a)) found.insert({a.X, a.Y});
    if (on_segment(c, d, b)) found.insert({b.X, b.Y});
    if (on_segment(a, b, c)) found.insert({c.X, c.Y});
    if (on_segment(a, b, d)) found.insert({d.X, d.Y});
    vector<pt> out;
    for (auto &f : found) out.push_back(pt(f.first, f.second));
    return out;
}

bool segments_intersect(pt a, pt b, pt c, pt d) { return !segment_intersect(a, b, c, d).empty(); }

double point_line_dist(pt a, pt b, pt p) { // distance from p to the infinite line a-b
    if (abs(b - a) < EPS) return abs(p - a);
    return fabs(orient(a, b, p)) / abs(b - a);
}

double point_segment_dist(pt a, pt b, pt p) {
    if (abs(b - a) < EPS) return abs(p - a);
    line l(a, b);
    if (l.cmp_proj(a, p) && l.cmp_proj(p, b)) return l.dist(p); // the foot lands inside the segment
    return min(abs(p - a), abs(p - b));
}

double segment_segment_dist(pt a, pt b, pt c, pt d) {
    if (segments_intersect(a, b, c, d)) return 0;
    return min({point_segment_dist(a, b, c), point_segment_dist(a, b, d), point_segment_dist(c, d, a),
                point_segment_dist(c, d, b)});
}

// the part of polygon Q on the left of the directed line A->B; the result is closed (last == first)
vector<pt> cut_polygon(pt A, pt B, const vector<pt> &Q) {
    vector<pt> out;
    int n = (int)Q.size();
    for (int i = 0; i < n; i++) {
        pt cur = Q[i], nxt = Q[(i + 1) % n];
        double s1 = orient(A, B, cur), s2 = orient(A, B, nxt);
        if (sign(s1) >= 0) out.push_back(cur);
        if (sign(s1) * sign(s2) < 0) out.push_back((cur * s2 - nxt * s1) / (s2 - s1));
    }
    if (!out.empty() && abs(out.back() - out.front()) > EPS) out.push_back(out.front());
    return out;
}

// Standard problem: two segments per test case - report whether they touch, and where
void solve() {
    int t;
    cin >> t;
    cout << fixed << setprecision(6);
    while (t--) {
        double x1, y1, x2, y2, x3, y3, x4, y4;
        cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3 >> x4 >> y4;
        vector<pt> hit = segment_intersect(pt(x1, y1), pt(x2, y2), pt(x3, y3), pt(x4, y4));
        if (hit.empty())
            cout << "NO\n";
        else if (hit.size() == 1)
            cout << "YES " << hit[0].X << ' ' << hit[0].Y << '\n';
        else
            cout << "OVERLAP " << hit[0].X << ' ' << hit[0].Y << " .. " << hit[1].X << ' ' << hit[1].Y << '\n';
    }
}
