#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define nl "\n"

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
    void build(vector<int> &nums, int idx, int lx, int rx) {
        if (lx == rx) {
            if (lx <= nums.size()) {
                tree[idx] = nums[lx - 1];
            }
            return;
        }
        int mx = (lx + rx) / 2;
        build(nums, 2 * idx, lx, mx);
        build(nums, 2 * idx + 1, mx + 1, rx);
        tree[idx] = merge(tree[2 * idx], tree[2 * idx + 1]);
    }
    void build(vector<int> &nums) { build(nums, 1, 1, size); }
    void update(int i, int v, int idx, int lx, int rx) {
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
    void update(int i, int v) { update(i, v, 1, 1, size); }
    Node query(int l, int r, int idx, int lx, int rx) {
        if (lx > r || rx < l) return Default;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        auto left = query(l, r, 2 * idx, lx, mx);
        auto right = query(l, r, 2 * idx + 1, mx + 1, rx);
        return merge(left, right);
    }
    ll query(int l, int r) { return query(l, r, 1, 1, size).val; }
};
/*
    // find k‑th c in [L,R], or –1
    int kth(int L, int R, char c, int k)
    {
        return kth_rec(L, R, c, k, 1, 1, size);
    }

    int kth_rec(int L, int R, char c, int k,
                int idx, int lx, int rx)
    {
        // no overlap
        if (lx > R || rx < L)
            return -1;

        // fully inside [L,R], maybe finish here
        if (lx >= L && rx <= R)
        {
            if (tree[idx].val < k)
                return -1;
            if (lx == rx)
                return lx;
        }

        int mid = (lx + rx) / 2;
        // count in left child ∩ [L,R]
        Node leftNode = query(L, R, 2 * idx, lx, mid);
        int leftCount = leftNode.val;

        if (leftCount >= k)
            return kth_rec(L, R, c, k, 2 * idx, lx, mid);
        else
            return kth_rec(L, R, c, k - leftCount, 2 * idx + 1, mid + 1, rx);
    }
*/
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    for (auto &i : a) cin >> i;
    SegTree st;
    st.init(n);
    st.build(a);

    while (q--) {
        int type, x, y;
        cin >> type >> x >> y;
        x++;
        if (type == 1)
            st.update(x, y);
        else
            cout << st.query(x, y) << "\n";
    }
}