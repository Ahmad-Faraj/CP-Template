// Test for 09_Geometry/Points_And_Polygons.cpp. Point<long long> results are compared exactly
// against __int128 arithmetic (coordinates up to 1e9); pointInPolygon against an exact boundary
// test plus a winding-number oracle; lineIntersection (Point<long double>) against an exact
// rational intersection with a 1e-9 relative tolerance.
// Every public function and operator is checked: +=, -=, *=, unary -, +, -, * (both sides), ==,
// >>, <<, conversion, dot, cross, square, length (template and long double), Line, rotate, sgn,
// pointOnLineLeft, lineIntersection, pointOnSegment, pointInPolygon.
#include "../../09_Geometry/Points_And_Polygons.cpp"
using ll = long long;
using i128 = __int128;
using PL = Point<ll>;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
PL rp(ll C) { return PL(rnd(-C, C), rnd(-C, C)); }

i128 o_cross(PL a, PL b) { return (i128)a.x * b.y - (i128)a.y * b.x; }
i128 o_orient(PL a, PL b, PL c) { return o_cross(PL(b.x - a.x, b.y - a.y), PL(c.x - a.x, c.y - a.y)); }
bool o_onseg(PL a, PL b, PL p) {
    return o_orient(a, b, p) == 0 && min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x) &&
           min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}
int o_half(PL v) { return (v.y < 0 || (v.y == 0 && v.x < 0)) ? 1 : 0; }
bool o_ang_less(PL a, PL b) {
    if (o_half(a) != o_half(b)) return o_half(a) < o_half(b);
    return o_cross(a, b) > 0;
}
int winding(const vector<PL> &poly, PL q) {
    long double s = 0;
    for (int i = 0, n = poly.size(); i < n; i++) {
        long double ax = poly[i].x - q.x, ay = poly[i].y - q.y;
        long double bx = poly[(i + 1) % n].x - q.x, by = poly[(i + 1) % n].y - q.y;
        s += atan2l(ax * by - ay * bx, ax * bx + ay * by);
    }
    return (int)llroundl(s / (2 * acosl(-1)));
}
vector<PL> star_poly(int n, ll C) { // simple polygon, any orientation by caller
    while (true) {
        vector<PL> v;
        for (int i = 0; i < n; i++) {
            PL p = rp(C);
            if (p.x || p.y) v.push_back(p);
        }
        sort(v.begin(), v.end(), o_ang_less);
        bool ok = v.size() >= 3;
        for (int i = 0; ok && i < (int)v.size(); i++)
            if (o_cross(v[i], v[(i + 1) % v.size()]) <= 0) ok = false;
        if (ok) return v;
    }
}

int main() {
    for (int it = 0; it < 100000; it++) {
        ll C = rng() % 2 ? 5 : 1000000000;
        PL a = rp(C), b = rp(C), c = rp(C);
        ll k = rnd(-3, 3);
        PL s = a;
        s += b;
        chk("operator+=", s.x == a.x + b.x && s.y == a.y + b.y);
        s = a;
        s -= b;
        chk("operator-=", s.x == a.x - b.x && s.y == a.y - b.y);
        s = a;
        s *= k;
        chk("operator*=", s.x == a.x * k && s.y == a.y * k);
        chk("operator-(unary)", (-a).x == -a.x && (-a).y == -a.y);
        chk("operator+", (a + b).x == a.x + b.x && (a + b).y == a.y + b.y);
        chk("operator-", (a - b).x == a.x - b.x && (a - b).y == a.y - b.y);
        chk("operator*(P,T)", (a * k).x == a.x * k && (a * k).y == a.y * k);
        chk("operator*(T,P)", (k * a).x == a.x * k && (k * a).y == a.y * k);
        chk("operator==", (a == b) == (a.x == b.x && a.y == b.y) && a == a);
        auto ad = (Point<long double>)a;
        chk("operator Point<U>", ad.x == (long double)a.x && ad.y == (long double)a.y);
        if (C == 5 || rng() % 2) { // keep products < 2^63 (|coords| <= 1e9 -> |dot| <= 2e18)
            chk("dot", (i128)dot(a, b) == (i128)a.x * b.x + (i128)a.y * b.y);
            chk("cross", (i128)cross(a, b) == o_cross(a, b));
            chk("square", (i128)square(a) == (i128)a.x * a.x + (i128)a.y * a.y);
            long double L = sqrtl((long double)a.x * a.x + (long double)a.y * a.y);
            chk("length(T)", fabsl(length(a) - L) <= 1e-12L * max((long double)1, L));
            chk("length(long double)", fabsl(length(ad) - L) <= 1e-15L * max((long double)1, L));
        }
        PL r = rotate(a);
        chk("rotate", r.x == -a.y && r.y == a.x);
        chk("sgn", sgn(a) == (o_half(a) == 0 && (a.x || a.y) ? 1 : -1)); // upper half incl +x axis
        Line<ll> l(a, b);
        chk("Line", l.a == a && l.b == b);
        chk("pointOnLineLeft", pointOnLineLeft(c, l) == (o_orient(a, b, c) > 0));
        chk("pointOnSegment", pointOnSegment(c, l) == o_onseg(a, b, c));
        if (rng() % 3 == 0) { // c exactly on the segment
            ll t = rnd(0, 4);
            PL m(a.x + (b.x - a.x) / 4 * t, a.y + (b.y - a.y) / 4 * t);
            chk("pointOnSegment", pointOnSegment(m, l) == o_onseg(a, b, m));
        }
        // lineIntersection in long double
        PL d = rp(C);
        i128 den = o_cross(PL(b.x - a.x, b.y - a.y), PL(d.x - c.x, d.y - c.y));
        if (den != 0 && C == 5) {
            i128 num = o_cross(PL(c.x - a.x, c.y - a.y), PL(d.x - c.x, d.y - c.y));
            long double t = (long double)num / (long double)den;
            long double wx = a.x + (b.x - a.x) * t, wy = a.y + (b.y - a.y) * t;
            auto got = lineIntersection(Line<long double>((Point<long double>)a, (Point<long double>)b),
                                        Line<long double>((Point<long double>)c, (Point<long double>)d));
            chk("lineIntersection", fabsl(got.x - wx) <= 1e-9L * max((long double)1, fabsl(wx)) &&
                                        fabsl(got.y - wy) <= 1e-9L * max((long double)1, fabsl(wy)));
        }
    }
    { // stream operators
        stringstream ss("3 -7");
        PL p;
        ss >> p;
        stringstream os;
        os << p;
        chk("operator>>", p.x == 3 && p.y == -7);
        chk("operator<<", os.str() == "(3, -7)");
    }
    for (int it = 0; it < 30000; it++) {
        ll C = vector<ll>{3, 6, 1000, 1000000000}[rng() % 4];
        vector<PL> poly = star_poly(rnd(3, 9), C);
        if (rng() % 2) reverse(poly.begin(), poly.end());
        for (int k = 0; k < 6; k++) {
            PL q = rp(C);
            if (k == 0) q = poly[rng() % poly.size()];
            if (k == 1) { // midpoint of an edge when exact
                int i = rng() % poly.size();
                PL u = poly[i], v = poly[(i + 1) % poly.size()];
                if ((u.x + v.x) % 2 == 0 && (u.y + v.y) % 2 == 0) q = PL((u.x + v.x) / 2, (u.y + v.y) / 2);
            }
            bool onb = false;
            for (int i = 0, n = poly.size(); i < n; i++) onb |= o_onseg(poly[i], poly[(i + 1) % n], q);
            bool want = onb || winding(poly, q) != 0;
            chk(onb ? "pointInPolygon(boundary)" : "pointInPolygon", pointInPolygon(q, poly) == want);
        }
    }
    { // edge cases
        chk("pointInPolygon(edge)", pointInPolygon(PL(0, 0), {PL(0, 0), PL(2, 0), PL(0, 2)}) &&
                                        !pointInPolygon(PL(2, 2), {PL(0, 0), PL(2, 0), PL(0, 2)}) &&
                                        pointInPolygon(PL(1, 1), {PL(0, 0), PL(2, 0), PL(0, 2)}));
        chk("pointOnSegment(edge)", pointOnSegment(PL(1, 1), Line(PL(1, 1), PL(1, 1))) &&
                                        !pointOnSegment(PL(1, 2), Line(PL(1, 1), PL(1, 1))));
        chk("sgn(edge)", sgn(PL(1, 0)) == 1 && sgn(PL(-1, 0)) == -1 && sgn(PL(0, 0)) == -1);
    }
    { // timing: point in polygon, n = 2e5 vertices, 1e3 queries (2e8 edge checks)
        int n = 200000;
        vector<PL> big;
        for (int i = 0; i < n; i++) {
            long double a = 2 * acosl(-1) * i / n;
            big.push_back(PL(llroundl(1e9L * cosl(a)), llroundl(1e9L * sinl(a))));
        }
        auto t0 = clock();
        int cnt = 0;
        for (int i = 0; i < 1000; i++) cnt += pointInPolygon(rp(1000000000), big);
        printf("pointInPolygon n=2e5, 1e3 queries: %.3fs (%d inside)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               cnt);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
