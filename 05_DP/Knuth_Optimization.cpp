// Knuth Optimization: interval DP dp[l][r] = min over l < m < r of dp[l][m] + dp[m][r] + cost(l,r).
// Use when: split/merge adjacent ranges - stick cutting (UVA 10003), breaking strings
// (ZOJ 2860), optimal BST, merging piles - and the O(n^3) interval DP is too slow.
// Usage: ll best = knuth(n, [&](int l, int r) -> ll { return x[r] - x[l]; }); // dp[0][n]
// Handles: n = 0 and n = 1 (answer 0). Needs opt[l][r-1] <= opt[l][r] <= opt[l+1][r]: holds
// when cost satisfies the quadrangle inequality and never shrinks as the interval grows.
// Time: O(n^2), memory O(n^2)
// Indexing: 0-based points 0..n; dp[l][l+1] = 0 (nothing to split)
// Note: without the monotone opt it is silently wrong. The O(n^3) loop is m = l+1 .. r-1.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <class F> ll knuth(int n, F cost) {
    vector<vector<ll>> dp(n + 1, vector<ll>(n + 1, 0));
    vector<vector<int>> opt(n + 1, vector<int>(n + 1));
    for (int l = 0; l < n; l++) opt[l][l + 1] = l + 1;
    for (int s = 2; s <= n; s++) // s = length of the interval
        for (int l = 0; l + s <= n; l++) {
            int r = l + s; // the best split of [l, r] lies in [opt[l][r-1], opt[l+1][r]]
            dp[l][r] = LLONG_MAX;
            for (int m = max(l + 1, opt[l][r - 1]); m <= min(r - 1, opt[l + 1][r]); m++) {
                ll cur = dp[l][m] + dp[m][r] + cost(l, r);
                if (cur < dp[l][r]) dp[l][r] = cur, opt[l][r] = m;
            }
        }
    return dp[0][n];
}
