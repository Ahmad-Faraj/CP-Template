// Test for 09_Geometry/Geometry_Lib.cpp. Inputs are small integer points, so every predicate is
// checked against exact long long / __int128 arithmetic. Floating results (intersection points,
// distances, angles, rotations, areas) are checked against independent closed forms with an
// absolute/relative tolerance of 1e-6 (coordinates stay within 1e3).
// Every public function, overload and line method is checked.
#include "../../09_Geometry/Geometry_Lib.cpp"
using namespace geo_lib;
using ll = long long;
using i128 = __int128;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
double rndd(double lo, double hi) { return lo + (hi - lo) * (rng() % 1000001) / 1e6; }

struct P { ll x, y; };
P rp(ll C) { return {rnd(-C, C), rnd(-C, C)}; }
pt D(P a) { return pt((double)a.x, (double)a.y); }
P sub(P a, P b) { return {a.x - b.x, a.y - b.y}; }
i128 cr(P a, P b) { return (i128)a.x * b.y - (i128)a.y * b.x; }
i128 dt(P a, P b) { return (i128)a.x * b.x + (i128)a.y * b.y; }
i128 orient(P a, P b, P c) { return cr(sub(b, a), sub(c, a)); }
int sg(i128 v) { return v > 0 ? 1 : v < 0 ? -1 : 0; }
bool same(P a, P b) { return a.x == b.x && a.y == b.y; }
bool near(double a, double b, double tol = 1e-6) { return fabs(a - b) <= tol * max(1.0, fabs(b)); }
bool nearp(pt a, pt b, double tol = 1e-6) {
    return near(a.real(), b.real(), tol) && near(a.imag(), b.imag(), tol);
}
// exact: p on closed segment [a, b]
bool o_onseg(P a, P b, P p) {
    if (orient(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
// exact angle class of v in [0, 2pi): (half, then cross).
int half(P v) { return (v.y < 0 || (v.y == 0 && v.x < 0)) ? 1 : 0; }
bool ang_less(P a, P b) { // angle(a) < angle(b), both in [0, 2pi); the origin sorts first
    bool za = a.x == 0 && a.y == 0, zb = b.x == 0 && b.y == 0;
    if (za || zb) return za && !zb;
    if (half(a) != half(b)) return half(a) < half(b);
    return cr(a, b) > 0;
}
// vector w expressed in the frame where d points along +x (exact, scaled by |d|)
P frame(P d, P w) { return {(ll)dt(d, w), (ll)cr(d, w)}; }
// line through integer points p, q intersect line through r, s: exact rational -> double
bool o_lineint(P p, P q, P r, P s, pt &out) {
    i128 den = cr(sub(q, p), sub(s, r));
    if (den == 0) return false;
    i128 t = cr(sub(r, p), sub(s, r)); // p + (q-p) * t/den
    long double tt = (long double)t / (long double)den;
    out = pt((double)(p.x + (q.x - p.x) * tt), (double)(p.y + (q.y - p.y) * tt));
    return true;
}
double o_psd(P a, P b, P p) { // point-segment distance, clamp formula
    long double dx = b.x - a.x, dy = b.y - a.y, L = dx * dx + dy * dy, t = 0;
    if (L > 0) t = max((long double)0, min((long double)1, ((p.x - a.x) * dx + (p.y - a.y) * dy) / L));
    long double fx = a.x + t * dx - p.x, fy = a.y + t * dy - p.y;
    return (double)sqrtl(fx * fx + fy * fy);
}
bool o_seg_touch(P a, P b, P c, P d) {
    int o1 = sg(orient(a, b, c)), o2 = sg(orient(a, b, d)), o3 = sg(orient(c, d, a)),
        o4 = sg(orient(c, d, b));
    if (o1 * o2 < 0 && o3 * o4 < 0) return true;
    return o_onseg(a, b, c) || o_onseg(a, b, d) || o_onseg(c, d, a) || o_onseg(c, d, b);
}
// winding number by summing turning angles (point not on boundary)
int winding(const vector<P> &poly, P q) {
    long double s = 0;
    int n = poly.size();
    for (int i = 0; i < n; i++) {
        P a = sub(poly[i], q), b = sub(poly[(i + 1) % n], q);
        s += atan2l((long double)cr(a, b), (long double)dt(a, b));
    }
    return (int)llroundl(s / (2 * acosl(-1)));
}
// simple polygon: random points sorted by angle around the origin, every gap < pi
vector<P> star_poly(int n, ll C) {
    while (true) {
        vector<P> v;
        for (int i = 0; i < n; i++) {
            P p = rp(C);
            if (p.x == 0 && p.y == 0) continue;
            v.push_back(p);
        }
        sort(v.begin(), v.end(), ang_less);
        bool ok = v.size() >= 3;
        for (int i = 0; ok && i < (int)v.size(); i++)
            if (cr(v[i], v[(i + 1) % v.size()]) <= 0) ok = false; // equal angles or gap >= pi
        if (ok) return v;
    }
}
// brute convex hull (gift wrap over all ordered pairs), ccw, strict vertices only
vector<P> o_hull(vector<P> p) {
    sort(p.begin(), p.end(), [](P a, P b) { return make_pair(a.x, a.y) < make_pair(b.x, b.y); });
    p.erase(unique(p.begin(), p.end(), same), p.end());
    int n = p.size();
    if (n < 3) return p;
    map<pair<ll, ll>, P> nxt;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            bool ok = true;
            for (int k = 0; k < n && ok; k++) {
                if (k == i || k == j) continue;
                i128 c = orient(p[i], p[j], p[k]);
                if (c < 0) ok = false;
                else if (c == 0) {
                    i128 d = dt(sub(p[j], p[i]), sub(p[k], p[i]));
                    if (d <= 0 || d >= dt(sub(p[j], p[i]), sub(p[j], p[i]))) ok = false;
                }
            }
            if (ok) nxt[{p[i].x, p[i].y}] = p[j];
        }
    if (nxt.empty()) return {p.front(), p.back()}; // collinear
    vector<P> h = {p[0]};
    while (true) {
        P q = nxt[{h.back().x, h.back().y}];
        if (same(q, h[0])) break;
        h.push_back(q);
    }
    return h;
}
double area_d(const vector<pt> &v) { // shoelace, signed
    double s = 0;
    for (int i = 0, n = v.size(); i < n; i++) s += cross(v[i], v[(i + 1) % n]);
    return s / 2;
}
// exact signed half-plane side of a point against directed line A->B, in double
double side_d(pt A, pt B, pt q) {
    return (B.real() - A.real()) * (q.imag() - A.imag()) - (B.imag() - A.imag()) * (q.real() - A.real());
}

int main() {
    const double PI_ = acosl(-1);
    // ---- scalar helpers
    for (int it = 0; it < 20000; it++) {
        double a = rndd(-5, 5), b = a + vector<double>{0, 1e-12, -1e-12, 1e-3, -1e-3, 2}[rng() % 6];
        int want = fabs(a - b) < 1e-10 ? 0 : (a < b ? -1 : 1);
        chk("dblcmp", dblcmp(a, b) == want);
        double v = b - a;
        chk("sign", sign(v) == want * -1);
        double l = rndd(-3, 3), r = l + rndd(0, 3);
        double x = vector<double>{l, r, l - 1e-12, r + 1e-12, l - 1e-3, r + 1e-3, (l + r) / 2}[rng() % 7];
        bool in = x >= l - 1e-10 && x <= r + 1e-10;
        chk("in_range", in_range(l, r, x) == in);
    }
    // ---- vectors, exact
    for (int it = 0; it < 50000; it++) {
        ll C = rng() % 2 ? 3 : 1000;
        P a = rp(C), b = rp(C), c = rp(C);
        chk("dot", dot(D(a), D(b)) == (double)dt(a, b));
        chk("cross", cross(D(a), D(b)) == (double)cr(a, b));
        chk("sq", sq(D(a)) == (double)dt(a, a));
        chk("dist", near(geo_lib::dist(D(a), D(b)), sqrt((double)dt(sub(a, b), sub(a, b))), 1e-12));
        chk("length", near(length(D(a)), sqrt((double)dt(a, a)), 1e-12));
        chk("ccw", ccw(D(a), D(b), D(c)) == (double)orient(a, b, c));
        chk("collinear", collinear(D(a), D(b), D(c)) == (orient(a, b, c) == 0));
        chk("perp", perp(D(a)) == pt(-a.y, a.x));
        chk("translate", translate(D(a), D(b)) == pt(a.x + b.x, a.y + b.y));
        double th = rndd(-10, 10), f = rndd(-3, 3);
        pt r1 = rotate_by(D(a), th);
        chk("rotate_by", nearp(r1, pt(a.x * cos(th) - a.y * sin(th), a.x * sin(th) + a.y * cos(th))));
        pt r2 = rotate_around(D(a), D(b), th);
        double ux = a.x - b.x, uy = a.y - b.y;
        chk("rotate_around",
            nearp(r2, pt(b.x + ux * cos(th) - uy * sin(th), b.y + ux * sin(th) + uy * cos(th))));
        pt s = scale_around(D(c), f, D(a));
        chk("scale_around", nearp(s, pt(c.x + (a.x - c.x) * f, c.y + (a.y - c.y) * f)));
        if (!same(a, b)) {
            bool want = orient(a, b, c) == 0 && dt(sub(b, a), sub(c, a)) >= 0;
            chk("on_ray", on_ray(D(a), D(b), D(c)) == want);
        }
        chk("polar_comp", polar_comp(D(a), D(b)) == ang_less(a, b));
    }
    // ---- in_angle: inside the ccw sweep from ray ab to ray ac
    for (int it = 0; it < 50000; it++) {
        ll C = rng() % 2 ? 3 : 50;
        P a = rp(C), b = rp(C), c = rp(C), p = rp(C);
        int got = in_angle(D(a), D(b), D(c), D(p));
        if (orient(a, b, c) == 0) {
            chk("in_angle(degenerate=-1)", got == -1);
            continue;
        }
        if (same(p, a)) continue;
        P d = sub(b, a), fc = frame(d, sub(c, a)), fp = frame(d, sub(p, a));
        bool on_b = fp.y == 0 && fp.x > 0, on_c = cr(fc, fp) == 0 && dt(fc, fp) > 0;
        if (on_b || on_c) {
            chk("in_angle(boundary)", got == (orient(a, b, c) > 0)); // see Note in the template
            continue;
        }
        chk("in_angle", got == (int)ang_less(fp, fc));
    }
    // ---- lines
    for (int it = 0; it < 50000; it++) {
        ll C = rng() % 2 ? 4 : 1000;
        P p = rp(C), q = rp(C), r = rp(C), s = rp(C), t = rp(C);
        if (same(p, q)) continue;
        P v = sub(q, p);
        line l(D(p), D(q));
        i128 sd = orient(p, q, r);
        chk("line(p,q).side", l.side(D(r)) == (double)sd);
        double L = sqrt((double)dt(v, v));
        chk("line.dist", near(l.dist(D(r)), fabs((double)sd) / L));
        chk("line.sq_dist", near(l.sq_dist(D(r)), (double)sd * (double)sd / (double)dt(v, v)));
        ll A = rnd(-C, C), B = rnd(-C, C), Cc = rnd(-C, C);
        if (A || B) {
            line l2(A, B, Cc); // A x + B y = Cc
            chk("line(a,b,c).side", sign(l2.side(D(r))) == sg((i128)A * r.x + (i128)B * r.y - Cc));
        }
        line l3(D(v), (double)Cc); // cross(v, x) = Cc
        chk("line(v,c).side", l3.side(D(r)) == (double)(cr(v, r) - Cc));
        line pt_ = l.perp_through(D(r));
        chk("line.perp_through", fabs(pt_.side(D(r))) < 1e-6 && dot(pt_.v, l.v) == 0);
        chk("line.cmp_proj", l.cmp_proj(D(r), D(s)) == (dt(v, r) < dt(v, s)));
        line lt = l.line_translate(D(t));
        chk("line.line_translate", lt.side(D({r.x + t.x, r.y + t.y})) == (double)sd);
        double dd = rndd(-5, 5);
        line ls = l;
        ls.shift_left(dd);
        pt moved = D(p) + pt(-v.y, v.x) / L * dd; // p moved dd to the left of p->q
        chk("line.shift_left", fabs(ls.side(moved)) / L < 1e-6);
        long double tt = (long double)dt(sub(r, p), v) / (long double)dt(v, v);
        pt foot((double)(p.x + tt * v.x), (double)(p.y + tt * v.y));
        chk("line.proj", nearp(l.proj(D(r)), foot));
        if (same(r, s)) continue;
        line m(D(r), D(s));
        pt got, want;
        bool okw = o_lineint(p, q, r, s, want), okg = line_intersect(l, m, got);
        chk("line_intersect", okw == okg && (!okw || nearp(got, want)));
        chk("line_parallel", line_parallel(l, m) == (cr(v, sub(s, r)) == 0));
        chk("line_perp", line_perp(l, m) == (dt(v, sub(s, r)) == 0));
    }
    // ---- segments
    for (int it = 0; it < 100000; it++) {
        ll C = rng() % 3 == 0 ? 1000 : 3;
        P a = rp(C), b = rp(C), c = rp(C), d = rp(C);
        if (rng() % 4 == 0) d = c; // point-segment
        if (rng() % 6 == 0) b = a;
        chk("on_segment", on_segment(D(a), D(b), D(c)) == o_onseg(a, b, c));
        int o1 = sg(orient(a, b, c)), o2 = sg(orient(a, b, d)), o3 = sg(orient(c, d, a)),
            o4 = sg(orient(c, d, b));
        bool proper = o1 * o2 < 0 && o3 * o4 < 0;
        pt out, want;
        bool got = proper_intersect(D(a), D(b), D(c), D(d), out);
        chk("proper_intersect", got == proper && (!proper || (o_lineint(a, b, c, d, want) && nearp(out, want))));
        // seg_inters: the endpoints of the intersection (one point if proper)
        vector<pt> si = seg_inters(D(a), D(b), D(c), D(d));
        bool ok;
        if (proper) ok = si.size() == 1 && nearp(si[0], want);
        else {
            set<pair<ll, ll>> w;
            for (P e : {a, b}) if (o_onseg(c, d, e)) w.insert({e.x, e.y});
            for (P e : {c, d}) if (o_onseg(a, b, e)) w.insert({e.x, e.y});
            set<pair<ll, ll>> g;
            for (auto z : si) g.insert({llround(z.real()), llround(z.imag())});
            ok = g == w && si.size() == w.size();
        }
        chk("seg_inters", ok);
        P p = rp(C);
        chk("segment_point_dist", near(segment_point_dist(D(a), D(b), D(p)), o_psd(a, b, p)));
        double wd = o_seg_touch(a, b, c, d)
                        ? 0
                        : min({o_psd(a, b, c), o_psd(a, b, d), o_psd(c, d, a), o_psd(c, d, b)});
        chk("seg_seg_dist", near(seg_seg_dist(D(a), D(b), D(c), D(d)), wd));
    }
    // ---- angles
    for (int it = 0; it < 50000; it++) {
        double th = rndd(-1000, 1000);
        chk("deg_to_rad", near(deg_to_rad(th), th / 180 * PI_, 1e-12));
        chk("rad_to_deg", near(rad_to_deg(th), th * 180 / PI_, 1e-12));
        double nt = normalize_angle(th);
        double k = (th - nt) / (2 * PI_);
        chk("normalize_angle", nt >= 0 && nt < 2 * PI_ && fabs(k - llround(k)) < 1e-9);
        ll C = rng() % 2 ? 3 : 1000;
        P a = rp(C), b = rp(C), c = rp(C);
        double an = angle(D(a)), la = sqrt((double)dt(a, a));
        if (same(a, {0, 0})) chk("angle(a)", an == 0);
        else chk("angle(a)", an > -PI_ - 1e-12 && an <= PI_ + 1e-12 && near(cos(an) * la, a.x) &&
                              near(sin(an) * la, a.y));
        if (same(a, {0, 0}) || same(b, {0, 0})) continue;
        // ccw angle from a to b in [0, 2pi)
        double ab = angle(D(a), D(b)), lb = sqrt((double)dt(b, b));
        chk("angle(a,b)", ab >= 0 && ab < 2 * PI_ + 1e-12 && near(cos(ab), (double)dt(a, b) / la / lb) &&
                              near(sin(ab), (double)cr(a, b) / la / lb));
        // oriented_angle(o, b, c): ccw angle from ray o->b to ray o->c in [0, 2pi)
        P o = c;
        P u = sub(a, o), w = sub(b, o);
        if (same(u, {0, 0}) || same(w, {0, 0})) continue;
        double oa = oriented_angle(D(o), D(a), D(b));
        double lu = sqrt((double)dt(u, u)), lw = sqrt((double)dt(w, w));
        chk("oriented_angle", oa >= 0 && oa < 2 * PI_ + 1e-12 &&
                                  near(cos(oa), (double)dt(u, w) / lu / lw) &&
                                  near(sin(oa), (double)cr(u, w) / lu / lw));
    }
    // ---- circles (integer centre and radius, exact counts)
    for (int it = 0; it < 50000; it++) {
        ll C = rng() % 2 ? 5 : 300;
        P o = rp(C), p = rp(C), q = rp(C);
        ll r = rnd(0, C);
        i128 d2 = dt(sub(p, o), sub(p, o));
        chk("point_circle_location", point_circle_location(D(o), r, D(p)) == sg((i128)r * r - d2));
        if (!same(p, q)) { // circle-line
            line l(D(p), D(q));
            P v = sub(q, p);
            i128 cs = orient(p, q, o), lhs = (i128)r * r * dt(v, v), rhs = cs * cs;
            int want = lhs < rhs ? 0 : lhs == rhs ? 1 : 2;
            pair<pt, pt> out;
            int got = intersect_circle_line(D(o), r, l, out);
            bool ok = got == want;
            if (ok && want)
                for (pt z : {out.first, out.second})
                    ok &= near(abs(z - D(o)), r) && fabs(l.side(z)) / abs(l.v) < 1e-6;
            if (ok && want == 2) ok &= abs(out.first - out.second) > 1e-9;
            chk("intersect_circle_line", ok);
        }
        { // circle-circle
            ll r2 = rnd(0, C);
            P o2 = rng() % 5 ? q : o;
            i128 dd = dt(sub(o2, o), sub(o2, o)), sp = (i128)(r + r2) * (r + r2),
                 df = (i128)(r - r2) * (r - r2);
            int want;
            if (dd == 0) want = r == r2 ? INT_MAX : -1;
            else if (dd > sp || dd < df) want = 0;
            else if (dd == sp || dd == df) want = 1;
            else want = 2;
            pair<pt, pt> out;
            int got = intersect_circles(D(o), r, D(o2), r2, out);
            bool ok = got == want;
            if (ok && (want == 1 || want == 2))
                for (pt z : {out.first, out.second})
                    ok &= near(abs(z - D(o)), r, 1e-5) && near(abs(z - D(o2)), r2, 1e-5);
            chk("intersect_circles", ok);
        }
        { // circumradius via exact circumcentre
            P a = p, b = q, c = rp(C);
            i128 den = 2 * orient(a, b, c);
            double got1 = radius_circumcircle(D(a), D(b), D(c));
            double got2 = radius_circumcircle(abs(D(a) - D(b)), abs(D(b) - D(c)), abs(D(c) - D(a)));
            if (den == 0) {
                chk("radius_circumcircle(pts)", isinf(got1));
                chk("radius_circumcircle(sides)", isinf(got2));
            } else {
                // centre relative to a: solve |x-(b-a)| = |x| = |x-(c-a)|
                P B = sub(b, a), Cv = sub(c, a);
                i128 bb = dt(B, B), cc = dt(Cv, Cv);
                long double ux = (long double)(Cv.y * bb - B.y * cc) / (long double)den;
                long double uy = (long double)(B.x * cc - Cv.x * bb) / (long double)den;
                double R = (double)sqrtl(ux * ux + uy * uy);
                // Heron loses precision on needle triangles: compare relative 1e-6 only when the
                // area is not tiny compared to the sides
                double area = fabs((double)den) / 4, mx = sqrt((double)max({bb, cc, dt(sub(c, b), sub(c, b))}));
                if (area > 1e-3 * mx * mx) {
                    chk("radius_circumcircle(pts)", near(got1, R, 1e-6));
                    chk("radius_circumcircle(sides)", near(got2, R, 1e-6));
                }
            }
        }
        if (!same(p, q)) { // circle through p and q with radius r: centre on the left of p->q
            pt cen;
            i128 dd = dt(sub(q, p), sub(q, p));
            bool want = (i128)4 * r * r >= dd;
            bool got = circle_points_radius_center(D(p), D(q), r, cen);
            bool ok = got == want;
            if (ok && got)
                ok &= near(abs(cen - D(p)), r, 1e-5) && near(abs(cen - D(q)), r, 1e-5) &&
                      side_d(D(p), D(q), cen) >= -1e-6;
            chk("circle_points_radius_center", ok);
        }
    }
    // ---- polygons
    for (int it = 0; it < 20000; it++) {
        ll C = rng() % 2 ? 4 : 1000;
        int n = rnd(3, 9);
        vector<P> poly;
        bool convex_kind = rng() % 2;
        if (convex_kind) {
            vector<P> pts;
            for (int i = 0; i < n + 2; i++) pts.push_back(rp(C));
            poly = o_hull(pts);
            if (poly.size() < 3) continue;
            // optionally insert a collinear midpoint on an edge (only when exact)
            if (rng() % 3 == 0) {
                int i = rng() % poly.size();
                P a = poly[i], b = poly[(i + 1) % poly.size()];
                if ((a.x + b.x) % 2 == 0 && (a.y + b.y) % 2 == 0)
                    poly.insert(poly.begin() + i + 1, P{(a.x + b.x) / 2, (a.y + b.y) / 2});
            }
        } else poly = star_poly(n, C);
        if (rng() % 2) reverse(poly.begin(), poly.end());
        rotate(poly.begin(), poly.begin() + rng() % poly.size(), poly.end());
        vector<pt> pd;
        for (P z : poly) pd.push_back(D(z));
        // is_convex on a simple polygon: no two nonzero turns of opposite sign
        int pos = 0, neg = 0;
        for (int i = 0, m = poly.size(); i < m; i++) {
            int o = sg(orient(poly[i], poly[(i + 1) % m], poly[(i + 2) % m]));
            pos += o > 0, neg += o < 0;
        }
        chk("is_convex", is_convex(pd) == !(pos && neg));
        // in_polygon: winding number for interior points, exact for boundary
        for (int k = 0; k < 6; k++) {
            P q = rp(C + 1);
            if (k == 0) q = poly[rng() % poly.size()];
            bool onb = false;
            for (int i = 0, m = poly.size(); i < m; i++) onb |= o_onseg(poly[i], poly[(i + 1) % m], q);
            bool want_in = onb ? false : winding(poly, q) != 0;
            chk("in_polygon(strict)", in_polygon(pd, D(q), true) == want_in);
            chk("in_polygon(non-strict)", in_polygon(pd, D(q), false) == (onb || want_in));
        }
        // cut_polygon by directed line A->B: keeps the left side
        P A = rp(C), B = rp(C);
        if (same(A, B)) continue;
        vector<pt> closed = pd;
        closed.push_back(pd[0]);
        vector<pt> L = cut_polygon(D(A), D(B), closed), R = cut_polygon(D(B), D(A), closed);
        auto open = [](vector<pt> v) {
            if (!v.empty()) v.pop_back();
            return v;
        };
        bool ok = (L.empty() || nearp(L.front(), L.back())) && (R.empty() || nearp(R.front(), R.back()));
        for (pt z : L) ok &= side_d(D(A), D(B), z) >= -1e-6;
        for (pt z : R) ok &= side_d(D(A), D(B), z) <= 1e-6;
        double tot = fabs(area_d(pd));
        ok &= near(fabs(area_d(open(L))) + fabs(area_d(open(R))), tot, 1e-6);
        if (convex_kind) { // independent area: hull of (kept vertices + edge crossings)
            vector<pt> keep;
            for (int i = 0, m = poly.size(); i < m; i++) {
                P a = poly[i], b = poly[(i + 1) % m];
                i128 sa = orient(A, B, a), sb = orient(A, B, b);
                if (sa >= 0) keep.push_back(D(a));
                pt x;
                if ((sa > 0 && sb < 0) || (sa < 0 && sb > 0)) {
                    o_lineint(a, b, A, B, x);
                    keep.push_back(x);
                }
            }
            double want = 0;
            if (keep.size() >= 3) {
                pt cen = 0;
                for (pt z : keep) cen += z;
                cen /= (double)keep.size();
                sort(keep.begin(), keep.end(), [&](pt a, pt b) {
                    return atan2(a.imag() - cen.imag(), a.real() - cen.real()) <
                           atan2(b.imag() - cen.imag(), b.real() - cen.real());
                });
                want = fabs(area_d(keep));
            }
            ok &= near(fabs(area_d(open(L))), want, 1e-6);
        }
        chk("cut_polygon", ok);
    }
    // ---- line_intersect_seg: segment p-q crosses line A-B
    for (int it = 0; it < 50000; it++) {
        ll C = rng() % 2 ? 5 : 1000;
        P p = rp(C), q = rp(C), A = rp(C), B = rp(C);
        if (same(A, B)) continue;
        i128 sp = orient(A, B, p), sq_ = orient(A, B, q);
        if (!((sp > 0 && sq_ < 0) || (sp < 0 && sq_ > 0))) continue;
        pt want;
        o_lineint(p, q, A, B, want);
        chk("line_intersect_seg", nearp(line_intersect_seg(D(p), D(q), D(A), D(B)), want));
    }
    // ---- named edge cases
    {
        // overlapping collinear segments that share an endpoint: (1,1)-(3,3) and (2,2)-(1,1)
        auto v = seg_inters(pt(1, 1), pt(3, 3), pt(2, 2), pt(1, 1));
        chk("seg_inters(edge)", v.size() == 2);
        // ccw angle from +x to -y is 3pi/2
        chk("oriented_angle(edge)", near(oriented_angle(pt(0, 0), pt(1, 0), pt(0, -1)), 1.5 * PI_));
        chk("is_convex(edge)", !is_convex({pt(0, 0), pt(1, 1)}));
        pair<pt, pt> o;
        chk("intersect_circles(edge)", intersect_circles(pt(1, 1), 2, pt(1, 1), 2, o) == INT_MAX &&
                                           intersect_circles(pt(1, 1), 2, pt(1, 1), 3, o) == -1);
        chk("cut_polygon(edge)", cut_polygon(pt(0, 0), pt(1, 0), {}).empty());
    }
    // ---- timing: 1e6 segment intersections + point-in-polygon on a 1e5-gon (1e3 queries)
    {
        auto t0 = clock();
        double s = 0;
        for (int i = 0; i < 1000000; i++) {
            pt a(rndd(-1e3, 1e3), rndd(-1e3, 1e3)), b(rndd(-1e3, 1e3), rndd(-1e3, 1e3));
            pt c(rndd(-1e3, 1e3), rndd(-1e3, 1e3)), d(rndd(-1e3, 1e3), rndd(-1e3, 1e3));
            s += seg_inters(a, b, c, d).size() + seg_seg_dist(a, b, c, d);
        }
        vector<pt> big;
        int n = 100000;
        for (int i = 0; i < n; i++) big.push_back(polar(1e3, 2 * PI_ * i / n));
        int cnt = 0;
        for (int i = 0; i < 1000; i++) cnt += in_polygon(big, pt(rndd(-1e3, 1e3), rndd(-1e3, 1e3)));
        vector<pt> cl = big;
        cl.push_back(big[0]);
        cnt += cut_polygon(pt(0, 0), pt(1, 1), cl).size();
        printf("1e6 seg_inters+seg_seg_dist, 1e3 in_polygon on 1e5-gon, 1 cut: %.3fs (%d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, cnt + (int)(s > 0));
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
