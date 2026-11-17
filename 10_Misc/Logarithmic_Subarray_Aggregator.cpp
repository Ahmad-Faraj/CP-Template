// Logarithmic Subarray Aggregator: counts how many subarrays give each gcd (or AND / OR) value.
// Use when: "how many subarrays have gcd exactly x", same for bitwise AND / OR, distinct values.
// Usage: auto cnt = subarray_counts(a, [](ll x, ll y) { return __gcd(x, y); }); cnt[x];
// Handles: gcd, AND, OR (any op where growing a subarray moves the value one way), duplicates.
// Time: O(n log(max) log n); only O(log max) distinct values end at each index
// Indexing: 0-based array; the result maps each achievable value to its subarray count
// Note: wrong for sum, max-min or xor (values do not collapse). Counts reach n(n+1)/2, so ll.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T, typename Op> map<T, ll> subarray_counts(const vector<T> &a, Op op) {
    map<T, ll> ans;
    vector<pair<T, ll>> v; // (value, how many subarrays ending at the current index give it)
    for (const T &x : a) {
        vector<pair<T, ll>> u;
        u.push_back({x, 1});
        for (auto [y, c] : v) {
            T g = op(y, x);
            if (g == u.back().first) u.back().second += c; // equal values are adjacent: merge
            else u.push_back({g, c});
        }
        v = u;
        for (auto [y, c] : v) ans[y] += c;
    }
    return ans;
}
