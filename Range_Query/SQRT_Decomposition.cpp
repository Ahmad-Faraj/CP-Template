// SQRT Decomposition: range sum with point assignment, over sqrt(n) precomputed blocks.
// Use when: the operation cannot be merged by a segment tree, but does aggregate per block.
// Handles: point assign, range sum. Swap the block aggregate for min/max/count. Slower than a segment tree.
// Time: build O(n) | update O(1) | query O(sqrt n)
// Indexing: 0-based, ranges inclusive
// Note: reach for a segment tree first - this is for operations a segment tree cannot merge.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SqrtDecomp {
    int n, block;
    vector<ll> a, blk;

    SqrtDecomp(const vector<ll> &v) : n((int)v.size()), a(v) {
        block = max(1, (int)sqrt(n));
        blk.assign(n / block + 1, 0);
        for (int i = 0; i < n; i++) blk[i / block] += a[i];
    }

    void update(int i, ll val) { // a[i] = val
        blk[i / block] += val - a[i];
        a[i] = val;
    }

    ll query(int l, int r) { // sum over a[l .. r]
        ll ret = 0;
        while (l <= r) {
            if (l % block == 0 && l + block - 1 <= r)
                ret += blk[l / block], l += block;
            else
                ret += a[l++];
        }
        return ret;
    }
};

// Standard problem: point assignment and range sum (CSES 1648 - Dynamic Range Sum Queries)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SqrtDecomp sd(a);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k;
            ll u;
            cin >> k >> u;
            sd.update(k - 1, u);
        } else {
            int l, r;
            cin >> l >> r;
            cout << sd.query(l - 1, r - 1) << '\n';
        }
    }
}
