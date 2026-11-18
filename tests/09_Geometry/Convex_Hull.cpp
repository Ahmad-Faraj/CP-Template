// Test for 09_Geometry/Convex_Hull.cpp against an O(n^3) edge-by-edge brute force hull.
// Points are multiples of 0.5 (exact in double, |coord| <= 1e4); the oracle doubles them to
// integers and works in __int128, so hull vertices are compared exactly (no tolerance).
// Every public method is checked: CH::hull_points (result, and its in-place sort + dedup of pts).
#include "../../09_Geometry/Convex_Hull.cpp"
using ll = long long;

map<string, pair<long long, long long>> cov;
mt19937_64 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

using IP = pair<ll, ll>; // doubled coordinates
// directed hull edges u -> v: every other point strictly left of u->v or strictly inside [u, v]
set<pair<IP, IP>> brute_edges(const vector<IP> &p) {
    set<pair<IP, IP>> e;
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
                    __int128 d = ux * wx + uy * wy;
                    if (d <= 0 || d >= ux * ux + uy * uy) ok = false;
                }
            }
            if (ok) e.insert({p[i], p[j]});
        }
    return e;
}
IP toI(pt z) { return {llround(z.real() * 2), llround(z.imag() * 2)}; }

void check(vector<pt> pts, const string &tag) {
    vector<IP> d;
    for (auto &z : pts) d.push_back(toI(z));
    sort(d.begin(), d.end());
    d.erase(unique(d.begin(), d.end()), d.end());
    CH ch;
    auto h = ch.hull_points(pts);
    // pts is now sorted by (x, y) with duplicates removed
    vector<IP> after;
    for (auto &z : pts) after.push_back(toI(z));
    chk("hull_points(sorts pts)", after == d);
    bool ok;
    if (d.size() <= 1) ok = h.size() == d.size() && (d.empty() || toI(h[0]) == d[0]);
    else {
        set<pair<IP, IP>> got;
        int m = h.size();
        for (int i = 0; i < m; i++) got.insert({toI(h[i]), toI(h[(i + 1) % m])});
        ok = (int)got.size() == m && got == brute_edges(d) && toI(h[0]) == d[0]; // starts at min (x, y)
    }
    chk("hull_points" + tag, ok);
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rnd(0, 12);
        ll C = vector<ll>{1, 3, 10, 20000}[rng() % 4];
        vector<pt> pts;
        for (int i = 0; i < n; i++) {
            if (!pts.empty() && rng() % 4 == 0) pts.push_back(pts[rng() % pts.size()]);
            else pts.push_back(pt(rnd(-C, C) / 2.0, rnd(-C, C) / 2.0));
        }
        check(pts, C == 20000 ? "(1e4)" : "(small)");
    }
    for (int it = 0; it < 5000; it++) { // all collinear, with duplicates
        ll dx = rnd(-3, 3), dy = rnd(-3, 3), x0 = rnd(-5, 5), y0 = rnd(-5, 5);
        vector<pt> pts;
        for (int i = 0, n = rnd(1, 8); i < n; i++) {
            ll t = rnd(-4, 4);
            pts.push_back(pt((x0 + t * dx) / 2.0, (y0 + t * dy) / 2.0));
        }
        check(pts, "(collinear)");
    }
    check({}, "(edge)");
    check({pt(5, 5)}, "(edge)");
    check({pt(1, 1), pt(1, 1), pt(1, 1)}, "(edge)");
    check({pt(0, 0), pt(1e4, 0), pt(1e4, 1e4), pt(0, 1e4), pt(5e3, 5e3), pt(1e4, 5e3)}, "(edge)");
    {
        int n = 200000;
        vector<pt> pts(n), circ;
        for (auto &p : pts) p = pt(rnd(-1e9, 1e9) / 1e5, rnd(-1e9, 1e9) / 1e5);
        for (int i = 0; i < n; i++) circ.push_back(polar(1e4, 2 * M_PI * i / n));
        auto t0 = clock();
        CH ch;
        int a = ch.hull_points(pts).size(), b = ch.hull_points(circ).size();
        printf("hull_points n=2e5 random + n=2e5 on a circle: %.3fs (sizes %d, %d)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, a, b);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
