// Test for 05_DP/Weighted_Job_Scheduling.cpp against trying every subset of jobs (closed intervals:
// two jobs clash when max(start) <= min(end)).
// Checks: max_profit on random jobs with ties, touching ends, duplicates, negative times and
// profits, n = 0, n = 1, and nested / identical intervals.
#include "../../05_DP/Weighted_Job_Scheduling.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(414);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll brute(const vector<array<ll, 3>> &a) {
    int n = a.size();
    ll best = 0;
    for (int m = 0; m < (1 << n); m++) {
        bool okm = true;
        ll s = 0;
        for (int i = 0; i < n && okm; i++) {
            if (!(m >> i & 1)) continue;
            s += a[i][2];
            for (int j = i + 1; j < n; j++)
                if ((m >> j & 1) && max(a[i][0], a[j][0]) <= min(a[i][1], a[j][1])) okm = false;
        }
        if (okm) best = max(best, s);
    }
    return best;
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 12;
        ll T = vector<ll>{3, 6, 20, 1000000000}[rng() % 4];
        bool neg = rng() % 3 == 0, negp = rng() % 4 == 0;
        vector<array<ll, 3>> a(n);
        for (auto &j : a) {
            ll s = rng() % T - (neg ? T / 2 : 0), e = s + rng() % (T / 2 + 1);
            ll p = rng() % 100 + 1 - (negp ? 50 : 0);
            j = {s, e, p};
        }
        if (n >= 2 && rng() % 4 == 0) a[1] = a[0]; // identical jobs
        if (n >= 2 && rng() % 4 == 0) // touching ends
            a[n - 1][0] = a[0][1], a[n - 1][1] = max(a[n - 1][1], a[0][1]);
        chk(negp ? "max_profit(negative profits)" : neg ? "max_profit(negative times)"
                                                        : "max_profit(random)",
            max_profit(a) == brute(a));
    }
    chk("max_profit(n=0)", max_profit({}) == 0);
    chk("max_profit(n=1)", max_profit({{{5, 9, 7}}}) == 7);
    chk("max_profit(n=1 negative)", max_profit({{{5, 9, -7}}}) == 0);
    chk("max_profit(touching closed)", max_profit({{{1, 3, 10}, {3, 5, 10}}}) == 10);
    chk("max_profit(touching negative start)", max_profit({{{-1, 3, 10}, {3, 5, 10}}}) == 10);
    chk("max_profit(gap of 1)", max_profit({{{1, 3, 10}, {4, 5, 10}}}) == 20);
    chk("max_profit(nested)", max_profit({{{1, 10, 15}, {2, 3, 10}, {5, 6, 10}}}) == 20);
    {
        int n = 200000;
        vector<array<ll, 3>> a(n);
        for (auto &j : a) {
            ll s = rng() % 1000000000;
            j = {s, s + (ll)(rng() % 1000000), (ll)(rng() % 1000000000)};
        }
        auto t0 = clock();
        ll r = max_profit(a);
        printf("n=2e5: %.3fs (profit=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, r);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
