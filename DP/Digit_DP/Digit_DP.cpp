#include "../../core.h"

/*
 * Topic: DP - Digit DP
 * Description: Used to count the number of integers in a range [L, R] that satisfy
 * a certain property related to their digits. It builds the number digit by digit.
 * Input: A range [L, R] and a digit-based condition.
 * Output: Count of valid integers within the given range.
 */

ll dp[20][2][2][90], k;
string L, R;

ll rec(int idx = 0, bool not_smaller = true, bool not_greater = true, int sum = 0) 
{    
    if (idx == L.size()) return !sum;
    ll &ret = dp[idx][not_smaller][not_greater][sum];
 
    if (~ret) return ret;
    int low = (not_smaller ? L[idx] - '0' : 0), high = (not_greater ? R[idx] - '0' : 9);
    ret = 0;

    for (int d = low; d <= high; d++)
        ret += rec(idx + 1, not_smaller & (d == (L[idx] - '0')), not_greater & (d == (R[idx] - '0')), (sum + d) % k);
 
    // Trick for big queries count: To reuse the DP table without clearing it between queries,
    // only memoize states that are completely free from the prefix limits.
    // bool is_free = (!not_smaller && !not_greater);
    // if (is_free) memo[rem][current_sum] = result; // or in this case: dp[idx][0][0][sum] = ret;
    
    return ret;
}

void solve(ll l, ll r) {
    memset(dp, -1, sizeof(dp));
    L = to_string(l);
    R = to_string(r);
    if (L.size() < R.size()) L = string(R.size() - L.size(), '0') + L;
}