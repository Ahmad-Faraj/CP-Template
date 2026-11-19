// Geometry Lib: 2D toolkit on complex<double>: lines, segments, angles, circles, polygons.
// Use when: non-integer coordinates, rotations, angles, circle intersections, cutting a polygon.
// Usage: using namespace geo_lib; line l(pt(0, 0), pt(3, 4)); l.side(p); seg_inters(a, b, c, d);
// Handles: vertical lines, parallel lines, touching/overlapping segments, tangent circles.
// Not: exact. For integer coordinates up to 1e9 use Points_And_Polygons (exact) / Convex_Hull_2.
// Time: O(1) per call, polygons O(n)
// Indexing: polygon = vertices in order (any orientation); cut_polygon repeats the first at the end
// Note: EPS = 1e-9 is absolute, so keep coordinates within ~1e4. Angles are radians.
// Note: in_angle on the two boundary rays answers 1 when b->c turns left, 0 when right.
// Note: is_convex assumes a simple polygon (a pentagram passes); an all-collinear one passes too.
// Note: line from two equal points, circle_points_radius_center(p, p, ..) divide by zero.

#include <bits/stdc++.h>
using namespace std;

namespace geo_lib {
const double EPS = 1E-9, PI = acos(-1);
using T = double; // keep double: some functions need it
typedef complex<T> pt;

int dblcmp(T a, T b) { return fabs(a - b) < EPS ? 0 : a < b ? -1 : 1; }
int sign(T val) { return (val > EPS ? +1 : val < -EPS ? -1 : 0); }
bool in_range(T l, T r, T v) { return (dblcmp(l, v) != 1 && dblcmp(v, r) != 1); }

// * ------------------------- Points & Vectors -------------------------

T dot(pt a, pt b) { return a.real() * b.real() + a.imag() * b.imag(); }
T cross(pt a, pt b) { return a.real() * b.imag() - a.imag() * b.real(); }
T sq(pt a) { return dot(a, a); }
double dist(pt a, pt b) { return abs(a - b); }
double length(pt a) { return abs(a); }

// + if c is on the left of a->b, - if on the right, 0 if collinear
T ccw(pt a, pt b, pt c) { return cross(b - a, c - a); }

pt rotate_by(pt a, double theta) { return a * polar(1.0, theta); }
pt rotate_around(pt a, pt b, double theta) { return (a - b) * polar(1.0, theta) + b; }
pt perp(pt p) { return pt(-p.imag(), p.real()); }
pt translate(pt a, pt b) { return a + b; }
// scale p by factor around center
pt scale_around(pt center, double factor, pt p) { return center + (p - center) * factor; }

bool collinear(pt a, pt b, pt c) { return fabs(ccw(a, b, c)) < EPS; }

bool on_ray(pt a, pt b, pt p) {
    if (!collinear(a, b, p)) return false;
    return dblcmp(dot(b - a, p - a), 0) >= 0;
}
// is p inside angle bac (counter-clockwise from ab to ac); -1 when a, b, c are collinear
int in_angle(pt a, pt b, pt c, pt p) {
    if (fabs(ccw(a, b, c)) < EPS) return -1;
    T abp = ccw(a, b, p), acp = ccw(a, c, p), abc = ccw(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= -EPS && acp <= EPS) ^ (abc < 0);
}

// sort by angle in [0, 2pi) starting from the +x axis (the origin counts as angle 0)
bool polar_comp(const pt &a, const pt &b) {
    int ha = a.imag() > EPS || (fabs(a.imag()) < EPS && a.real() >= -EPS);
    int hb = b.imag() > EPS || (fabs(b.imag()) < EPS && b.real() >= -EPS);
    if (ha != hb) return ha > hb;
    return cross(a, b) > 0;
}

// * ------------------------- Lines -------------------------
struct line {
    pt v;
    T c;
    // From direction vector v and offset c : cross(v,p) = c
    line(pt v, T c) : v(v), c(c) {}
    // From equation ax+by=cf
    line(T a, T b, T c) : v({b, -a}), c(c) {}
    // From points p and q
    line(pt p, pt q) : v(q - p), c(cross(q - p, p)) {}
    // which side the point is at : 0 on line , -ve right , +ve left
    T side(pt p) { return cross(v, p) - c; }
    double dist(pt p) { return abs(v) < EPS ? 0 : abs(side(p)) / abs(v); }
    double sq_dist(pt p) { return abs(v) < EPS ? 0 : (side(p) * side(p)) / sq(v); }
    // find the perp. line on self that goes through point p
    line perp_through(pt p) { return {p, p + perp(v)}; }
    bool cmp_proj(const pt &p, const pt &q) { return dot(v, p) < dot(v, q); }
    line line_translate(pt t) { return {v, c + cross(v, t)}; }
    void shift_left(double dist) { c += dist * abs(v); }
    pt proj(pt p) { return p - perp(v) * side(p) / sq(v); }
};

bool line_intersect(line l1, line l2, pt &out) {
    T d = cross(l1.v, l2.v);
    if (fabs(d) < EPS) return false;
    out = (l2.v * l1.c - l1.v * l2.c) / d;
    return true;
}

bool line_perp(line l1, line l2) { return fabs(dot(l1.v, l2.v)) < EPS; }
bool line_parallel(line l1, line l2) { return fabs(cross(l1.v, l2.v)) < EPS; }
// * ------------------------- Segments -------------------------

bool on_segment(pt a, pt b, pt p) {
    if (!collinear(a, b, p)) return false;
    if (abs(a - b) < EPS) return abs(p - a) < EPS;
    return dblcmp(dot(p - a, b - a), 0) >= 0 && dblcmp(dot(p - b, a - b), 0) >= 0;
}

bool proper_intersect(pt a, pt b, pt c, pt d, pt &out) {
    double oa = ccw(c, d, a), ob = ccw(c, d, b);
    double oc = ccw(a, b, c), od = ccw(a, b, d);
    if (sign(oa) * sign(ob) < 0 && sign(oc) * sign(od) < 0) {
        out = (a * ob - b * oa) / (ob - oa);
        return true;
    }
    return false;
}

vector<pt> seg_inters(pt a, pt b, pt c, pt d) {
    pt out;
    if (proper_intersect(a, b, c, d, out)) return {out};
    vector<pt> ret;
    if (on_segment(c, d, a)) ret.push_back(a);
    if (on_segment(c, d, b)) ret.push_back(b);
    if (on_segment(a, b, c)) ret.push_back(c);
    if (on_segment(a, b, d)) ret.push_back(d);
    sort(ret.begin(), ret.end(), [](pt p, pt q) {
        return make_pair(p.real(), p.imag()) < make_pair(q.real(), q.imag());
    });
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

double segment_point_dist(pt a, pt b, pt p) {
    if (a != b) {
        line l(a, b);
        if (l.cmp_proj(a, p) && l.cmp_proj(p, b))
            return l.dist(p);
    }
    return min(abs(p - a), abs(p - b));
}

double seg_seg_dist(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (proper_intersect(a, b, c, d, dummy)) return 0;
    return min({segment_point_dist(a, b, c), segment_point_dist(a, b, d),
                segment_point_dist(c, d, a), segment_point_dist(c, d, b)});
}
// * ------------------------- Angles -------------------------

double deg_to_rad(double theta) { return theta * PI / 180.0; }
double rad_to_deg(double theta) { return theta * 180.0 / PI; }
double normalize_angle(double theta) {
    theta = fmod(theta, 2.0 * PI);
    if (theta < 0) theta += 2.0 * PI;
    return theta;
}
double angle(pt a) {
    if (abs(a) < EPS) return 0;
    return atan2(a.imag(), a.real());
}
// counter-clockwise angle from vector a to vector b, in [0, 2pi)
double angle(pt a, pt b) { return normalize_angle(angle(b) - angle(a)); }
// counter-clockwise angle from ray a->b to ray a->c, in [0, 2pi)
double oriented_angle(pt a, pt b, pt c) { return angle(b - a, c - a); }
// * ------------------------- Circles -------------------------

// inside : 1 , outside -1 , boundary 0
int point_circle_location(pt center, double radius, pt p) {
    return dblcmp(radius, abs(p - center));
}

// returns the number of common points (0, 1, 2), written to out
int intersect_circle_line(pt o, double r, line l, pair<pt, pt> &out) {
    double h2 = r * r - l.sq_dist(o);
    if (h2 < -EPS) return 0;
    if (fabs(h2) < EPS) h2 = 0;
    pt p = l.proj(o);
    pt h = l.v * sqrt(h2) / abs(l.v);
    out = {p - h, p + h};
    return 1 + (h2 > EPS);
}

// returns the number of common points (0, 1, 2); same circle INT_MAX, concentric -1
int intersect_circles(pt o1, double r1, pt o2, double r2, pair<pt, pt> &out) {
    pt d = o2 - o1;
    double d2 = sq(d);
    if (d2 < EPS) return fabs(r1 - r2) <= EPS ? INT_MAX : -1;
    double pd = (d2 + r1 * r1 - r2 * r2) / 2;
    double h2 = r1 * r1 - pd * pd / d2;
    if (h2 < -EPS) return 0;
    if (fabs(h2) < EPS) h2 = 0;
    pt p = o1 + d * pd / d2;
    pt h = perp(d) * sqrt(h2 / d2);
    out = {p - h, p + h};
    return 1 + (h2 > EPS);
}
double radius_circumcircle(double ab, double bc, double ca) {
    double p = (ab + bc + ca) / 2.0;
    double area2 = p * (p - ab) * (p - bc) * (p - ca);
    if (area2 <= EPS) return INFINITY;
    double area = sqrt(area2);
    return ab * bc * ca / (4.0 * area);
}

double radius_circumcircle(pt a, pt b, pt c) {
    return radius_circumcircle(dist(a, b), dist(b, c), dist(c, a));
}
// centre of a circle of radius r through p1 and p2: the one on the left of p1->p2
// (swap p1, p2 for the other one); false when |p1 p2| > 2r
bool circle_points_radius_center(pt p1, pt p2, double r, pt &ret) {
    double d2 = sq(p1 - p2);
    double det = r * r / d2 - 0.25;
    if (det < -EPS) return false;
    if (fabs(det) < EPS) det = 0;
    double h = sqrt(det);
    ret = pt((p1.real() + p2.real()) * 0.5 + (p1.imag() - p2.imag()) * h,
             (p1.imag() + p2.imag()) * 0.5 + (p2.real() - p1.real()) * h);
    return true;
}
// * ------------------------- Polygons -------------------------
bool is_convex(const vector<pt> &p) {
    if (p.size() < 3) return false;
    int n = p.size(), sign = 0;
    for (int i = 0; i < n; ++i) {
        double cross_val = ccw(p[i], p[(i + 1) % n], p[(i + 2) % n]);
        if (fabs(cross_val) < EPS) continue;
        int o = cross_val > 0 ? 1 : -1;
        if (sign == 0)
            sign = o;
        else if (sign != o)
            return false;
    }
    return true;
}

// is a inside polygon p; strict = 1: boundary counts as outside, strict = 0: as inside
bool in_polygon(const vector<pt> &p, pt a, bool strict = 1) {
    auto above = [](pt p, pt q) { return p.imag() >= q.imag() - EPS; };
    auto ray_cut = [above](pt a, pt p, pt q) {
        return (above(a, q) - above(a, p)) * ccw(a, p, q) > EPS;
    };
    bool flag = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        if (on_segment(p[i], p[(i + 1) % n], a)) return !strict;
        flag ^= ray_cut(a, p[i], p[(i + 1) % n]);
    }
    return flag;
}
// compute the intersection point between segment p-q and line A-B
pt line_intersect_seg(pt p, pt q, pt A, pt B) {
    double a = B.imag() - A.imag(), b = A.real() - B.real();
    double c = B.real() * A.imag() - A.real() * B.imag();
    double u = fabs(a * p.real() + b * p.imag() + c);
    double v = fabs(a * q.real() + b * q.imag() + c);
    if (fabs(u + v) < EPS) return p; // fallback if degenerate
    return pt((p.real() * v + q.real() * u) / (u + v), (p.imag() * v + q.imag() * u) / (u + v));
}
// part of polygon Q on the left of line A->B (order matters); Q and the result repeat the
// first point at the end
vector<pt> cut_polygon(pt A, pt B, const vector<pt> &Q) {
    vector<pt> p;
    for (int i = 0; i < (int)Q.size(); ++i) {
        double left1 = cross(B - A, Q[i] - A), left2 = 0;
        if (i != (int)Q.size() - 1) left2 = cross(B - A, Q[i + 1] - A);
        if (left1 > -EPS) p.push_back(Q[i]);
        if (left1 * left2 < -EPS) p.push_back(line_intersect_seg(Q[i], Q[i + 1], A, B));
    }
    if (!p.empty() && abs(p.back() - p.front()) > EPS) p.push_back(p.front());
    return p;
}
} // namespace geo_lib