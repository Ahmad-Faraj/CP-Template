// Persistent Segment Tree: point assign and range sum, keeping every old version.
// Use when: "answer on the array as it was after the k-th update", k-th smallest in a range
//           (build counts over values, query version r minus version l-1).
// Usage: PST<ll> t(1, n); t.build(a);   (version 1; version 0 is all zeros)
//        t.update(i, v) -> new version; t.query(l, r, ver); t.get(ver, i); t.copy(ver);
// Time: build O(n) | update, query O(log(Rx - Lx)); each update adds O(log) nodes
// Indexing: positions are coordinates in [Lx, Rx] (can be -1e9..1e9 without build);
//           versions: 0 = empty, then one per build / update / copy, in order
// Note: build(a) needs Lx = 1 and puts a[i - 1] at position i. Nodes are never freed.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

template <typename T = ll> struct PST {
    struct Node {
        T val;
        Node *left, *right;
        Node(T v = 0) : val(v), left(this), right(this) {} // a zero node is its own children
        Node(T v, Node *l, Node *r) : val(v), left(l), right(r) {}
    };

    vector<Node *> roots = {new Node()};
    ll Lx, Rx;

    PST(ll lx = -1e9, ll rx = 1e9) : Lx(lx), Rx(rx) {}

    Node *build(const vector<T> &nums, ll l, ll r) {
        if (l == r) return new Node(l - 1 < (ll)nums.size() ? nums[l - 1] : T(0));
        ll mx = l + (r - l) / 2;
        Node *L = build(nums, l, mx), *R = build(nums, mx + 1, r);
        return new Node(L->val + R->val, L, R);
    }

    void build(const vector<T> &nums) { roots.push_back(build(nums, Lx, Rx)); }

    Node *update(Node *root, ll idx, T val, ll lx, ll rx) {
        if (idx < lx || idx > rx) return root;
        if (lx == rx) return new Node(val);
        ll mx = lx + (rx - lx) / 2;
        Node *L = update(root->left, idx, val, lx, mx);
        Node *R = update(root->right, idx, val, mx + 1, rx);
        return new Node(L->val + R->val, L, R);
    }

    void update(ll idx, T val) { roots.push_back(update(roots.back(), idx, val, Lx, Rx)); }

    // replaces version `version` in place instead of adding a new one
    void update_version(ll idx, T val, int version) {
        roots[version] = update(roots[version], idx, val, Lx, Rx);
    }

    T query(Node *root, ll l, ll r, ll lx, ll rx) {
        if (lx > r || l > rx) return 0;
        if (lx >= l && rx <= r) return root->val;
        ll mx = lx + (rx - lx) / 2;
        return query(root->left, l, r, lx, mx) + query(root->right, l, r, mx + 1, rx);
    }

    T query(ll l, ll r, int version) { return query(roots[version], l, r, Lx, Rx); }

    void copy(int version) { roots.push_back(roots[version]); } // clone a version as the newest

    T get(int version, ll idx) { return query(idx, idx, version); }
};
