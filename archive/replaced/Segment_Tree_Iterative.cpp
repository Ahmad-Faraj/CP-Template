#include <bits/stdc++.h>
using namespace std;
#define ll long long

// 0-based indexing
class Segtree {
    int sz;
    vector<ll> seg;
    ll none = 0;
    ll merge(ll a, ll b) { return a + b; }

    void init(int n) {
        sz = 1;
        while (sz < n) sz <<= 1;
        seg.assign(sz << 1, none);
    }

    void update(int idx, ll val) {
        idx += sz;
        seg[idx] = val;
        for (idx >>= 1; idx >= 1; idx >>= 1) seg[idx] = merge(seg[idx << 1], seg[idx << 1 | 1]);
    }

    ll query(int l, int r) {
        l += sz, r += sz;
        ll ret = none;
        while (l <= r) {
            if (l & 1) ret = merge(ret, seg[l++]);
            if (!(r & 1)) ret = merge(ret, seg[r--]);
            l >>= 1, r >>= 1;
        }
        return ret;
    }

    Segtree(int n) { init(n); }
};
