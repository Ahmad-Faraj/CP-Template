// Persistent Lazy Segment Tree: range add and range sum on any version; each add makes a version.
// Use when: "add v on [l, r]; ask a sum as it was k updates ago", or roll back to version k.
// Usage: PersistentSegTree<ll> pst(n, a); pst.update(pst.cur(), l, r, v); pst.query(ver, l, r);
//        pst.revert(ver);
// Handles: branching (update any old version), rollback, negative values, query l > r (gives 0).
// Time: build O(n) | update O(log n) | query O(log n) | memory O(n + q log n) nodes, never freed
// Indexing: 1-based positions, ranges inclusive; 0-based versions, the constructor builds version 0
// Note: query() pushes lazy and allocates (~4 log n nodes) - shared versions still read correctly.
// T = int overflows quickly on sums; use ll. n >= 1.

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct PersistentSegTree {
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

    PersistentSegTree(int n, const vector<T> &nums) : N(n) { // nums[0] goes to position 1
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
            node->l->val += node->lazy * (M - L + 1);
            node->r->lazy += node->lazy;
            node->r->val += node->lazy * (R - M);
            node->lazy = 0;
        }
    }

    Node *update(Node *cur, int L, int R, int i, int j, T v) {
        if (j < L || R < i) return cur;
        Node *node = new Node(*cur);
        if (i <= L && R <= j) {
            node->lazy += v;
            node->val += v * (R - L + 1);
            return node;
        }
        push(node, L, R);
        int M = (L + R) >> 1;
        node->l = update(node->l, L, M, i, j, v);
        node->r = update(node->r, M + 1, R, i, j, v);
        node->val = node->l->val + node->r->val;
        return node;
    }

    // new version = version ver with a[l .. r] += v
    void update(int ver, int l, int r, T v) { roots.push_back(update(roots[ver], 1, N, l, r, v)); }

    T query(Node *node, int L, int R, int i, int j) {
        if (j < L || R < i) return 0;
        if (i <= L && R <= j) return node->val;
        push(node, L, R);
        int M = (L + R) >> 1;
        return query(node->l, L, M, i, j) + query(node->r, M + 1, R, i, j);
    }

    T query(int ver, int l, int r) { return query(roots[ver], 1, N, l, r); } // sum a[l .. r] at ver

    int cur() { return (int)roots.size() - 1; }     // newest version
    void revert(int ver) { roots.resize(ver + 1); } // drop every version after ver
};

template <typename T> typename PersistentSegTree<T>::Node *PersistentSegTree<T>::EMPTY = nullptr;
