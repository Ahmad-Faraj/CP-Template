// Digit DP: counts integers in [l, r] whose digit sum is divisible by k.
// Use when: "how many numbers in [l, r] with <property of their digits>", bounds up to 1e18, answer
//           is a count.
// Usage: digit_dp(l, r, k) -> count. To change the property, change the state (sum) and the base
//        case.
// Handles: l = 0, both bounds in one call (no f(r) - f(l - 1) needed).
// Time: O(len * 2 * 2 * k * 10), len = number of digits of r
// Indexing: none (works on the values l, r)
// Note: sized for 0 <= l <= r < 1e19 and 1 <= k <= 90. Leading zeros of the padded l add 0 to the
//       sum.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll dp[20][2][2][90], k;
string L, R;

ll rec(int idx = 0, bool not_smaller = true, bool not_greater = true, int sum = 0) {
    if (idx == (int)L.size()) return !sum;
    ll &ret = dp[idx][not_smaller][not_greater][sum];
    if (~ret) return ret;
    int low = (not_smaller ? L[idx] - '0' : 0), high = (not_greater ? R[idx] - '0' : 9);
    ret = 0;
    for (int d = low; d <= high; d++)
        ret += rec(idx + 1, not_smaller & (d == L[idx] - '0'),
                   not_greater & (d == R[idx] - '0'), (sum + d) % k);
    return ret;
}

ll digit_dp(ll l, ll r, ll K) {
    k = K;
    memset(dp, -1, sizeof(dp));
    L = to_string(l), R = to_string(r);
    if (L.size() < R.size()) L = string(R.size() - L.size(), '0') + L;
    return rec();
}
