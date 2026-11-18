// Iterative Segment Tree: range query over any associative merge, with point assignment.
// Use when: "range sum/min/max on [l, r] with point updates" and you want the shortest, fastest
//           segment tree.
// Usage: SegTree st(n); st.build(a); st.update(i, v); st.query(l, r);  (edit merge and identity)
// Handles: any associative merge with an identity; point assign, range query. No lazy, no range
//          updates.
// Time: build O(n) | update O(log n) | query O(log n)
// Indexing: 0-based, query range inclusive
// Note: change merge() and none together - none must be the identity element of merge().

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    int n;
    vector<ll> seg;
    ll none = 0;
    ll merge(ll a, ll b) { return a + b; }

    SegTree(int m) {
        n = 1;
        while (n < m) n <<= 1;
        seg.assign(n << 1, none);
    }

    void build(const vector<ll> &a) { // O(n) fill, instead of n calls to update()
        for (int i = 0; i < (int)a.size(); i++) seg[n + i] = a[i];
        for (int i = n - 1; i >= 1; i--) seg[i] = merge(seg[i << 1], seg[i << 1 | 1]);
    }

    void update(int idx, ll val) { // a[idx] = val
        idx += n;
        seg[idx] = val;
        for (idx >>= 1; idx >= 1; idx >>= 1) seg[idx] = merge(seg[idx << 1], seg[idx << 1 | 1]);
    }

    // merge over a[l .. r], in left-to-right order (KACTL SegmentTree.h loop)
    ll query(int l, int r) {
        ll ra = none, rb = none;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) ra = merge(ra, seg[l++]);
            if (r & 1) rb = merge(seg[--r], rb);
        }
        return merge(ra, rb);
    }
};
