// Test for 05_DP/DnC_Optimization.cpp against the naive O(k n^2) partition DP.
// Checks: dnc_dp with (sum of group)^2, the CF 321E pair cost (2D prefix sums), a max objective via
// negation (concave cost), k = 1, k = n, k > n (INF), k = 0, n = 1, and that a cost breaking the
// monotone split point really gives wrong answers (so the stated restriction is real).
#include "../../05_DP/DnC_Optimization.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(321);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
const ll BIG = LLONG_MAX / 4;

template <class F> ll brute(int n, int k, F cost) {
    vector<vector<ll>> dp(k + 1, vector<ll>(n + 1, BIG));
    dp[0][0] = 0;
    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= n; j++)
            for (int t = 0; t < j; t++)
                if (dp[i - 1][t] < BIG) dp[i][j] = min(dp[i][j], dp[i - 1][t] + cost(t + 1, j));
    return k >= 1 && k <= n ? dp[k][n] : BIG;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 25 + 1, k = rng() % (n + 2);
        ll hi = vector<ll>{1, 5, 1000, 1000000}[rng() % 4];
        vector<ll> p(n + 1, 0);
        for (int i = 1; i <= n; i++) p[i] = p[i - 1] + (ll)(rng() % (hi + 1));
        auto sq = [&](int l, int r) -> ll { return (p[r] - p[l - 1]) * (p[r] - p[l - 1]); };
        ll got = dnc_dp(n, k, sq), want = brute(n, k, sq);
        bool ok = (want >= BIG) ? got >= BIG : got == want;
        chk("dnc_dp(sum^2)", ok);
        if (k > n || k == 0) chk("dnc_dp(k > n or k = 0 -> INF)", got >= BIG);
        if (k == n) chk("dnc_dp(k = n)", ok);
        if (k == 1) chk("dnc_dp(k = 1)", got == sq(1, n));
        // max objective: sum of S * (C - S), concave in S, solved as -dnc_dp(-cost)
        if (k >= 1 && k <= n) {
            ll C = p[n] + 7;
            auto g = [&](int l, int r) -> ll { ll s = p[r] - p[l - 1]; return s * (C - s); };
            auto ng = [&](int l, int r) -> ll { return -g(l, r); };
            // brute max: negate brute min of -g
            chk("dnc_dp(max via negation)", -dnc_dp(n, k, ng) == -brute(n, k, ng));
        }
    }
    // CF 321E: cost(l, r) = sum of u[i][j] over l <= i < j <= r, u symmetric, non-negative
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 20 + 1, k = rng() % n + 1;
        vector<vector<ll>> u(n + 1, vector<ll>(n + 1, 0)), c(n + 1, vector<ll>(n + 1, 0));
        for (int i = 1; i <= n; i++)
            for (int j = i + 1; j <= n; j++) u[i][j] = u[j][i] = rng() % 10;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                c[i][j] = u[i][j] + c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
        auto cost = [&](int l, int r) -> ll {
            return (c[r][r] - c[l - 1][r] - c[r][l - 1] + c[l - 1][l - 1]) / 2;
        };
        chk("dnc_dp(CF 321E pair cost)", dnc_dp(n, k, cost) == brute(n, k, cost));
    }
    chk("dnc_dp(n = 1)", dnc_dp(1, 1, [](int, int) -> ll { return 42; }) == 42);
    // a random cost table has no monotone split point: DnC must disagree with brute somewhere
    {
        int bad = 0;
        for (int it = 0; it < 2000; it++) {
            int n = rng() % 10 + 3, k = rng() % (n - 1) + 2;
            vector<vector<ll>> t(n + 2, vector<ll>(n + 2));
            for (auto &r : t)
                for (auto &v : r) v = rng() % 100;
            auto cost = [&](int l, int r) -> ll { return t[l][r]; };
            bad += dnc_dp(n, k, cost) != brute(n, k, cost);
        }
        printf("random (non-monotone) cost: %d / 2000 wrong, as expected\n", bad);
        chk("restriction is real (random cost fails)", bad > 0);
    }
    {
        int n = 4000, k = 800; // CF 321E limits
        vector<vector<int>> c(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; i++)
            for (int j = i + 1; j <= n; j++) c[i][j] = c[j][i] = rng() % 10;
        long long calls = 0;
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++) c[i][j] += c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
        auto cost = [&](int l, int r) -> ll {
            calls++;
            return (c[r][r] - c[l - 1][r] - c[r][l - 1] + c[l - 1][l - 1]) / 2;
        };
        auto t0 = clock();
        ll r = dnc_dp(n, k, cost);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=4000 k=800 (CF 321E): %.3fs (ans=%lld, cost calls=%lld)\n", t, r, calls);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
