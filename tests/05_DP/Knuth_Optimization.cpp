// Test for 05_DP/Knuth_Optimization.cpp against the plain O(n^3) interval DP
// dp[l][r] = min over l < m < r of dp[l][m] + dp[m][r] + cost(l, r), dp[l][l+1] = 0.
// Checks: knuth with stick-cutting cost x[r] - x[l] (duplicate and zero gaps), (x[r] - x[l])^2,
// merging piles (prefix sums), n = 0, n = 1, n = 2, and an n = 2000 timing run.
#include "../../05_DP/Knuth_Optimization.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(2860);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

template <class F> ll brute(int n, F cost) {
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
    for (int s = 2; s <= n; s++)
        for (int l = 0; l + s <= n; l++) {
            int r = l + s;
            dp[l][r] = LLONG_MAX;
            for (int m = l + 1; m < r; m++)
                dp[l][r] = min(dp[l][r], dp[l][m] + dp[m][r] + cost(l, r));
        }
    return dp[0][n];
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 30;
        ll hi = vector<ll>{0, 1, 3, 100, 1000000}[rng() % 5];
        vector<ll> x(n + 1, 0);
        for (int i = 1; i <= n; i++) x[i] = x[i - 1] + (ll)(rng() % (hi + 1)); // gaps may be 0
        auto lin = [&](int l, int r) -> ll { return x[r] - x[l]; };
        auto sq = [&](int l, int r) -> ll { return (x[r] - x[l]) * (x[r] - x[l]); };
        chk("knuth(stick cutting x[r]-x[l])", knuth(n, lin) == brute(n, lin));
        chk("knuth((x[r]-x[l])^2)", knuth(n, sq) == brute(n, sq));
        if (n <= 2) chk("knuth(n <= 2)", knuth(n, lin) == (n == 2 ? x[2] - x[0] : 0));
    }
    // merging piles a[0..n-1], a merge costs the total of both parts; pile i lies between
    // points i and i+1
    for (int it = 0; it < 5000; it++) {
        int n = rng() % 25 + 1;
        vector<ll> p(n + 1, 0);
        for (int i = 0; i < n; i++) p[i + 1] = p[i] + (ll)(rng() % 1000);
        auto cost = [&](int l, int r) -> ll { return p[r] - p[l]; };
        // independent oracle: classic dp over piles [i, j], merge cost sum(i..j)
        vector<vector<ll>> d(n, vector<ll>(n, 0));
        for (int len = 2; len <= n; len++)
            for (int i = 0; i + len <= n; i++) {
                int j = i + len - 1;
                d[i][j] = LLONG_MAX;
                for (int m = i; m < j; m++) d[i][j] = min(d[i][j], d[i][m] + d[m + 1][j]);
                d[i][j] += p[j + 1] - p[i];
            }
        chk("knuth(merging piles)", knuth(n, cost) == d[0][n - 1]);
    }
    {
        int n = 2000;
        vector<ll> x(n + 1, 0);
        for (int i = 1; i <= n; i++) x[i] = x[i - 1] + (ll)(rng() % 1000000 + 1);
        auto t0 = clock();
        ll r = knuth(n, [&](int l, int r) -> ll { return x[r] - x[l]; });
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2000: %.3fs (ans=%lld)\n", t, r);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
