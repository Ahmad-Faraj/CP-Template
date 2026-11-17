// Test for 05_DP/Deque_Trick.cpp (CF 1077F2) against (a) every subset of positions for small n and
// (b) the O(n^2 x) DP that scans the whole window for medium n.
// Checks: max_pick_every_k on random a, k, x including k = 1, k >= n, x = 0, x = n, x > n,
// impossible cases (-1), zeros and duplicates, values up to 1e9, and an n = 5000 timing run.
#include "../../05_DP/Deque_Trick.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(1077);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll brute_subsets(const vector<ll> &a, int k, int x) {
    int n = a.size();
    ll best = -1;
    for (int m = 0; m < (1 << n); m++) {
        if (__builtin_popcount(m) != x) continue;
        bool ok = true;
        for (int s = 0; s + k <= n && ok; s++) ok = (m >> s) & ((1 << k) - 1);
        if (!ok) continue;
        ll sum = 0;
        for (int i = 0; i < n; i++)
            if (m >> i & 1) sum += a[i];
        best = max(best, sum);
    }
    return best;
}

// dp[i][j]: j picks, the last one at position i (1-based)
ll brute_dp(const vector<ll> &a, int k, int x) {
    int n = a.size();
    const ll NO = LLONG_MIN;
    vector<vector<ll>> dp(n + 1, vector<ll>(x + 1, NO));
    dp[0][0] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= x; j++)
            for (int p = max(0, i - k); p < i; p++)
                if (dp[p][j - 1] != NO) dp[i][j] = max(dp[i][j], dp[p][j - 1] + a[i - 1]);
    ll best = -1;
    for (int i = max(0, n - k + 1); i <= n; i++)
        if (x <= n && dp[i][x] != NO) best = max(best, dp[i][x]);
    return best;
}

int main() {
    for (int it = 0; it < 60000; it++) {
        int n = rng() % 13, k = rng() % (n + 3) + 1, x = rng() % (n + 3);
        ll hi = vector<ll>{1, 3, 100, 1000000000}[rng() % 4];
        vector<ll> a(n);
        for (auto &v : a) v = rng() % (hi + 1);
        ll got = max_pick_every_k(a, k, x), want = brute_subsets(a, k, x);
        chk("max_pick_every_k(vs subsets)", got == want);
        if (want == -1) chk("max_pick_every_k(impossible)", got == -1);
        if (k >= n) chk("max_pick_every_k(k >= n)", got == want);
        if (x == 0) chk("max_pick_every_k(x = 0)", got == want);
        if (x > n) chk("max_pick_every_k(x > n)", got == -1);
    }
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 60 + 1, k = rng() % n + 1, x = rng() % (n + 1);
        vector<ll> a(n);
        for (auto &v : a) v = rng() % 1000000001;
        chk("max_pick_every_k(vs O(n^2 x) dp)", max_pick_every_k(a, k, x) == brute_dp(a, k, x));
    }
    chk("max_pick_every_k(k = 1 takes all)", max_pick_every_k({4, 5, 6}, 1, 3) == 15);
    chk("max_pick_every_k(k = 1 too few)", max_pick_every_k({4, 5, 6}, 1, 2) == -1);
    chk("max_pick_every_k(CF sample 1)", max_pick_every_k({5, 1, 3, 10, 1}, 2, 3) == 18);
    chk("max_pick_every_k(CF sample 2)", max_pick_every_k({10, 30, 30, 70, 10, 10}, 1, 5) == -1);
    chk("max_pick_every_k(CF sample 3)", max_pick_every_k({1, 100, 1, 1}, 3, 1) == 100);
    chk("max_pick_every_k(n = 0)", max_pick_every_k({}, 1, 0) == 0);
    {
        int n = 5000;
        vector<ll> a(n);
        for (auto &v : a) v = rng() % 1000000000 + 1;
        auto t0 = clock();
        ll r = max_pick_every_k(a, 50, 2500);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=5000 k=50 x=2500 (CF F2 scale): %.3fs (ans=%lld)\n", t, r);
        chk("max_pick_every_k(large, x = n)", max_pick_every_k(a, 5000, 5000) ==
                                                   accumulate(a.begin(), a.end(), 0LL));
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-36s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
