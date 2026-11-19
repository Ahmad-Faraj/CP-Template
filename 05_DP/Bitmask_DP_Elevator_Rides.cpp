// Elevator Rides (bitmask DP): fewest rides of capacity x carrying people of weights w (CSES 1653).
// Use when: "pack n <= 20 items into the fewest bins of capacity x", any subset-of-items DP.
// Usage: int rides = elevator_rides(w, x);
// Handles: n = 0 (0 rides).
// Time: O(2^n * n), memory O(2^n)
// Indexing: 0-based people, bit j of mask = person j
// Note: needs 1 <= w[i] <= x.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int elevator_rides(const vector<ll> &w, ll x) {
    int n = w.size();
    vector<pair<int, ll>> dp(1 << n);
    dp[0] = {0, 0};
    for (int mask = 1; mask < (1 << n); mask++) {
        dp[mask] = {n + 1, 0};
        for (int j = 0; j < n; j++) {
            if (!(mask >> j & 1)) continue;
            auto cur = dp[mask ^ (1 << j)];
            if (cur.second + w[j] <= x) cur.second += w[j];
            else cur.first++, cur.second = w[j];
            dp[mask] = min(dp[mask], cur);
        }
    }
    auto [closed, open] = dp[(1 << n) - 1];
    return closed + (open > 0);
}
