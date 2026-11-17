// Test for 05_DP/CHT.cpp against the minimum over every added line, computed by brute force.
// Checks: add + query (non-decreasing x, interleaved with adds), add + query_bs / bs (any x),
// equal slopes, duplicate lines, negative values, max mode via add(-m, -b), large values where a
// double hull test breaks, the CF 319C DP shape vs O(n^2), and that both stated order rules
// (slopes non-increasing, query x non-decreasing) are real.
#include "../../05_DP/CHT.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(319);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }

ll brute(const vector<pair<ll, ll>> &ls, ll x, bool mx) {
    ll best = mx ? LLONG_MIN : LLONG_MAX;
    for (auto [m, b] : ls) best = mx ? max(best, m * x + b) : min(best, m * x + b);
    return best;
}

// one random scenario; returns false on any mismatch. mx = max mode via negation.
bool scenario(ll M, ll Bv, ll X, bool mx, const string &tag, bool ptr_queries) {
    int n = rng() % 30 + 1;
    vector<ll> ms(n);
    for (auto &m : ms) m = rnd(-M, M);
    if (rng() % 3 == 0)
        for (auto &m : ms) m = rnd(-2, 2); // many equal slopes
    sort(ms.begin(), ms.end());
    if (!mx) reverse(ms.begin(), ms.end()); // min: non-increasing; max: non-decreasing
    CHT c;
    vector<pair<ll, ll>> ls;
    vector<ll> xs;
    for (int q = 0; q < 2 * n; q++) xs.push_back(rnd(-X, X));
    sort(xs.begin(), xs.end());
    int xi = 0;
    bool ok = true;
    for (int i = 0; i < n; i++) {
        ll b = rnd(-Bv, Bv);
        if (i && rng() % 5 == 0) b = ls.back().second, ms[i] = ls.back().first; // duplicate line
        ls.push_back({ms[i], b});
        mx ? c.add(-ms[i], -b) : c.add(ms[i], b);
        int nq = rng() % 3;
        for (int q = 0; q < nq; q++) {
            ll x = ptr_queries ? xs[min(xi++, 2 * n - 1)] : rnd(-X, X);
            ll want = brute(ls, x, mx);
            if (ptr_queries) {
                ll got = mx ? -c.query(x) : c.query(x);
                chk(tag + " query", got == want), ok &= got == want;
            }
            ll got = mx ? -c.query_bs(x) : c.query_bs(x);
            chk(tag + " query_bs", got == want), ok &= got == want;
            ll got2 = mx ? -c.bs(0, c.m.size() - 1, x) : c.bs(0, c.m.size() - 1, x);
            chk(tag + " bs", got2 == want);
        }
    }
    return ok;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        bool ptr = it % 2;
        scenario(5, 10, 10, false, "min small", ptr);
        scenario(5, 10, 10, true, "max small", ptr);
        scenario(1000000, 1000000000, 1000000, false, "min 1e6/1e9", ptr);
        scenario(1000000, 1000000000, 1000000, true, "max 1e6/1e9", ptr);
        // hull products near 1e24: a double test misjudges near-ties
        scenario(1000000000, 1000000000000000LL, 1000, false, "min large (int128 hull)", ptr);
    }
    // CF 319C shape: dp[i] = min over j < i of dp[j] + b[j] * a[i], a increasing, b decreasing
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 40 + 1;
        vector<ll> a(n), b(n);
        for (auto &v : a) v = rnd(1, 1000);
        for (auto &v : b) v = rnd(0, 1000);
        sort(a.begin(), a.end());
        sort(b.rbegin(), b.rend());
        vector<ll> dp(n, LLONG_MAX), d2(n);
        dp[0] = d2[0] = 0;
        for (int i = 1; i < n; i++)
            for (int j = 0; j < i; j++) dp[i] = min(dp[i], dp[j] + b[j] * a[i]);
        CHT c;
        c.add(b[0], 0);
        for (int i = 1; i < n; i++) d2[i] = c.query(a[i]), c.add(b[i], d2[i]);
        chk("CF 319C dp (query)", dp == d2);
    }
    // the stated order rules are real: break them and the answers go wrong
    {
        int bad_x = 0, bad_slope = 0;
        for (int it = 0; it < 3000; it++) {
            int n = rng() % 10 + 3;
            vector<pair<ll, ll>> ls;
            CHT c, d;
            vector<ll> ms(n);
            for (auto &m : ms) m = rnd(-20, 20);
            sort(ms.rbegin(), ms.rend());
            for (int i = 0; i < n; i++)
                ls.push_back({ms[i], rnd(-50, 50)}), c.add(ls[i].first, ls[i].second);
            for (int q = 0; q < 6; q++) { // x going down
                ll x = 20 - 7 * q;
                bad_x += c.query(x) != brute(ls, x, false);
            }
            shuffle(ls.begin(), ls.end(), rng); // slopes out of order
            for (auto [m, b] : ls) d.add(m, b);
            for (ll x = -20; x <= 20; x += 5) bad_slope += d.query_bs(x) != brute(ls, x, false);
        }
        printf("rule check: decreasing x wrong %d times, unsorted slopes wrong %d times\n", bad_x,
               bad_slope);
        chk("rule is real: query x must not decrease", bad_x > 0);
        chk("rule is real: slopes must not increase", bad_slope > 0);
    }
    {
        int n = 1000000;
        vector<ll> ms(n);
        for (auto &m : ms) m = rnd(-1000000, 1000000);
        sort(ms.rbegin(), ms.rend());
        CHT c;
        ll s = 0;
        auto t0 = clock();
        for (int i = 0; i < n; i++) c.add(ms[i], rnd(-1000000000000LL, 1000000000000LL));
        for (int i = 0; i < n; i++) s += c.query(-1000000 + 2 * i);
        for (int i = 0; i < n; i++) s += c.query_bs(rnd(-1000000, 1000000));
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("1e6 adds + 1e6 query + 1e6 query_bs: %.3fs (checksum %lld)\n", t, s);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
