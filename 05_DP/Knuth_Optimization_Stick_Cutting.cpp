// Stick Cutting: min total cost to make every cut of a stick; a cut costs the current piece length.
// Use when: UVA 10003 / ZOJ 2860 "breaking strings", any "cut or merge in some order, pay the
// piece size" interval DP (matrix-chain shape) whose O(n^3) is too slow.
// Usage: ll best = stick_cutting(len, cuts); // cuts = distinct positions in (0, len), any order
// Handles: no cuts (0), unsorted cuts.
// Time: O(n^2), n = number of cuts
// Indexing: none (cuts are positions along the stick)

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll stick_cutting(ll len, vector<ll> cuts) {
    sort(cuts.begin(), cuts.end());
    int n = cuts.size();
    vector<ll> p(n + 2);
    p[0] = 0, p[n + 1] = len;
    for (int i = 0; i < n; i++) p[i + 1] = cuts[i];
    vector<vector<ll>> dp(n + 2, vector<ll>(n + 2, 0));
    vector<vector<int>> opt(n + 2, vector<int>(n + 2));
    for (int i = 0; i <= n; i++) opt[i][i + 1] = i + 1;
    for (int i = n + 1; i >= 0; i--)
        for (int j = i + 2; j <= n + 1; j++) {
            dp[i][j] = LLONG_MAX;
            for (int k = max(i + 1, opt[i][j - 1]); k <= min(j - 1, opt[i + 1][j]); k++)
                if (dp[i][k] + dp[k][j] + p[j] - p[i] < dp[i][j])
                    dp[i][j] = dp[i][k] + dp[k][j] + p[j] - p[i], opt[i][j] = k;
        }
    return dp[0][n + 1];
}
