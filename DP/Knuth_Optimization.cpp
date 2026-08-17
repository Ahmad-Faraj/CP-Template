// Knuth Optimization: dp[l][r] = min over l < m < r of dp[l][m] + dp[m][r] + cost(l, r).
// Use when: interval DP merging two adjacent ranges - stick cutting, optimal BST, merging stones - when O(n^3) is slow.
// Handles: any cost with opt[l][r-1] <= opt[l][r] <= opt[l+1][r]. Not for costs breaking that monotonicity.
// Time: O(n^2)
// Indexing: 0-based (a[0] and a[n+1] are the two ends)
// Note: replace cost() with the problem's cost. Sized n <= 1000.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 1010;

ll a[N], res[N][N];
int opt[N][N];

ll cost(int l, int r) { return a[r] - a[l]; }

ll knuth(int n) { // n = index of the last point; returns the optimum over [0, n]
    for (int s = 0; s <= n; s++)
        for (int l = 0; l + s <= n; l++) {
            int r = l + s;
            if (s < 2) {
                res[l][r] = 0, opt[l][r] = l;
                continue;
            }
            res[l][r] = 2e18;
            for (int m = opt[l][r - 1]; m <= opt[l + 1][r]; m++) {
                ll cur = res[l][m] + res[m][r] + cost(l, r);
                if (res[l][r] > cur) res[l][r] = cur, opt[l][r] = m;
            }
        }
    return res[0][n];
}

// Standard problem: cut a stick of length x at n points, a cut costs the current piece's length (UVA 10003)
void solve() {
    int x, n;
    while (cin >> x && x) {
        cin >> n;
        a[0] = 0, a[n + 1] = x;
        for (int i = 1; i <= n; i++) cin >> a[i];
        cout << "The minimum cutting is " << knuth(n + 1) << ".\n";
    }
}
