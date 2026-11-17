// Wavelet Tree: k-th smallest, count <= k, count == k and sum of values <= k on a static range.
// Use when: "k-th smallest in a[l..r]", "how many in [l, r] are <= x", median of a subarray.
// Usage: wavelet_tree t; t.init(a + 1, a + n + 1, -1e9, 1e9); t.kth(l, r, k); t.LTE(l, r, x);
// Handles: negative values, duplicates, any value range [lo, hi] passed to init(). Static only.
// Time: build O(n log V) | kth / LTE / count / sum O(log V), V = hi - lo + 1
// Indexing: 1-based positions: l, r index into [from, to) as from[0] = position 1
// Note: init() REORDERS the input array (copy it first if needed). Every value must be in [lo, hi].
// Note: kth needs 1 <= k <= r - l + 1. "count >= x" = (r - l + 1) - LTE(l, r, x - 1).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct wavelet_tree {
    int lo = 1, hi = 0;
    wavelet_tree *l = nullptr, *r = nullptr;
    vector<int> b; // b[i] = how many of the first i elements go to the left child (<= mid)
    vector<ll> c;  // c[i] = sum of the first i elements

    void init(int *from, int *to, int x, int y) {
        lo = x, hi = y;
        if (from >= to) return;
        int mid = (int)(((ll)lo + hi) >> 1); // floor, no overflow
        auto f = [mid](int x) { return x <= mid; };
        int n = (int)(to - from);
        b.assign(n + 1, 0), c.assign(n + 1, 0);
        for (int i = 0; i < n; i++) b[i + 1] = b[i] + f(from[i]), c[i + 1] = c[i] + from[i];
        if (hi == lo) return;
        auto pivot = stable_partition(from, to, f);
        l = new wavelet_tree();
        l->init(from, pivot, lo, mid);
        r = new wavelet_tree();
        r->init(pivot, to, mid + 1, hi);
    }
    // kth smallest element in [l, r]; for [1,2,1,3,5] the 2nd smallest is 1, the 3rd is 2
    int kth(int l, int r, int k) {
        if (l > r) return 0;
        if (lo == hi) return lo;
        int inLeft = b[r] - b[l - 1], lb = b[l - 1], rb = b[r];
        if (k <= inLeft) return this->l->kth(lb + 1, rb, k);
        return this->r->kth(l - lb, r - rb, k - inLeft);
    }
    // count of numbers in [l, r] less than or equal to k
    int LTE(int l, int r, int k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return r - l + 1;
        int lb = b[l - 1], rb = b[r];
        return this->l->LTE(lb + 1, rb, k) + this->r->LTE(l - lb, r - rb, k);
    }
    // count of numbers in [l, r] equal to k
    int count(int l, int r, int k) {
        if (l > r || k < lo || k > hi) return 0;
        if (lo == hi) return r - l + 1;
        int lb = b[l - 1], rb = b[r];
        int mid = (int)(((ll)lo + hi) >> 1);
        if (k <= mid) return this->l->count(lb + 1, rb, k);
        return this->r->count(l - lb, r - rb, k);
    }
    // sum of numbers in [l, r] less than or equal to k
    ll sum(int l, int r, int k) {
        if (l > r || k < lo) return 0;
        if (hi <= k) return c[r] - c[l - 1];
        int lb = b[l - 1], rb = b[r];
        return this->l->sum(lb + 1, rb, k) + this->r->sum(l - lb, r - rb, k);
    }
    ~wavelet_tree() { delete l, delete r; }
};
