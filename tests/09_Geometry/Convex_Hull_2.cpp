// Test for 09_Geometry/Convex_Hull_2.cpp against an O(n^3) edge-by-edge brute force hull.
// Every public function is checked: Point (<, ==, -), cross, ccw, dist, dist2, convexHull.
#include "../../09_Geometry/Convex_Hull_2.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

// directed hull edges u -> v: every other point is strictly left of u->v, or lies strictly
// between u and v on the segment. Distinct points only. Uses __int128, not the template's cross.
set<pair<pair<ll, ll>, pair<ll, ll>>> brute_edges(const vector<pair<ll, ll>> &p) {
    set<pair<pair<ll, ll>, pair<ll, ll>>> e;
    int n = p.size();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            bool ok = true;
            for (int k = 0; k < n && ok; k++) {
                if (k == i || k == j) continue;
                __int128 ux = p[j].first - p[i].first, uy = p[j].second - p[i].second;
                __int128 wx = p[k].first - p[i].first, wy = p[k].second - p[i].second;
                __int128 c = ux * wy - uy * wx;
                if (c < 0) ok = false;
                else if (c == 0) {
                    __int128 d = ux * wx + uy * wy; // must be strictly inside the segment
                    if (d <= 0 || d >= ux * ux + uy * uy) ok = false;
                }
            }
            if (ok) e.insert({p[i], p[j]});
        }
    return e;
}

void check_hull(const vector<Point> &pts, const string &tag) {
    vector<pair<ll, ll>> d;
    for (auto &q : pts) d.push_back({q.x, q.y});
    sort(d.begin(), d.end());
    d.erase(unique(d.begin(), d.end()), d.end());
    auto h = convexHull(pts);
    bool ok;
    if (d.size() <= 1) ok = h.size() == d.size() && (d.empty() || (h[0].x == d[0].first && h[0].y == d[0].second));
    else {
        set<pair<pair<ll, ll>, pair<ll, ll>>> got;
        int m = h.size();
        for (int i = 0; i < m; i++) got.insert({{h[i].x, h[i].y}, {h[(i + 1) % m].x, h[(i + 1) % m].y}});
        auto want = brute_edges(d);
        ok = (int)got.size() == m && got == want; // also rules out repeated vertices
        // starts at the lowest, then leftmost point
        auto lo = *min_element(d.begin(), d.end(), [](auto &a, auto &b) {
            return make_pair(a.second, a.first) < make_pair(b.second, b.first);
        });
        ok &= h[0].x == lo.first && h[0].y == lo.second;
    }
    chk("convexHull" + tag, ok);
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rnd(0, 12);
        ll C = vector<ll>{1, 3, 10, 1000000000}[rng() % 4];
        vector<Point> pts;
        for (int i = 0; i < n; i++) {
            if (!pts.empty() && rng() % 4 == 0) pts.push_back(pts[rng() % pts.size()]); // duplicate
            else pts.push_back(Point(rnd(-C, C), rnd(-C, C)));
        }
        check_hull(pts, C == 1000000000 ? "(1e9)" : "(small)");
    }
    for (int it = 0; it < 5000; it++) { // all collinear on a random line, with duplicates
        ll dx = rnd(-3, 3), dy = rnd(-3, 3), x0 = rnd(-5, 5), y0 = rnd(-5, 5);
        vector<Point> pts;
        int n = rnd(1, 8);
        for (int i = 0; i < n; i++) {
            ll t = rnd(-4, 4);
            pts.push_back(Point(x0 + t * dx, y0 + t * dy));
        }
        check_hull(pts, "(collinear)");
    }
    {
        check_hull({}, "(edge)");
        check_hull({Point(5, 5)}, "(edge)");
        check_hull({Point(1, 1), Point(1, 1), Point(1, 1)}, "(edge)");
        check_hull({Point(2, 3), Point(2, 3)}, "(edge)");
        ll M = 1000000000;
        check_hull({Point(-M, -M), Point(M, -M), Point(M, M), Point(-M, M), Point(0, 0), Point(M, 0)},
                   "(edge)");
        chk("convexHull(identical)", convexHull({Point(1, 1), Point(1, 1), Point(1, 1)}).size() == 1);
    }
    for (int it = 0; it < 20000; it++) {
        ll C = rng() % 2 ? 5 : 1000000000;
        Point a(rnd(-C, C), rnd(-C, C)), b(rnd(-C, C), rnd(-C, C)), c(rnd(-C, C), rnd(-C, C));
        __int128 cr = (__int128)(b.x - a.x) * (c.y - a.y) - (__int128)(b.y - a.y) * (c.x - a.x);
        chk("cross", (__int128)cross(a, b, c) == cr);
        chk("ccw", ccw(a, b, c) == (cr > 0));
        __int128 d2 = (__int128)(a.x - b.x) * (a.x - b.x) + (__int128)(a.y - b.y) * (a.y - b.y);
        chk("dist2", (__int128)dist2(a, b) == d2);
        long double d = sqrtl((long double)d2);
        chk("dist", fabsl(dist(a, b) - d) <= 1e-12L * max((long double)1, d));
        Point s = a - b;
        chk("operator-", s.x == a.x - b.x && s.y == a.y - b.y);
        chk("operator==", (a == b) == (a.x == b.x && a.y == b.y) && a == a);
        chk("operator<", (a < b) == (make_pair(a.x, a.y) < make_pair(b.x, b.y)));
    }
    {
        int n = 200000;
        vector<Point> pts(n);
        for (auto &p : pts) p = Point(rnd(-1000000000, 1000000000), rnd(-1000000000, 1000000000));
        auto t0 = clock();
        auto h = convexHull(pts);
        vector<Point> circ;
        for (int i = 0; i < n; i++) {
            double a = 2 * M_PI * i / n;
            circ.push_back(Point(llround(1e9 * cos(a)), llround(1e9 * sin(a))));
        }
        auto h2 = convexHull(circ);
        printf("convexHull n=2e5 random + n=2e5 on a circle: %.3fs (sizes %d, %d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, (int)h.size(), (int)h2.size());
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
