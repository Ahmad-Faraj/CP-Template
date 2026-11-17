// Meet In The Middle: counts the subsets with sum exactly x by splitting the array in two halves.
// Use when: n up to ~40, values too large for a knapsack, "how many subsets sum to x".
// Usage: ll ways = count_subsets_with_sum(a, x);
// Handles: negatives, zeros, duplicates, the empty subset (counted when x == 0), n = 0.
// Time: O(2^(n/2) * n)  |  Memory: O(2^(n/2))
// Indexing: 0-based; subset_sums(a, l, r) covers a[l..r] inclusive
// Note: n > ~44 does not fit in memory. Sums and the answer (up to 2^n) are long long.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> subset_sums(const vector<ll> &a, int l, int r) { // all 2^(r-l+1) sums, empty included
    int len = r - l + 1;
    vector<ll> res;
    for (int i = 0; i < (1 << len); i++) {
        ll sum = 0;
        for (int j = 0; j < len; j++)
            if (i >> j & 1) sum += a[l + j];
        res.push_back(sum);
    }
    return res;
}

ll count_subsets_with_sum(const vector<ll> &a, ll x) {
    int n = a.size();
    vector<ll> left = subset_sums(a, 0, n / 2 - 1), right = subset_sums(a, n / 2, n - 1);
    sort(right.begin(), right.end());
    ll ans = 0;
    for (ll s : left)
        ans += upper_bound(right.begin(), right.end(), x - s) -
               lower_bound(right.begin(), right.end(), x - s);
    return ans;
}
