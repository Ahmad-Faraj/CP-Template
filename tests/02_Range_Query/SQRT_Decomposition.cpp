// Test for 02_Range_Query/SQRT_Decomposition.cpp against a plain array with naive sums.
// Every public method is checked: constructor (n = 0 .. 300, around block boundaries),
// update (point assign, negatives, overwrite) and query (every [l, r] pair, plus empty l > r).
#include "../../02_Range_Query/SQRT_Decomposition.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
void chk_n(const string &m, long long checks, long long bad) {
    cov[m].first += checks, cov[m].second += bad;
}
ll rnd(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); }

int main() {
    for (int it = 0; it < 4000; it++) {
        int n = it < 300 ? it : (int)rnd(0, 120);
        ll V = it % 2 ? 10 : (ll)1e12;
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-V, V);
        SqrtDecomp sd(a);
        auto all_ranges = [&](const string &m) {
            long long checks = 0, bad = 0;
            for (int l = 0; l < n; l++) {
                ll s = 0;
                for (int r = l; r < n; r++) s += a[r], checks++, bad += sd.query(l, r) != s;
                if (l) checks++, bad += sd.query(l, l - 1) != 0; // empty range
            }
            chk_n(m, checks, bad);
        };
        all_ranges("query (after build)");
        if (n == 0) continue;
        for (int s = 0; s < 40; s++) {
            int i = (int)rnd(0, n - 1);
            ll v = rnd(-V, V);
            sd.update(i, v), a[i] = v;
            int l = (int)rnd(0, n - 1), r = (int)rnd(l, n - 1);
            ll want = 0;
            for (int k = l; k <= r; k++) want += a[k];
            chk("update + query", sd.query(l, r) == want && sd.query(i, i) == v);
        }
        if (n <= 150) all_ranges("query (after updates)");
    }
    {
        int n = 200000, q = 200000;
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-1e9, 1e9);
        auto t0 = clock();
        SqrtDecomp sd(a);
        ll sink = 0;
        for (int i = 0; i < q; i++) {
            sd.update((int)rnd(0, n - 1), rnd(-1e9, 1e9));
            int l = (int)rnd(0, n - 1), r = (int)rnd(l, n - 1);
            sink += sd.query(l, r);
        }
        printf("timing n=2e5: 2e5 updates + 2e5 queries: %.3fs (sink %lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sink & 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first,
               c.second);
        ok &= c.first > 0 && c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
