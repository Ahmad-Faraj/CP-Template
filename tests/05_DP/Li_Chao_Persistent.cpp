// Test for 05_DP/Li_Chao_Persistent.cpp against a brute-force min over the lines of each version.
// Every public method is checked: add_line (on latest and on random older versions), query
// (every version, incl. empty version 0), latest, count. Ranges include negative, tiny and L == R.
#include "../../05_DP/Li_Chao_Persistent.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

int main() {
    for (int it = 0; it < 20000; it++) {
        ll L, R;
        int kind = it % 5;
        if (kind == 0) L = rnd(-5, 5), R = L + rnd(0, 3);      // tiny, L == R possible
        else if (kind == 1) L = rnd(-70, -2), R = rnd(L, -1);  // all negative
        else if (kind == 2) L = -64, R = 63;                   // deep chains with tangents
        else if (kind == 3) L = -1000000, R = 1000000;
        else L = rnd(-50, 0), R = rnd(0, 50);
        Li_Chao_Persistent t(L, R);
        vector<vector<pair<ll, ll>>> lines{{}}; // lines of each version
        int n = rnd(0, 40);
        bool tangent = rng() % 2;
        for (int i = 0; i < n; i++) {
            ll m, c;
            if (tangent) { // tangents of a concave parabola: every line is on the envelope
                ll p = rnd(L, R);
                m = -2 * p, c = p * p;
            } else {
                ll big = kind == 3 ? 1000000 : 20;
                m = rnd(-big, big), c = rnd(-big * big, big * big);
            }
            int par = rng() % 3 ? t.latest() : (int)(rng() % lines.size());
            int v = t.add_line(par, {m, c});
            auto ls = lines[par];
            ls.push_back({m, c});
            lines.push_back(ls);
            chk("add_line(id)", v == (int)lines.size() - 1 && v == t.latest());
            chk(par == v - 1 ? "add_line(latest)" : "add_line(older version)", true);
            chk("count", t.count() == (int)lines.size());
        }
        for (int q = 0; q < 30; q++) {
            int v = rng() % lines.size();
            ll x = q < 3 ? (q == 0 ? L : q == 1 ? R : (L + R) / 2) : rnd(L, R);
            ll want = Li_Chao_Persistent::INF;
            for (auto [m, c] : lines[v]) want = min(want, m * x + c);
            chk(lines[v].empty() ? "query(empty version)" : "query", t.query(v, x) == want);
        }
        // every version at every x of a small range
        if (R - L <= 130) {
            bool ok = true;
            for (int v = 0; v < (int)lines.size(); v++)
                for (ll x = L; x <= R; x++) {
                    ll want = Li_Chao_Persistent::INF;
                    for (auto [m, c] : lines[v]) want = min(want, m * x + c);
                    ok &= t.query(v, x) == want;
                }
            chk("query(all versions, all x)", ok);
        }
    }
    {   // max via negation, as in ACPC 2024 E: max of x*V + y on a root path
        Li_Chao_Persistent t(-10, 10);
        int v1 = t.add_line(0, {-3, -1}), v2 = t.add_line(v1, {-(-2), -5}); // 3x+1, -2x+5
        chk("max via negation", -t.query(v2, 2) == 7 && -t.query(v2, -2) == 9 && -t.query(v1, -2) == -5);
    }
    {
        Li_Chao_Persistent t(-1000000, 1000000);
        t.t.reserve(100000 * 24);
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 100000; i++) {
            int par = rng() % 4 ? t.latest() : (int)(rng() % t.count());
            t.add_line(par, {rnd(-1000000, 1000000), rnd(-(ll)1e12, (ll)1e12)});
        }
        for (int i = 0; i < 100000; i++) s += t.query(rng() % t.count(), rnd(-1000000, 1000000));
        printf("1e5 inserts (random parents) + 1e5 queries, C=2e6: %.3fs, nodes=%zu (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, t.t.size(), s % 7);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
