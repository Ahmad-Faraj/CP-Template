// Digit DP: counts integers in [l, r] whose digit sum is divisible by k.
// Use when: "how many numbers in [l, r] with <property of their digits>", bounds up to 1e18, answer is a count.
// Handles: any per-digit predicate by changing the state; both bounds tight at once. Not for querying one number.
// Time: O(len * 2 * 2 * k * 10), len = number of digits of r
// Indexing: 0-based (string index)
// Note: sized for r < 1e20 and k <= 90. Set k and call solve(); rec() needs L padded to |R|.

#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define sz(x) (int)(x).size()

ll dp[20][2][2][90], k;
string L, R;

ll rec(int idx = 0, bool not_smaller = true, bool not_greater = true, int sum = 0) {
    if (idx == sz(L)) return !sum;
    ll &ret = dp[idx][not_smaller][not_greater][sum];
    if (~ret) return ret;
    int low = (not_smaller ? L[idx] - '0' : 0), high = (not_greater ? R[idx] - '0' : 9);
    ret = 0;
    for (int d = low; d <= high; d++)
        ret += rec(idx + 1, not_smaller & (d == (L[idx] - '0')), not_greater & (d == (R[idx] - '0')), (sum + d) % k);
    return ret;
}

void solve() {
    ll l, r;
    cin >> l >> r >> k;
    memset(dp, -1, sizeof(dp));
    L = to_string(l), R = to_string(r);
    if (sz(L) < sz(R)) L = string(sz(R) - sz(L), '0') + L;
    cout << rec() << '\n';
}
