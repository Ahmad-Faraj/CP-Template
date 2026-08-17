// Meet in the Middle: splits the array in half, enumerates each half's subset sums, and joins them by binary search.
// Use when: n is around 40 - too big for 2^n, and the values are too large for a knapsack over the sum.
// Handles: counting subsets of a given sum, the largest subset sum not exceeding a cap, negatives, duplicates.
// Time: O(2^(n/2) * n) | Memory: O(2^(n/2))
// Indexing: 0-based, the whole vector
// Note: n above ~44 will not fit in memory. Sums are long long; the subset count itself can reach 2^n, so it is too.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> subset_sums(const vector<ll> &a) { // all 2^|a| subset sums, unsorted, the empty subset included
    vector<ll> res{0};
    res.reserve((size_t)1 << a.size());
    for (ll v : a) {
        int m = (int)res.size();
        for (int i = 0; i < m; i++) res.push_back(res[i] + v);
    }
    return res;
}

ll count_subsets_with_sum(const vector<ll> &a, ll x) { // how many subsets sum to exactly x
    int n = (int)a.size(), h = n / 2;
    vector<ll> L = subset_sums(vector<ll>(a.begin(), a.begin() + h));
    vector<ll> R = subset_sums(vector<ll>(a.begin() + h, a.end()));
    sort(R.begin(), R.end());
    ll ans = 0;
    for (ll v : L) {
        ll need = x - v;
        ans += upper_bound(R.begin(), R.end(), need) - lower_bound(R.begin(), R.end(), need);
    }
    return ans;
}

// largest subset sum that is <= cap, or LLONG_MIN when no subset qualifies
ll best_subset_sum_at_most(const vector<ll> &a, ll cap) {
    int n = (int)a.size(), h = n / 2;
    vector<ll> L = subset_sums(vector<ll>(a.begin(), a.begin() + h));
    vector<ll> R = subset_sums(vector<ll>(a.begin() + h, a.end()));
    sort(R.begin(), R.end());
    ll best = LLONG_MIN;
    for (ll v : L) {
        auto it = upper_bound(R.begin(), R.end(), cap - v);
        if (it == R.begin()) continue;
        best = max(best, v + *prev(it));
    }
    return best;
}

// Standard problem: CSES 1628 - count the subsets of n numbers whose sum is exactly x
void solve() {
    int n;
    ll x;
    cin >> n >> x;
    vector<ll> a(n);
    for (ll &v : a) cin >> v;
    cout << count_subsets_with_sum(a, x) << '\n';
}
