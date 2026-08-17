// Logarithmic Subarray Aggregator: counts subarrays by gcd, AND or OR, since only O(log) values occur per endpoint.
// Use when: "how many subarrays have gcd exactly x", the same for bitwise AND or OR, or the distinct values themselves.
// Handles: any operation that only ever loses information (gcd, AND, OR), duplicates, and answers up to n*(n+1)/2.
// Time: O(n log(max value) * log n) with a map, or O(n log(max value)) if you collect into a vector
// Indexing: 0-based array; the result maps each achievable value to how many subarrays produce it
// Note: this works only because gcd/AND/OR are monotone - each step can only shrink the value. Wrong for sums.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// how many subarrays produce each value under `op`; op must be associative and monotone
// (repeatedly applying it can only move the value in one direction, as gcd, AND and OR do)
template <typename T, typename Op> map<T, ll> subarray_value_counts(const vector<T> &a, Op op) {
    map<T, ll> total;
    vector<pair<T, ll>> ending; // (value, how many subarrays ending here produce it)
    for (const T &x : a) {
        vector<pair<T, ll>> next;
        next.push_back({x, 1});
        for (auto [value, count] : ending) {
            T combined = op(value, x);
            if (combined == next.back().first)
                next.back().second += count; // same value, so merge the runs
            else
                next.push_back({combined, count});
        }
        ending = move(next);
        for (auto [value, count] : ending) total[value] += count;
    }
    return total;
}

map<ll, ll> subarray_gcd_counts(const vector<ll> &a) {
    return subarray_value_counts(a, [](ll p, ll q) { return __gcd(p, q); });
}

map<ll, ll> subarray_and_counts(const vector<ll> &a) {
    return subarray_value_counts(a, [](ll p, ll q) { return p & q; });
}

map<ll, ll> subarray_or_counts(const vector<ll> &a) {
    return subarray_value_counts(a, [](ll p, ll q) { return p | q; });
}

// the distinct values only, without the counts
template <typename T, typename Op> vector<T> distinct_subarray_values(const vector<T> &a, Op op) {
    map<T, ll> counts = subarray_value_counts(a, op);
    vector<T> out;
    for (auto &kv : counts) out.push_back(kv.first);
    return out;
}

// the (value, count) pairs for subarrays ending at each index, which is the structure behind all of this
template <typename T, typename Op> vector<vector<pair<T, ll>>> per_endpoint(const vector<T> &a, Op op) {
    vector<vector<pair<T, ll>>> out;
    vector<pair<T, ll>> ending;
    for (const T &x : a) {
        vector<pair<T, ll>> next;
        next.push_back({x, 1});
        for (auto [value, count] : ending) {
            T combined = op(value, x);
            if (combined == next.back().first)
                next.back().second += count;
            else
                next.push_back({combined, count});
        }
        ending = move(next);
        out.push_back(ending);
    }
    return out;
}

// Standard problem: Codeforces 475D - for each query x, how many subarrays have gcd exactly x
void solve() {
    int n;
    cin >> n;
    vector<ll> a(n);
    for (ll &x : a) cin >> x;
    map<ll, ll> counts = subarray_gcd_counts(a);
    int q;
    cin >> q;
    while (q--) {
        ll x;
        cin >> x;
        auto it = counts.find(x);
        cout << (it == counts.end() ? 0 : it->second) << '\n';
    }
}
