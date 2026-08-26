#include "../../core.h"

ll dp[20][2][2][90], k;
string L, R;

ll rec(int idx = 0, bool not_smaller = true, bool not_greater = true, int sum = 0) {
    if (idx == (int)L.size()) return !sum;

    ll &ret = dp[idx][not_smaller][not_greater][sum];
    if (~ret) return ret;

    int l_val = L[idx] - '0';
    int r_val = R[idx] - '0';
    int low = (not_smaller ? l_val : 0);
    int high = (not_greater ? r_val : 9);

    ret = 0;
    for (int d = low; d <= high; d++) {
        bool nxt_smaller = not_smaller && (d == l_val);
        bool nxt_greater = not_greater && (d == r_val);
        int nxt_sum = (sum + d) % k;

        ret += rec(idx + 1, nxt_smaller, nxt_greater, nxt_sum);
    }

    // Trick for multiple queries: To reuse the DP table without clearing it,
    // only memoize states that are completely free from the prefix limits.
    // bool is_free = (!not_smaller && !not_greater);
    // if (is_free) memo[idx][sum] = ret;

    return ret;
}

void solve(ll l, ll r) {
    memset(dp, -1, sizeof(dp));
    L = to_string(l);
    R = to_string(r);
    if (L.size() < R.size()) L = string(R.size() - L.size(), '0') + L;
}