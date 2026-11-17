// DnC Optimization: min cost to split items 1..n into exactly k non-empty consecutive groups.
// Use when: dp[i][j] = min over t < j of dp[i-1][t] + cost(t+1, j) and the best t never moves
// left as j grows: cost = (group sum)^2, "pairs inside a group" (CF 321E), quadrangle inequality.
// Usage: ll best = dnc_dp(n, k, [&](int l, int r) -> ll { return cost of items l..r; });
// Handles: k = 1, k = n; k > n or k = 0 give LLONG_MAX / 4. Max: pass -cost, negate the answer.
// Time: O(k * n log n) calls of cost, recursion depth O(log n)
// Indexing: 1-based items; cost(l, r) is called with 1 <= l <= r <= n
// Note: cost must be O(1) (prefix sums). Without the monotone split point it is silently wrong.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class F> ll dnc_dp(int n, int k, F cost) {
    const ll INF = LLONG_MAX / 4;
    vector<ll> prv(n + 1, INF), cur(n + 1); // prv[j] = dp[i-1][j], cur[j] = dp[i][j]
    for (int j = 1; j <= n; j++) prv[j] = cost(1, j);
    // fills cur[l..r] knowing their best split points lie in [optl, optr]
    auto rec = [&](auto &&self, int l, int r, int optl, int optr) -> void {
        if (l > r) return;
        int mid = (l + r) / 2, opt = -1;
        cur[mid] = INF;
        for (int t = optl; t <= min(mid - 1, optr); t++) {
            if (prv[t] >= INF) continue;
            ll c = prv[t] + cost(t + 1, mid);
            if (c < cur[mid]) cur[mid] = c, opt = t;
        }
        if (opt == -1) { // 1..mid cannot form i groups, so neither can anything left of mid
            for (int j = l; j < mid; j++) cur[j] = INF;
            self(self, mid + 1, r, optl, optr);
            return;
        }
        self(self, l, mid - 1, optl, opt);
        self(self, mid + 1, r, opt, optr);
    };
    for (int i = 2; i <= k; i++) {
        rec(rec, 1, n, 1, n);
        swap(prv, cur);
    }
    return k >= 1 && k <= n ? prv[n] : INF;
}
