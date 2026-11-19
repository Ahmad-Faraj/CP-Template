// Deque Trick: DP whose transition is a max over the last k states, kept in a monotonic deque.
// Use when: dp[i][j] = a[i] + max(dp[i-k..i-1][j-1]), "every k consecutive must contain a pick"
// (CF 1077F2), any sliding-window max/min inside a DP transition.
// Usage: ll best = max_pick_every_k(a, k, x); // exactly x picks, every k in a row has one, else -1
// Handles: impossible (-1), k >= n, x = 0, x > n.
// Time: O(n * x), memory O(n)
// Indexing: a is 0-based
// Note: needs a[i] >= 0 (-1 means impossible). For a min window pop on <= and use +INF for "none".

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll max_pick_every_k(const vector<ll> &a, int k, int x) {
    int n = a.size();
    if (x > n) return -1;
    const ll NEG = LLONG_MIN / 4;
    vector<ll> prv(n + 1, NEG), cur(n + 1);
    prv[0] = 0;
    for (int j = 1; j <= x; j++) {
        fill(cur.begin(), cur.end(), NEG);
        deque<array<ll, 2>> dq;
        dq.push_back({j - 1, prv[j - 1]});
        for (int i = j; i <= n; i++) {
            while (dq.size() && dq.front()[0] < i - k) dq.pop_front();
            if (dq.size() && dq.front()[1] > NEG) cur[i] = dq.front()[1] + a[i - 1];
            while (dq.size() && prv[i] >= dq.back()[1]) dq.pop_back();
            dq.push_back({i, prv[i]});
        }
        swap(prv, cur);
    }
    ll ans = -1;
    for (int i = n; i >= 0 && i > n - k; i--) ans = max(ans, prv[i]);
    return ans;
}
