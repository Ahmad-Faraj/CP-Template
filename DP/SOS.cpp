// SOS DP: for every mask, aggregate dp over all of its submasks or all of its supermasks.
// Use when: "for each mask, sum/count over its subsets", counting pairs by AND/OR, inclusion-exclusion over bits.
// Handles: subset and superset sums plus their exact inverses, negative values, repeated composition. bits <= 22.
// Time: O(bits * 2^bits) per transform
// Indexing: 0-based (masks 0 .. 2^bits - 1)
// Note: every transform runs in place on dp[]. Each sum has an exact inverse, so they compose and undo.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SOS {
    int bits, n;
    vector<ll> dp;

    SOS(int bits) : bits(bits), n(1 << bits), dp(1 << bits, 0) {}

    void subset_sum() { // dp[mask] = sum of dp over every submask of mask
        for (int bit = 0; bit < bits; bit++)
            for (int mask = 0; mask < n; mask++)
                if (mask >> bit & 1) dp[mask] += dp[mask ^ (1 << bit)];
    }

    void superset_sum() { // dp[mask] = sum of dp over every supermask of mask
        for (int bit = 0; bit < bits; bit++)
            for (int mask = n - 1; mask >= 0; mask--)
                if (mask >> bit & 1) dp[mask ^ (1 << bit)] += dp[mask];
    }

    void subset_sum_inv() { // exact inverse of subset_sum()
        for (int bit = 0; bit < bits; bit++)
            for (int mask = n - 1; mask >= 0; mask--)
                if (mask >> bit & 1) dp[mask] -= dp[mask ^ (1 << bit)];
    }

    void superset_sum_inv() { // exact inverse of superset_sum()
        for (int bit = 0; bit < bits; bit++)
            for (int mask = 0; mask < n; mask++)
                if (mask >> bit & 1) dp[mask ^ (1 << bit)] -= dp[mask];
    }
};

// Standard problem: for each a[i] count elements that are submasks, supermasks, and neither (CSES 1654)
void solve() {
    const int B = 20;
    int n;
    cin >> n;
    vector<int> a(n);
    SOS sub(B), sup(B);
    for (int i = 0; i < n; i++) {
        cin >> a[i];
        sub.dp[a[i]]++, sup.dp[a[i]]++;
    }
    sub.subset_sum();
    sup.superset_sum();
    for (int i = 0; i < n; i++)
        cout << sub.dp[a[i]] << ' ' << sup.dp[a[i]] << ' ' << n - sub.dp[((1 << B) - 1) ^ a[i]] << '\n';
}
