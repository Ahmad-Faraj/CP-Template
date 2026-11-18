// Segment Tree: range query over any associative merge, with point assignment (recursive).
// Use when: "range sum/min/max/gcd on [l, r]" with point updates; Node can carry several fields.
// Usage: SegTree st; st.init(n); st.build(a); st.update(i, v); st.query(l, r);
// Handles: any associative merge with an identity, non-commutative too (merges left to right).
// Not: range updates - see Segment_Tree_Lazy.
// Time: init O(n) | build O(n) | update O(log n) | query O(log n)
// Indexing: 1-based positions, query range inclusive; build() reads a[0] into position 1
// Note: change Node, merge() and Default together - Default must be the identity of merge().
// build() may get fewer than n values (call it right after init: it never clears the other leaves).
// query(l, r) with l > r returns Default.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree {
    struct Node {
        ll val;
        Node(ll _val = 0) { val = _val; }
        Node operator=(ll _val) {
            val = _val;
            return *this;
        }
    };

    int size;
    vector<Node> tree;
    Node Default;

    void init(int n) {
        size = 1;
        while (size < n) size *= 2;
        Default = 0;
        tree = vector<Node>(2 * size + 1, Default);
    }

    Node merge(Node &a, Node &b) {
        Node res = a.val + b.val;
        return res;
    }

    void build(const vector<ll> &nums, int idx, int lx, int rx) {
        if (lx == rx) {
            if (lx <= (int)nums.size()) tree[idx] = nums[lx - 1];
            return;
        }
        int mx = (lx + rx) / 2;
        build(nums, 2 * idx, lx, mx);
        build(nums, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }

    void build(const vector<ll> &nums) { build(nums, 1, 1, size); }

    void update(int i, ll v, int idx, int lx, int rx) {
        if (lx == rx) {
            tree[idx] = v;
            return;
        }
        int mx = (lx + rx) / 2;
        if (i <= mx)
            update(i, v, 2 * idx, lx, mx);
        else
            update(i, v, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }

    void update(int i, ll v) { update(i, v, 1, 1, size); } // a[i] = v

    Node query(int l, int r, int idx, int lx, int rx) {
        if (lx > r || rx < l) return Default;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    }

    ll query(int l, int r) { return query(l, r, 1, 1, size).val; } // merge of a[l .. r]
};
