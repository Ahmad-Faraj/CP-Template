// Lazy Segment Tree: range add and range sum, both O(log n).
// Use when: "add v to every a[i] in [l, r]" mixed with "sum of a[l..r]".
// Usage: Lazy_Segment_Tree st; st.init(n); st.build(a); st.update(l, r, v); st.query(l, r);
// Time: init/build O(n) | update, query O(log n)
// Indexing: 1-based positions l..r inclusive; build(a) takes a 0-based vector (a[0] -> position 1)
// Note: for other operations change merge(), push() and the update at a full cover together,
//       or use Segment_Tree_Lazy_Generic.cpp (plug in any node/tag, e.g. affine, assign).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Lazy_Segment_Tree {
    struct Node {
        ll val, lazy;
        Node(ll _val = 0, ll _lazy = 0) : val(_val), lazy(_lazy) {}
        Node operator=(ll _val) {
            val = _val;
            lazy = 0;
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
        tree = vector<Node>(2 * size);
    }
    Node merge(Node &a, Node &b) { return a.val + b.val; }

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

    void push(int idx, int lx, int rx) {
        if (tree[idx].lazy == 0) return;
        tree[idx].val += (ll)(rx - lx + 1) * tree[idx].lazy;
        if (lx != rx) {
            tree[2 * idx].lazy += tree[idx].lazy;
            tree[2 * idx + 1].lazy += tree[idx].lazy;
        }
        tree[idx].lazy = 0;
    }

    void update(int l, int r, ll v, int idx, int lx, int rx) {
        push(idx, lx, rx);
        if (rx < l || lx > r) return;
        if (lx >= l && rx <= r) {
            tree[idx].lazy += v;
            push(idx, lx, rx);
            return;
        }
        int mx = (lx + rx) / 2;
        update(l, r, v, 2 * idx, lx, mx);
        update(l, r, v, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }

    void update(int l, int r, ll v) { update(l, r, v, 1, 1, size); }

    Node query(int l, int r, int idx, int lx, int rx) {
        push(idx, lx, rx);
        if (rx < l || lx > r) return Default;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    }

    ll query(int l, int r) { return query(l, r, 1, 1, size).val; }
};