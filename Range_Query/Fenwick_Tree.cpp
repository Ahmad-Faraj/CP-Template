// Fenwick Tree: prefix and range sums under point updates, plus search by prefix sum.
// Use when: "sum on [l, r] with updates", counting inversions, k-th smallest over a value axis.
// Handles: point add/set, range sum, range add + point read, prefix-sum lower/upper bound, 0- or 1-based.
// Time: build O(n log n) | add O(log n) | sum O(log n) | lower_bound O(log n)
// Indexing: 1-based by default; FenwickTree<false> for 0-based
// Note: update_range() makes the tree a difference array - read a point with sum(idx), and never call sum(l, r).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <bool one_based = true> struct FenwickTree {
    vector<ll> bit;
    int n;

    FenwickTree(int m = 0) : bit(m + 1, 0), n(m + 1) {}

    FenwickTree(const vector<ll> &a) : FenwickTree((int)a.size()) {
        for (int i = 0; i < (int)a.size(); i++) add(i + one_based, a[i]);
    }

    void add(int idx, ll delta) { // a[idx] += delta
        for (idx += !one_based; idx < n; idx += idx & -idx) bit[idx] += delta;
    }

    ll sum(int idx) { // a[first .. idx]
        ll ret = 0;
        for (idx += !one_based; idx > 0; idx -= idx & -idx) ret += bit[idx];
        return ret;
    }

    ll sum(int l, int r) { return sum(r) - (l - 1 >= 0 ? sum(l - 1) : 0); } // a[l .. r]

    ll get(int idx) { return sum(idx) - sum(idx - 1); } // a[idx]

    void set(int idx, ll val) { add(idx, val - get(idx)); } // a[idx] = val

    void update_range(int l, int r, ll val) { add(l, val), add(r + 1, -val); } // a[l .. r] += val, read via sum(idx)

    int lower_bound(ll x) { // smallest idx whose prefix sum is >= x; values must be non-negative
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1)
            if (idx + mask < n && bit[idx + mask] < x) x -= bit[idx + mask], idx += mask;
        return idx + one_based;
    }

    int upper_bound(ll x) { // smallest idx whose prefix sum is > x; values must be non-negative
        int idx = 0, mask = 1;
        while (mask < n) mask <<= 1;
        for (mask >>= 1; mask > 0; mask >>= 1)
            if (idx + mask < n && bit[idx + mask] <= x) x -= bit[idx + mask], idx += mask;
        return idx + one_based;
    }
};

// Standard problem: point assignment and range sum (CSES 1648 - Dynamic Range Sum Queries)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    FenwickTree<> ft(a);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k;
            ll u;
            cin >> k >> u;
            ft.set(k, u);
        } else {
            int l, r;
            cin >> l >> r;
            cout << ft.sum(l, r) << '\n';
        }
    }
}
