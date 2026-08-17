// Segment Tree: range query over any associative merge, with point assignment.
// Use when: "range sum/min/max/gcd on [l, r]" with point updates, and the merge needs editing per problem.
// Handles: any associative merge with an identity; point assign, range query. No range updates - see the Lazy variant.
// Time: build O(n) | update O(log n) | query O(log n)
// Indexing: 1-based, ranges inclusive
// Note: change Node, merge() and Default together - Default must be the identity element of merge().

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

    ll query(int l, int r) { return query(l, r, 1, 1, size).val; } // merge over a[l .. r]
};

// Standard problem: point assignment and range sum (CSES 1648 - Dynamic Range Sum Queries)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SegTree st;
    st.init(n);
    st.build(a);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int k;
            ll u;
            cin >> k >> u;
            st.update(k, u);
        } else {
            int l, r;
            cin >> l >> r;
            cout << st.query(l, r) << '\n';
        }
    }
}
