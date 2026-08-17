// Persistent Segment Tree: range sum on any past version; every update creates a new version.
// Use when: "the array as it was after update k", k-th smallest on a range, counting distinct values offline.
// Handles: point assign creating a version, range sum on any version, cloning a version. Nodes are never freed.
// Time: build O(n) | update O(log n) | query O(log n) | memory O((n + q) log n)
// Indexing: 1-based values and 1-based versions; version 0 is empty and build() creates version 1
// Note: update() appends a version; update_version() overwrites one in place and breaks the chain after it.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = int> struct PST {
    struct Node {
        T val;
        Node *left, *right;

        Node(T _val = 0) {
            val = _val;
            left = right = this;
        }

        Node(Node *node, Node *l = new Node, Node *r = new Node) {
            val = node->val;
            left = l;
            right = r;
        }
    };

    vector<Node *> roots = {new Node()};
    int N;
    T Lx, Rx;

    PST(int n = 0, T lx = -1e9, T rx = 1e9) : N(n), Lx(lx), Rx(rx) {}

    Node *merge(Node *a, Node *b) {
        Node *node = new Node();
        node->val = a->val + b->val;
        return node;
    }

    Node *build(const vector<T> &nums, T l, T r) {
        if (l == r) return new Node(nums[l - 1]);
        T mx = l + (r - l) / 2;
        Node *L = build(nums, l, mx);
        Node *R = build(nums, mx + 1, r);
        return new Node(merge(L, R), L, R);
    }

    void build(const vector<T> &nums) { roots.push_back(build(nums, Lx, Rx)); } // creates version 1

    Node *update(Node *root, int idx, T val, T lx, T rx) {
        if (idx < lx || idx > rx) return root;
        if (lx == rx) return new Node(val);
        T mx = lx + (rx - lx) / 2;
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(merge(L, R), L, R);
    }

    void update(int idx, T val) { roots.push_back(update(roots.back(), idx, val, Lx, Rx)); } // a[idx] = val, new ver

    void update_version(int idx, T val, int version) { roots[version] = update(roots[version], idx, val, Lx, Rx); }

    Node *query(Node *root, int l, int r, T lx, T rx) {
        if (root == nullptr) return new Node();
        if (lx > r || l > rx) return new Node();
        if (lx >= l && rx <= r) return root;
        T mx = lx + (rx - lx) / 2;
        Node *L = query(root->left, l, r, lx, mx);
        Node *R = query(root->right, l, r, mx + 1, rx);
        return merge(L, R);
    }

    T query(int l, int r, int version) { return query(roots[version], l, r, Lx, Rx)->val; } // sum of a[l .. r] then

    void copy(int version) { roots.push_back(roots[version]); } // clone a version onto the end

    T get(int version, int idx) { return query(idx, idx, version); } // a[idx] as of that version

    int versions() { return (int)roots.size() - 1; }
};

// Standard problem: type 1 assigns and makes a new version, type 2 asks the sum of [l, r] on version k
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    PST<int> pst(n, 1, n);
    pst.build(a);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int idx, val;
            cin >> idx >> val;
            pst.update(idx, val);
        } else {
            int k, l, r;
            cin >> k >> l >> r;
            cout << pst.query(l, r, k) << '\n';
        }
    }
}
