// Segment Tree Lazy Persistent: range add, range sum, and every old version stays queryable.
// Use when: "add v on [l, r] creating a new version; sum on [l, r] of version k; go back to k"
//           (SPOJ TTM, "To the moon").
// Usage: PersistentSegTree<ll> t(n, a);  (version 0 = a)
//        t.update(ver, l, r, v) -> new newest version; t.query(ver, l, r); t.roots.resize(k + 1)
// Time: build O(n) | update, query O(log n); each update or query may add O(log n) nodes
// Indexing: 1-based positions, a[i - 1] is position i; versions 0-based in creation order
// Note: n >= 1. Use T = ll: lazy * length overflows int. Going back = roots.resize(k + 1).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct PersistentSegTree {
    struct Node {
        T val, lazy;
        Node *l, *r;
        Node() : val(0), lazy(0), l(this), r(this) {}
        Node(T v) : val(v), lazy(0), l(EMPTY), r(EMPTY) {}
        Node(const Node &o) : val(o.val), lazy(o.lazy), l(o.l), r(o.r) {}
    };

    static Node *EMPTY;

    int N;
    vector<Node *> roots;

    PersistentSegTree(int n, const vector<T> &nums) : N(n) {
        if (EMPTY == nullptr) {
            EMPTY = new Node();
            EMPTY->l = EMPTY->r = EMPTY;
        }
        roots.push_back(build(1, N, nums));
    }

    Node *build(int L, int R, const vector<T> &nums) {
        if (L == R) return new Node(nums[L - 1]);
        int M = (L + R) >> 1;
        Node *node = new Node();
        node->l = build(L, M, nums);
        node->r = build(M + 1, R, nums);
        node->val = node->l->val + node->r->val;
        return node;
    }

    void push(Node *node, int L, int R) {
        if (node->lazy && L < R) {
            int M = (L + R) >> 1;
            node->l = new Node(*node->l);
            node->r = new Node(*node->r);
            node->l->lazy += node->lazy;
            node->l->val += node->lazy * (T)(M - L + 1);
            node->r->lazy += node->lazy;
            node->r->val += node->lazy * (T)(R - M);
            node->lazy = 0;
        }
    }

    Node *update(Node *cur, int L, int R, int i, int j, T v) {
        if (j < L || R < i) return cur;
        Node *node = new Node(*cur);
        if (i <= L && R <= j) {
            node->lazy += v;
            node->val += v * (T)(R - L + 1);
            return node;
        }
        push(node, L, R);
        int M = (L + R) >> 1;
        node->l = update(node->l, L, M, i, j, v);
        node->r = update(node->r, M + 1, R, i, j, v);
        node->val = node->l->val + node->r->val;
        return node;
    }

    void update(int ver, int l, int r, T v) { roots.push_back(update(roots[ver], 1, N, l, r, v)); }

    T query(Node *node, int L, int R, int i, int j) {
        if (j < L || R < i) return 0;
        if (i <= L && R <= j) return node->val;
        push(node, L, R);
        int M = (L + R) >> 1;
        return query(node->l, L, M, i, j) + query(node->r, M + 1, R, i, j);
    }

    T query(int ver, int l, int r) { return query(roots[ver], 1, N, l, r); }
};

template <typename T> typename PersistentSegTree<T>::Node *PersistentSegTree<T>::EMPTY = nullptr;
