#include <bits/stdc++.h>
using namespace std;

// 1-based indexing
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

    PersistentSegTree(int n, vector<int> nums) : N(n) {
        if (EMPTY == nullptr) {
            EMPTY = new Node();
            EMPTY->l = EMPTY->r = EMPTY;
        }
        roots.push_back(build(1, N, nums));
    }

    Node *build(int L, int R, vector<int> &nums) {
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

void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> arr(n);
    for (auto &x : arr) cin >> x;

    PersistentSegTree<> pst(n, arr);

    while (q--) {
        char op;
        cin >> op;
        if (op == 'C') { // add and create and new version
            int l, r, v;
            cin >> l >> r >> v;
            pst.update((int)pst.roots.size() - 1, l, r, v);
        } else if (op == 'Q') { // Query [l,r] of current version
            int l, r;
            cin >> l >> r;
            cout << pst.query((int)pst.roots.size() - 1, l, r) << endl;
        } else if (op == 'H') { // Query [l,r] of ver version
            int l, r, ver;
            cin >> l >> r >> ver;
            cout << pst.query(ver, l, r) << endl;
        } else if (op == 'B') { // Back to ver version
            int ver;
            cin >> ver;
            pst.roots.resize(ver + 1);
        }
    }
}