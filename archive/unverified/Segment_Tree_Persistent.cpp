// Persistent Segment Tree: point assign makes a new version; range sum on any version.
// Use when: "the array as it was after update k", or counting values in [x, y] inside a[l..r]:
// version i = frequencies of a[1..i]; answer = query(x, y, r) - query(x, y, l - 1).
// Usage: PST<ll> pst(n, 1, n); pst.build(a); pst.update(i, v); pst.query(l, r, ver);
//        pst.get(ver, i);
// Handles: version 0 = all zeros (update() works without build()), huge or negative ranges
// [Lx, Rx], cloning a version with copy(), editing one version in place with update_version().
// Time: build O(n) | update O(log C) | query O(log C), C = Rx - Lx + 1; memory O(n + q log C)
// Indexing: positions in [Lx, Rx] inclusive; build() needs Lx = 1, Rx = a.size(), makes version 1.
// Versions: 0 = empty; update() appends version versions() + 1 built from the newest one.
// Note: T is both the value type and the coordinate type. Nodes are never freed.

#include <bits/stdc++.h>
using namespace std;

template <typename T = int> struct PST {
    struct Node {
        T val;
        Node *left, *right;
        Node(T _val = 0) : val(_val), left(this), right(this) {} // self-loop = all-zero subtree
        Node(T _val, Node *l, Node *r) : val(_val), left(l), right(r) {}
    };

    vector<Node *> roots = {new Node()};
    int N;
    T Lx, Rx;

    PST(int n = 0, T lx = -1e9, T rx = 1e9) : N(n), Lx(lx), Rx(rx) {}

    T merge(T a, T b) { return a + b; }
    // floor((lx + rx) / 2) in 64 bits: lx + (rx - lx) / 2 overflows int on [-1e9, 1e9]
    static T mid(T lx, T rx) { return (T)(((long long)lx + rx) >> 1); }

    Node *build(const vector<T> &nums, T l, T r) {
        if (l == r) return new Node(nums[l - 1]);
        T mx = mid(l, r);
        Node *L = build(nums, l, mx);
        Node *R = build(nums, mx + 1, r);
        return new Node(merge(L->val, R->val), L, R);
    }

    void build(const vector<T> &nums) { roots.push_back(build(nums, Lx, Rx)); }

    Node *update(Node *root, T idx, T val, T lx, T rx) {
        if (idx < lx || idx > rx) return root;
        if (lx == rx) return new Node(val);
        T mx = mid(lx, rx);
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(merge(L->val, R->val), L, R);
    }

    // new version = newest version with a[idx] = val
    void update(T idx, T val) { roots.push_back(update(roots.back(), idx, val, Lx, Rx)); }

    // a[idx] = val inside that version itself (no new version; later versions are unaffected)
    void update_version(T idx, T val, int version) {
        roots[version] = update(roots[version], idx, val, Lx, Rx);
    }

    T query(Node *root, T l, T r, T lx, T rx) {
        if (lx > r || l > rx) return 0;
        if (lx >= l && rx <= r) return root->val;
        T mx = mid(lx, rx);
        return merge(query(root->left, l, r, lx, mx), query(root->right, l, r, mx + 1, rx));
    }

    T query(T l, T r, int version) { return query(roots[version], l, r, Lx, Rx); } // sum a[l..r]

    void copy(int version) { roots.push_back(roots[version]); } // clone a version onto the end

    T get(int version, T idx) { return query(idx, idx, version); } // a[idx] in that version

    int versions() { return (int)roots.size() - 1; } // newest version number
};
