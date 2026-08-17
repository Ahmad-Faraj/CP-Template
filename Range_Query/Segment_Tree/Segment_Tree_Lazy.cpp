// Lazy Segment Tree: range add and range sum, both in O(log n).
// Use when: "add v to every element of [l, r]" together with "sum of [l, r]" - updates hit whole ranges, not points.
// Handles: range add, range sum, point read via query(i, i). Swap push() and merge() for other lazy operations.
// Time: build O(n) | update O(log n) | query O(log n)
// Indexing: 1-based, ranges inclusive
// Note: push() applies pending lazy on entry, so every access must go through update()/query(), never tree[] itself.

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

    Node merge(Node &a, Node &b) { return Node(a.val + b.val); }

    void build(vector<ll> &nums, int idx, int lx, int rx) {
        if (lx == rx) {
            if (lx <= (int)nums.size()) tree[idx] = nums[lx - 1];
            return;
        }
        int mx = (lx + rx) / 2;
        build(nums, 2 * idx, lx, mx);
        build(nums, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }

    void build(vector<ll> &nums) { build(nums, 1, 1, size); }

    void push(int idx, int lx, int rx) {
        if (tree[idx].lazy == 0) return;
        tree[idx].val += (rx - lx + 1) * tree[idx].lazy;
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

    void update(int l, int r, ll v) { update(l, r, v, 1, 1, size); } // a[l .. r] += v

    Node query(int l, int r, int idx, int lx, int rx) {
        push(idx, lx, rx);
        if (rx < l || lx > r) return Default;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    }

    ll query(int l, int r) { return query(l, r, 1, 1, size).val; } // sum over a[l .. r]
};

// Standard problem: type 1 adds v on [l, r], type 2 asks the sum of [l, r]
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    Lazy_Segment_Tree st;
    st.init(n);
    st.build(a);
    while (q--) {
        int type, l, r;
        cin >> type >> l >> r;
        if (type == 1) {
            ll v;
            cin >> v;
            st.update(l, r, v);
        } else {
            cout << st.query(l, r) << '\n';
        }
    }
}
