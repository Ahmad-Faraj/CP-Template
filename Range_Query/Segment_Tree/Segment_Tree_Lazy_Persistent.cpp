// Persistent Lazy Segment Tree: range add and range sum on any past version.
// Use when: "range add, then read the array as it was k updates ago", or rolling the whole array back to a version.
// Handles: range add creating a version, range sum on any version, rollback via revert(). Nodes are never freed.
// Time: build O(n) | update O(log n) | query O(log n) | memory O((n + q) log n)
// Indexing: 1-based positions, 0-based versions; the constructor builds version 0
// Note: query() pushes lazy and therefore allocates - it is not const. Shared versions still read correctly.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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

    PersistentSegTree(int n, vector<T> nums) : N(n) {
        if (EMPTY == nullptr) {
            EMPTY = new Node();
            EMPTY->l = EMPTY->r = EMPTY;
        }
        roots.push_back(build(1, N, nums));
    }

    Node *build(int L, int R, vector<T> &nums) {
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

    T query(Node *node, int L, int R, int i, int j) {
        if (j < L || R < i) return 0;
        if (i <= L && R <= j) return node->val;
        push(node, L, R);
        int M = (L + R) >> 1;
        return query(node->l, L, M, i, j) + query(node->r, M + 1, R, i, j);
    }

    int cur() { return (int)roots.size() - 1; } // index of the newest version

    void update(int ver, int l, int r, T v) { roots.push_back(update(roots[ver], 1, N, l, r, v)); } // a[l..r] += v

    T query(int ver, int l, int r) { return query(roots[ver], 1, N, l, r); } // sum of a[l .. r] as of ver

    void revert(int ver) { roots.resize(ver + 1); } // discard every version after ver
};

template <typename T> typename PersistentSegTree<T>::Node *PersistentSegTree<T>::EMPTY = nullptr;

// Standard problem: C adds on a range, Q asks the current sum, H asks an old version, B rolls back to a version
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> arr(n);
    for (auto &x : arr) cin >> x;
    PersistentSegTree<> pst(n, arr);
    while (q--) {
        char op;
        cin >> op;
        if (op == 'C') {
            int l, r, v;
            cin >> l >> r >> v;
            pst.update(pst.cur(), l, r, v);
        } else if (op == 'Q') {
            int l, r;
            cin >> l >> r;
            cout << pst.query(pst.cur(), l, r) << '\n';
        } else if (op == 'H') {
            int l, r, ver;
            cin >> l >> r >> ver;
            cout << pst.query(ver, l, r) << '\n';
        } else {
            int ver;
            cin >> ver;
            pst.revert(ver);
        }
    }
}
