#include <bits/stdc++.h>
using namespace std;

namespace geo_lib {
// ! beware that angles are in rad by default
const double EPS = 1E-9, PI = acos(-1);
using T = double; //  for some functions , double is a must
typedef complex<T> pt;
#define X real()
#define Y imag()

int dblcmp(T a, T b) { return fabs(a - b) < EPS ? 0 : a < b ? -1 : 1; }
int sign(T val) { return (val > EPS ? +1 : val < -EPS ? -1 : 0); }
bool in_range(T l, T r, T v) { return (dblcmp(l, v) != 1 && dblcmp(v, r) != 1); }

// * ------------------------- Points & Vectors -------------------------

T dot(pt a, pt b) { return a.X * b.X + a.Y * b.Y; }
T cross(pt a, pt b) { return a.X * b.Y - a.Y * b.X; }
T sq(pt a) { return dot(a, a); }
double dist(pt a, pt b) { return abs(a - b); }
double length(pt a) { return abs(a); }

// counter clockwise test (ccw) peace sign
// + if c is on the left of BA , - for the right , 0 if collinear
T ccw(pt a, pt b, pt c) { return cross(b - a, c - a); }

pt rotate_by(pt a, double theta) { return a * polar(1.0, theta); }
pt rotate_around(pt a, pt b, double theta) { return (a - b) * polar(1.0, theta) + b; }
pt perp(pt p) { return pt(-p.Y, p.X); }
pt translate(pt a, pt b) { return a + b; }
// scale vector p , with factor around center
pt scale_around(pt center, double factor, pt p) { return center + (p - center) * factor; }

bool collinear(pt a, pt b, pt c) { return fabs(ccw(a, b, c)) < EPS; }

bool on_ray(pt a, pt b, pt p) {
    if (!collinear(a, b, p)) return false;
    return dblcmp(dot(b - a, p - a), 0) >= 0;
}
// check if point p is inside the angle BAC (peace sign)
// check if point p lies in the angle between ba and ac (bac counter-clockwise)
int in_angle(pt a, pt b, pt c, pt p) {
    if (fabs(ccw(a, b, c)) < EPS) return -1;
    T abp = ccw(a, b, p), acp = ccw(a, c, p), abc = ccw(a, b, c);
    if (abc < 0) swap(abp, acp);
    return (abp >= -EPS && acp <= EPS) ^ (abc < 0);
}

bool polar_comp(const pt &a, const pt &b) {
    int ha = a.Y > EPS || (fabs(a.Y) < EPS && a.X >= -EPS);
    int hb = b.Y > EPS || (fabs(b.Y) < EPS && b.X >= -EPS);
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
    sort(ret.begin(), ret.end(), polar_comp); // or any other comparator
    ret.erase(unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

double segment_point_dist(pt a, pt b, pt p) {
    if (a != b) {
        line l(a, b);
        if (l.cmp_proj(a, p) && l.cmp_proj(p, b)) // if closest to projection
            return l.dist(p);                     // output distance to	line
    }
    return min(abs(p - a), abs(p - b)); // otherwise distance to A or B
}

double seg_seg_dist(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (proper_intersect(a, b, c, d, dummy)) return 0;
    return min({segment_point_dist(a, b, c), segment_point_dist(a, b, d), segment_point_dist(c, d, a),
                segment_point_dist(c, d, b)});
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
    return atan2(a.Y, a.X);
}
double angle(pt a, pt b) { return normalize_angle(angle(b) - angle(a)); }
// angle between three vectors BAC (counter-clockwise)
double oriented_angle(pt a, pt b, pt c) {
    T ccw_val = ccw(a, b, c);
    if (ccw_val >= -EPS) return angle(b - a, c - a);
    return 2 * PI - angle(b - a, c - a);
}
// * ------------------------- Circles -------------------------

// inside : 1 , outside -1 , boundary 0
int point_circle_location(pt center, double radius, pt p) { return dblcmp(radius, abs(p - center)); }

int intersect_circle_line(pt o, double r, line l, pair<pt, pt> &out) {
    double h2 = r * r - l.sq_dist(o);
    if (h2 < -EPS) return 0; // line not touching
    if (fabs(h2) < EPS) h2 = 0;
    pt p = l.proj(o);
    pt h = l.v * sqrt(h2) / abs(l.v);
    out = {p - h, p + h};
    return 1 + (h2 > EPS);
}

int intersect_circles(pt o1, double r1, pt o2, double r2, pair<pt, pt> &out) {
    pt d = o2 - o1;
    double d2 = sq(d);
    if (d2 < EPS) return fabs(r1 - r2) <= EPS ? INT_MAX : -1; // same ? inf : else none
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

double radius_circumcircle(pt a, pt b, pt c) { return radius_circumcircle(dist(a, b), dist(b, c), dist(c, a)); }
// given 2 points on the circle (p1 and p2) and radius r of some intersecting circle
// determine the location of the centers(c1 and c2)
bool circle_points_radius_center(pt p1, pt p2, double r, pt &ret) {
    double d2 = sq(p1 - p2);
    double det = r * r / d2 - 0.25;
    if (det < -EPS) return false;
    if (fabs(det) < EPS) det = 0;
    double h = sqrt(det);
    // computes c1 , to get the other center reverse p1 and p2
    ret = pt((p1.X + p2.X) * 0.5 + (p1.Y - p2.Y) * h, (p1.Y + p2.Y) * 0.5 + (p2.X - p1.X) * h);
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

// check if point a is (strictly) inside the polygon P ,
bool in_polygon(const vector<pt> &p, pt a, bool strict = 1) {
    auto above = [](pt p, pt q) { return p.Y >= q.Y - EPS; };
    // check if [PQ] crosses ray from A
    auto ray_cut = [above](pt a, pt p, pt q) { return (above(a, q) - above(a, p)) * ccw(a, p, q) > EPS; };
    bool flag = 0;
    for (int i = 0, n = p.size(); i < n; ++i) {
        if (on_segment(p[i], p[(i + 1) % n], a)) return !strict;
        flag ^= ray_cut(a, p[i], p[(i + 1) % n]);
    }
    return flag;
}
// compute the intersection point between segment p-q and line A-B
pt line_intersect_seg(pt p, pt q, pt A, pt B) {
    double a = B.Y - A.Y, b = A.X - B.X, c = B.X * A.Y - A.X * B.Y;
    double u = fabs(a * p.X + b * p.Y + c);
    double v = fabs(a * q.X + b * q.Y + c);
    if (fabs(u + v) < EPS) return p; // fallback if degenerate
    return pt((p.X * v + q.X * u) / (u + v), (p.Y * v + q.Y * u) / (u + v));
}
// cuts polygon Q along the line formed by point A->point B (order matters)
// (note: the last point must be the same as the first point)
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