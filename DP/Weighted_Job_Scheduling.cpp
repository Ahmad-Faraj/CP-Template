// Weighted Job Scheduling: maximum total profit from a set of pairwise non-overlapping jobs.
// Use when: "choose non-overlapping intervals maximising total value" — meetings, bookings, jobs with a weight.
// Handles: unsorted input, arbitrary endpoints, ties, duplicates; touching intervals count as non-overlapping.
// Time: O(n log n)
// Indexing: 0-based
// Note: a job is {end, start, profit} in that order. jobs is sorted in place.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

using Job = array<ll, 3>; // {end, start, profit}

ll max_profit(vector<Job> jobs) {
    int n = sz(jobs);
    if (!n) return 0;
    sort(jobs.begin(), jobs.end());
    vector<ll> dp(n);
    dp[0] = jobs[0][2];
    for (int i = 1; i < n; i++) {
        int k = (int)(lower_bound(jobs.begin(), jobs.end(), Job{jobs[i][1], 0, 0}) - jobs.begin()) - 1;
        dp[i] = max(dp[i - 1], (k >= 0 ? dp[k] : 0) + jobs[i][2]);
    }
    return dp[n - 1];
}

// Standard problem: n jobs given as start, end, profit; report the largest achievable profit
void solve() {
    int n;
    cin >> n;
    vector<Job> jobs(n);
    for (int i = 0; i < n; i++) cin >> jobs[i][1] >> jobs[i][0] >> jobs[i][2];
    cout << max_profit(jobs) << '\n';
}
