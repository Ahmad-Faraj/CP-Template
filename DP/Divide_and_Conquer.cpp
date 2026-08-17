// Divide & Conquer DP: dp[i][j] = min over k < j of dp[i-1][k] + cost(k+1, j), split [1..n] into i parts.
// Use when: "split the array into exactly k consecutive groups minimising total cost", k layers over n items.
// Handles: any cost() whose optimal split point never moves backwards. Not for costs that break that.
// Time: O(parts * n log n)
// Indexing: 1-based
// Note: needs opt[i][j] non-decreasing in j. Replace cost() with the problem's cost. Sized n <= 4000, parts <= 800.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const int N = 4010, inf = 1e9;

int a[N][N], c[N][N], dp[810][N]; // dp[i][j] = minimum cost of splitting [1..j] into i parts

ll cost(int i, int j) { return c[j][j] - c[i - 1][j] - c[j][i - 1] + c[i - 1][i - 1]; }

void dc(int i, int l, int r, int optl, int optr) {
    if (l > r) return;
    int mid = (l + r) / 2, opt = -1;
    dp[i][mid] = inf;
    for (int k = optl; k <= min(mid - 1, optr); k++) {
        int cur = dp[i - 1][k] + cost(k + 1, mid);
        if (cur < dp[i][mid]) {
            dp[i][mid] = cur;
            opt = k;
        }
    }
    if (opt == -1) { // [1..mid] cannot be split into i parts, so no split point exists to the left either
        dc(i, mid + 1, r, optl, optr);
        return;
    }
    dc(i, l, mid - 1, optl, opt);
    dc(i, mid + 1, r, opt, optr);
}

// Standard problem: split 1..n into k consecutive groups minimising the total pair cost (CF 321E)
void solve() {
    int n, k;
    cin >> n >> k;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) cin >> a[i][j];
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) c[i][j] = a[i][j] + c[i - 1][j] + c[i][j - 1] - c[i - 1][j - 1];
    for (int i = 1; i <= k; i++)
        for (int j = 1; j <= n; j++) dp[i][j] = inf;
    for (int i = 1; i <= n; i++) dp[1][i] = cost(1, i);
    for (int i = 2; i <= k; i++) dc(i, 1, n, 1, n);
    cout << dp[k][n] / 2 << '\n';
}
