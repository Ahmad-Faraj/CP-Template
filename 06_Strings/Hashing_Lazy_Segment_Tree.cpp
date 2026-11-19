// Hash_LazySegmentTree: double-mod hash of any substring while letters are assigned over ranges.
// Use when: "set s[l..r] to letter c" or "s[i] = c" mixed with "is s[a..b] equal to s[c..d]",
// palindrome / periodicity checks on a string that changes.
// Usage: Hash_LazySegmentTree<> st(n, s); st.update(l, r, 'c'); st.update(i, 'c');
//        st.equal(a, b, c, d); st.query(l, r);
// Handles: range assign, point assign, range hash, range equality.
// Time: build O(n) | update O(log n) | query O(log n)
// Indexing: 1-based inclusive ranges; s is 0-based (Base = 1: s is 1-based, s[0] is a dummy)
// Note: lowercase 'a'..'z' only (value c - 'a' + 1). query() is little-endian (s[l] has weight
// p^0), so compare it only with this tree's own queries, not with other hashing templates.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = int, bool Base = 0> struct Hash_LazySegmentTree {
    static constexpr ll M[2] = {1000000007, 1000000009}, P[2] = {31, 37};
    static constexpr ll INVP1[2] = {233333335, 972222231}; // (P - 1)^-1 mod M

    string s;
    int n, size;
    vector<ll> pw[2];
    vector<T> lazy;
    vector<pair<T, T>> tree;

    struct Item {
        T len;
        pair<T, T> ans;
        Item() : len(0), ans({0, 0}) {}
        Item(pair<T, T> ans, T len) : len(len), ans(ans) {}
    };

    explicit Hash_LazySegmentTree(int n, const string &s) : s(s), n(n) {
        for (int k = 0; k < 2; k++) {
            pw[k].assign(n + 1, 1);
            for (int i = 1; i <= n; i++) pw[k][i] = pw[k][i - 1] * P[k] % M[k];
        }
        size = 1;
        while (size < n) size *= 2;
        lazy.assign(2 * size, -1);
        tree.assign(2 * size, {0, 0});
        if (n > 0) build(1, 1, n);
    }

    pair<T, T> merge(const pair<T, T> &a, const pair<T, T> &b, T len) {
        return {(a.first + b.first * pw[0][len]) % M[0], (a.second + b.second * pw[1][len]) % M[1]};
    }

    pair<T, T> lazy_process(T val, T len) {
        T h1 = val * ((pw[0][len] - 1 + M[0]) % M[0]) % M[0] * INVP1[0] % M[0];
        T h2 = val * ((pw[1][len] - 1 + M[1]) % M[1]) % M[1] * INVP1[1] % M[1];
        return {h1, h2};
    }

    void propagate(int idx, int l, int r) {
        if (lazy[idx] == -1) return;
        tree[idx] = lazy_process(lazy[idx], r - l + 1);
        if (l != r) lazy[2 * idx] = lazy[2 * idx + 1] = lazy[idx];
        lazy[idx] = -1;
    }

    void build(int idx, int l, int r) {
        if (l == r) {
            T v = s[l - !Base] - 'a' + 1;
            return void(tree[idx] = {v, v});
        }
        int m = (l + r) / 2;
        build(2 * idx, l, m);
        build(2 * idx + 1, m + 1, r);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1], m - l + 1);
    }

    void update(int idx, int l, int r, int L, int R, T val) {
        propagate(idx, l, r);
        if (l > R or r < L) return;
        if (l >= L and r <= R) {
            lazy[idx] = val;
            propagate(idx, l, r);
            return;
        }
        int m = (l + r) / 2;
        update(2 * idx, l, m, L, R, val);
        update(2 * idx + 1, m + 1, r, L, R, val);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1], m - l + 1);
    }

    Item query(int idx, int l, int r, int L, int R) {
        propagate(idx, l, r);
        if (l > R or r < L) return Item();
        if (l >= L and r <= R) return Item(tree[idx], r - l + 1);
        int m = (l + r) / 2;
        Item left = query(2 * idx, l, m, L, R);
        Item right = query(2 * idx + 1, m + 1, r, L, R);
        return Item(merge(left.ans, right.ans, left.len), left.len + right.len);
    }

    void update(int l, int r, char c) { update(1, 1, n, l, r, c - 'a' + 1); } // s[l..r] = c
    void update(int i, char c) { update(i, i, c); }                           // s[i] = c
    pair<T, T> query(int l, int r) { return query(1, 1, n, l, r).ans; }       // hash of s[l..r]

    bool equal(int l1, int r1, int l2, int r2) { // is s[l1..r1] == s[l2..r2]
        if (r1 - l1 != r2 - l2) return false;
        return query(l1, r1) == query(l2, r2);
    }
};
