// Test for 05_DP/Li_Chao_Tree.cpp against the minimum over every inserted line / segment that
// covers x, computed by brute force.
// Checks: add_line, add_segment (inside, clipped to [L, R], empty l > r, single point), query
// (INF when nothing covers x), the default constructor, ranges that are tiny / negative / a single
// point / +-1e9, slopes and queries in random order, equal lines, max mode via {-m, -c}.
#include "../../05_DP/Li_Chao_Tree.cpp"

// nodes are never freed (by design), so LeakSanitizer is switched off for this test
extern "C" const char *__asan_default_options() { return "detect_leaks=0"; }

map<string, pair<long long, long long>> cov;
mt19937_64 rng(4231);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
const ll INF = LiChaoTree::INF;

struct Seg { ll m, c, l, r; };
ll brute(const vector<Seg> &s, ll x, bool mx) {
    ll best = INF;
    for (auto &g : s)
        if (g.l <= x && x <= g.r) best = min(best, mx ? -(g.m * x + g.c) : g.m * x + g.c);
    return best;
}

void scenario(ll L, ll R, ll M, ll C, bool mx, const string &tag) {
    LiChaoTree t(L, R);
    vector<Seg> segs;
    int ops = rng() % 40 + 1;
    for (int o = 0; o < ops; o++) {
        int ty = rng() % 3;
        ll m = rnd(-M, M), c = rnd(-C, C);
        if (segs.size() && rng() % 6 == 0) m = segs.back().m, c = segs.back().c; // equal line
        ll sm = mx ? -m : m, sc = mx ? -c : c;
        if (ty == 0) {
            t.add_line({sm, sc});
            segs.push_back({m, c, L, R});
            chk(tag + " add_line", true);
        } else if (ty == 1) {
            ll span = R - L;
            ll l = rnd(L - 2, R + 2), r = rnd(L - 2, R + 2); // may stick out of [L, R] or be empty
            if (rng() % 4 == 0 && span > 4) l = rnd(L, R), r = l; // single point
            t.add_segment({sm, sc}, l, r);
            segs.push_back({m, c, max(l, L), min(r, R)});
            chk(tag + " add_segment", true);
        } else {
            for (int q = 0; q < 3; q++) {
                ll x = rng() % 5 == 0 ? (rng() % 2 ? L : R) : rnd(L, R);
                ll want = brute(segs, x, mx), got = t.query(x); // max: both hold -line
                chk(tag + " query", got == want);
                if (want == INF) chk(tag + " query (no cover -> INF)", got == INF);
            }
        }
    }
    // final sweep over the whole range when it is small
    if (R - L <= 40)
        for (ll x = L; x <= R; x++) chk(tag + " query (sweep)", t.query(x) == brute(segs, x, mx));
}

int main() {
    for (int it = 0; it < 20000; it++) {
        bool mx = it % 2;
        string mode = mx ? "max" : "min";
        scenario(-8, 8, 5, 20, mx, mode + " [-8,8]");
        scenario(0, 0, 5, 20, mx, mode + " [0,0]");
        scenario(-30, -3, 100, 1000, mx, mode + " [-30,-3]");
        // |m x + c| <= 1e9 * 1e9 + 9e17 stays below INF = 2e18
        scenario(-1000000000, 1000000000, 1000000000, 900000000000000000LL, mx, mode + " +-1e9");
    }
    {
        LiChaoTree t; // default range
        vector<Seg> segs;
        for (int i = 0; i < 300; i++) {
            ll m = rnd(-1e6, 1e6), c = rnd(-1e12, 1e12), l = rnd(-1e9, 1e9), r = rnd(l, 1e9);
            t.add_segment({m, c}, l, r), segs.push_back({m, c, l, r});
            ll x = rnd(-1e9, 1e9);
            chk("default ctor query", t.query(x) == brute(segs, x, false));
        }
    }
    {
        LiChaoTree t(-1000000000, 1000000000);
        int n = 200000;
        unsigned long long s = 0;
        auto t0 = clock();
        for (int i = 0; i < n; i++) {
            ll l = rnd(-1e9, 1e9), r = rnd(l, 1e9);
            t.add_segment({rnd(-1e9, 1e9), rnd(-9e17, 9e17)}, l, r);
        }
        auto t1 = clock();
        for (int i = 0; i < n; i++) t.add_line({rnd(-1e9, 1e9), rnd(-9e17, 9e17)});
        for (int i = 0; i < n; i++) s += t.query(rnd(-1e9, 1e9));
        auto t2 = clock();
        printf("+-1e9: 2e5 add_segment %.3fs | 2e5 add_line + 2e5 query %.3fs (checksum %llu)\n",
               double(t1 - t0) / CLOCKS_PER_SEC, double(t2 - t1) / CLOCKS_PER_SEC, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
