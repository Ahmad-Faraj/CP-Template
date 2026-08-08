#include "../../core.h"

/*
 * Topic: DP - Deque Trick (Sliding Window Min/Max)
 * Description: Optimizes DP transitions of the form dp[i] = max_{i-k <= j < i} (dp[j]) + a[i]
 * It reduces the complexity from O(NK^2) or O(NK) to O(NK) or O(N) respectively,
 * by answering min/max queries among k consecutive elements in O(1) amortized.
 * Input: An array a and a sliding window size k.
 * Output: The optimal DP value taking the max/min of the previous k states.
 * Problem link: https://codeforces.com/contest/1077/problem/F2
 */

void solve() {
    int n, k, x; cin >> n >> k >> x;
    vector<int> a(n + 1);
    for (int i = 1; i <= n; i++) cin >> a[i];
    ll dp[n + 1][x + 1]; // dp[i][j] = answer using j pictures from the first i-th pictures
    memset(dp, -0x3f, sizeof dp);
    dp[0][0] = 0;
    for (int j = 1; j <= x; j++) {
        deque<vector<ll>> dq; // {id, value}
        dq.push_back({j - 1, dp[j - 1][j - 1]});
        for (int i = j; i <= n; i++) {
            while (dq.size() && dq.front()[0] < i - k) dq.pop_front();
            dp[i][j] = dq.front()[1] + a[i];
            while (dq.size() && dp[i][j - 1] >= dq.back()[1]) dq.pop_back();
            dq.push_back({i, dp[i][j - 1]});
        }

    }
    ll ans = -1;
    for (int i = 0; i < k; i++) ans = max(ans, dp[n - i][x]);
    cout << ans << "\n";
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    int tc = 1; 
    // cin >> tc;
    for (int t = 1; t <= tc; t++) {
        // cout << "Case #" << t  << ": ";
        solve();
    }
}
