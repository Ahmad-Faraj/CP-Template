// Deque Trick: max of the previous DP layer over a sliding window of the last k indices, O(1) per index.
// Use when: dp[i] = a[i] + max(dp[i-k..i-1]), "at least one chosen every k consecutive", window max in a transition.
// Handles: fixed-width trailing window, negative values, any DP layer count. Width is fixed at construction.
// Time: O(n) per DP layer
// Indexing: 1-based
// Note: call best(i) before push(i, .) for each i, and push in increasing i. Negate values for a minimum window.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

struct MaxWindow {
    int k;
    ll none;                // returned when the window holds nothing; low enough to survive one addition
    deque<array<ll, 2>> dq; // {index, value}

    MaxWindow(int k, ll none = -4e18) : k(k), none(none) {}

    ll best(int i) { // max of pushed values whose index lies in [i - k, i - 1]
        while (sz(dq) && dq.front()[0] < i - k) dq.pop_front();
        return dq.empty() ? none : dq.front()[1];
    }

    void push(int i, ll v) { // offer value v at index i
        while (sz(dq) && dq.back()[1] <= v) dq.pop_back();
        dq.push_back({i, v});
    }
};

// Standard problem: pick exactly x of n pictures so every window of k has one, maximising the sum (CF 1077 F2)
void solve() {
    int n, k, x;
    cin >> n >> k >> x;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    vector<vector<ll>> dp(n + 1, vector<ll>(x + 1, -4e18));
    dp[0][0] = 0;
    for (int j = 1; j <= x; j++) {
        MaxWindow w(k);
        w.push(j - 1, dp[j - 1][j - 1]);
        for (int i = j; i <= n; i++) {
            dp[i][j] = w.best(i) + a[i];
            w.push(i, dp[i][j - 1]);
        }
    }
    ll ans = -1;
    for (int i = 0; i < k; i++) ans = max(ans, dp[n - i][x]);
    cout << ans << '\n';
}
