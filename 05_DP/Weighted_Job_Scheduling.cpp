// Weighted Job Scheduling: max total profit of pairwise non-overlapping jobs {start, end, profit}.
// Use when: "choose non-overlapping intervals maximising total value" - meetings, bookings.
// Usage: ll best = max_profit(jobs); // jobs[i] = {start, end, profit}
// Handles: unsorted input, ties, duplicates, negative times, negative profits (skipped), n = 0.
// Time: O(n log n)
// Indexing: 0-based
// Note: intervals are CLOSED: a job ending at t and one starting at t overlap. For half-open [s, e)
// change `j[1] < s` to `j[1] <= s`. Needs start <= end.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll max_profit(vector<array<ll, 3>> jobs) {
    sort(jobs.begin(), jobs.end(), [](auto &a, auto &b) { return a[1] < b[1]; });
    int n = jobs.size();
    vector<ll> dp(n + 1, 0);
    for (int i = 0; i < n; i++) {
        ll s = jobs[i][0];
        auto before = [&](auto &j) { return j[1] < s; };
        int k = partition_point(jobs.begin(), jobs.begin() + i, before) - jobs.begin();
        dp[i + 1] = max(dp[i], dp[k] + jobs[i][2]);
    }
    return dp[n];
}
