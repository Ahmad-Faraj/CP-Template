// Implicit Treap: a sequence keyed by position - insert/erase anywhere, reverse, affine update and
//                 sum on any range.
// Use when: an array that changes length (insert/erase in the middle), range reverse, cut-and-
//           paste of blocks, plus range add/assign/affine and range sum.
// Usage: Treap_Implicit<ll> t(a); t.insert(i, x); t.erase(i); t.reverse(l, r);
//        t.update(l, r, b, c); t.query(l, r); t.get(i); t.size(); t.split(x, k); t.merge(a, b);
// Handles: update is a[i] = b * a[i] + c: add x -> (1, x), assign x -> (0, x). Empty ranges l == r.
// Time: every operation O(log n) expected
// Indexing: 0-based, ranges HALF-OPEN [l, r); insert(i, x) puts x before a[i] (i = size() appends)
// Note: T needs + * and T(int); use ModInt<998244353> for "sum mod p". Erased nodes are not
//       reused. Node 0 is the empty tree. split(x, k) = (first k elements, rest).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct Treap_Implicit {
    struct Node {
        T val, sum, mul, add;
        int l, r, sz;
        unsigned pri;
        bool rev, tag;
    };
    vector<Node> t{Node{}};
    int root = 0;
    mt19937 rng{20260};

    Treap_Implicit(const vector<T> &a = {}) {
        for (auto &x : a) root = merge(root, new_node(x));
    }

    int new_node(T v) {
        t.push_back({v, v, T(1), T(0), 0, 0, 1, (unsigned)rng(), false, false});
        return (int)t.size() - 1;
    }
    void apply(int x, T b, T c) {
        if (!x) return;
        Node &n = t[x];
        n.val = b * n.val + c, n.sum = b * n.sum + c * T(n.sz);
        n.mul = b * n.mul, n.add = b * n.add + c, n.tag = 1;
    }
    void flip(int x) {
        if (x) swap(t[x].l, t[x].r), t[x].rev ^= 1;
    }
    void push(int x) {
        Node &n = t[x];
        if (n.rev) flip(n.l), flip(n.r), n.rev = 0;
        if (!n.tag) return;
        apply(n.l, n.mul, n.add), apply(n.r, n.mul, n.add);
        n.mul = T(1), n.add = T(0), n.tag = 0;
    }
    void pull(int x) {
        Node &n = t[x];
        n.sz = t[n.l].sz + 1 + t[n.r].sz;
        n.sum = t[n.l].sum + n.val + t[n.r].sum;
    }

    pair<int, int> split(int x, int k) { // (first k elements, the rest)
        if (!x) return {0, 0};
        push(x);
        if (k <= t[t[x].l].sz) {
            auto [a, b] = split(t[x].l, k);
            t[x].l = b, pull(x);
            return {a, x};
        }
        auto [a, b] = split(t[x].r, k - t[t[x].l].sz - 1);
        t[x].r = a, pull(x);
        return {x, b};
    }
    int merge(int a, int b) { // every element of a goes before every element of b
        if (!a || !b) return a ? a : b;
        if (t[a].pri > t[b].pri) {
            push(a), t[a].r = merge(t[a].r, b), pull(a);
            return a;
        }
        push(b), t[b].l = merge(a, t[b].l), pull(b);
        return b;
    }

    template <class F> void range(int l, int r, F f) { // f(subtree holding [l, r))
        auto [a, bc] = split(root, l);
        auto [b, c] = split(bc, r - l);
        f(b);
        root = merge(a, merge(b, c));
    }

    void insert(int i, T v) {
        auto [a, b] = split(root, i);
        int m = new_node(v);
        root = merge(merge(a, m), b);
    }
    void erase(int i) {
        auto [a, bc] = split(root, i);
        root = merge(a, split(bc, 1).second);
    }
    void reverse(int l, int r) { range(l, r, [&](int x) { flip(x); }); }
    void update(int l, int r, T b, T c) { range(l, r, [&](int x) { apply(x, b, c); }); }
    T query(int l, int r) { // sum of [l, r)
        T s = T(0);
        range(l, r, [&](int x) { s = t[x].sum; });
        return s;
    }
    T get(int i) { return query(i, i + 1); }
    int size() { return t[root].sz; }
};
