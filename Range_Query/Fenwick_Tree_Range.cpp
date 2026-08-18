// Fenwick Tree Range: range add AND range sum together, using two Fenwick trees instead of one.
// Use when: "add x to a[l..r]" and "sum of a[l..r]" in the same problem - one plain BIT cannot do both.
// Handles: range add, point add, range sum, point read, negative values, and building from an array.
// Time: build O(n log n) | range add O(log n) | range sum O(log n)
// Indexing: 1-based, inclusive [l, r]
// Note: Fenwick_Tree.cpp does range add with POINT read only. This is the version when you need both ends ranged.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Fenwick_Tree_Range {
    int n;
    vector<ll> mul, add; // prefix(i) = sum(mul, i) * i - sum(add, i)

    Fenwick_Tree_Range(int n = 0) : n(n), mul(n + 2, 0), add(n + 2, 0) {}

    Fenwick_Tree_Range(const vector<ll> &a) : Fenwick_Tree_Range((int)a.size()) {
        for (int i = 0; i < (int)a.size(); i++) update(i + 1, i + 1, a[i]); // a is 0-based
    }

    void touch(vector<ll> &tree, int i, ll delta) {
        for (; i <= n; i += i & -i) tree[i] += delta;
    }

    ll gather(const vector<ll> &tree, int i) const {
        ll total = 0;
        for (; i > 0; i -= i & -i) total += tree[i];
        return total;
    }

    void update(int l, int r, ll x) { // a[l..r] += x
        if (l > r) return;
        touch(mul, l, x);
        touch(mul, r + 1, -x);
        touch(add, l, x * (l - 1));
        touch(add, r + 1, -x * r);
    }

    void update(int i, ll x) { update(i, i, x); } // a[i] += x

    ll prefix(int i) const { // sum over a[1..i]
        if (i <= 0) return 0;
        return gather(mul, i) * i - gather(add, i);
    }

    ll query(int l, int r) const { return l > r ? 0 : prefix(r) - prefix(l - 1); } // sum over a[l..r]

    ll get(int i) const { return query(i, i); } // a[i]

    void set(int i, ll value) { update(i, i, value - get(i)); } // a[i] = value
};

// Standard problem: an array with two operations - add x to a range, or report a range sum
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (ll &x : a) cin >> x;
    Fenwick_Tree_Range bit(a);
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll x;
            cin >> x;
            bit.update(l, r, x);
        } else
            cout << bit.query(l, r) << '\n';
    }
}
