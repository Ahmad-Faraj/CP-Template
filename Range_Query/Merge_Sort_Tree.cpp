#include <bits/stdc++.h>
#define ll long long
#define sz(x) (int)(x.size())
using namespace std;

// 1-Based indexing
// build: nlog(n) | query/update: log^2(n)
struct Segment_Tree {

    struct Node {
        vector<int> vec;
        Node(const vector<int> &V = vector<int>()) : vec(V) {}
        Node operator=(const int rhs) {
            vec = vector<int>(1, rhs);
            return *this;
        }
    };

    int size;
    Node DEFAULT;
    vector<Node> tree;

    Segment_Tree(int n = 0) {
        size = 1, DEFAULT = vector<int>();
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
    }

    Segment_Tree(int n, const vector<int> &nums) {
        size = 1, DEFAULT = vector<int>();
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
        build(nums);
    }

    // Main operation to do
    Node operation(const Node &a, const Node &b) {
        int i = 0, j = 0;
        Node res;
        while (i < sz(a.vec) && j < sz(b.vec)) {
            if (a.vec[i] < b.vec[j]) {
                res.vec.push_back(a.vec[i]);
                i++;
            } else {
                res.vec.push_back(b.vec[j]);
                j++;
            }
        }
        while (i < sz(a.vec)) {
            res.vec.push_back(a.vec[i]);
            i++;
        }
        while (j < sz(b.vec)) {
            res.vec.push_back(b.vec[j]);
            j++;
        }
        return res;
    }

    // nums should be 0-based indexing
    void build(const vector<int> &nums, int idx, int lx, int rx) {
        if (lx > sz(nums)) return;
        if (rx == lx)
            tree[idx] = nums[lx - 1];
        else {
            int mx = (rx + lx) / 2;
            build(nums, 2 * idx, lx, mx);
            build(nums, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void build(const vector<int> &nums) { build(nums, 1, 1, size); }

    void update(int index, int v, int idx, int lx, int rx) {
        if (rx == lx)
            tree[idx] = v;
        else {
            int mx = (rx + lx) / 2;
            if (index <= mx)
                update(index, v, 2 * idx, lx, mx);
            else
                update(index, v, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void update(const int index, const int v) { update(index, v, 1, 1, size); }

    int query(int l, int r, int k, int idx, int lx, int rx) {
        if (lx > r || l > rx) return 0;
        if (lx >= l && rx <= r) {
            auto it = tree[idx].vec.end() - upper_bound(tree[idx].vec.begin(), tree[idx].vec.end(), k);
            return it;
        }
        int mx = (lx + rx) / 2;
        return query(l, r, k, 2 * idx, lx, mx) + query(l, r, k, 2 * idx + 1, mx + 1, rx);
    }

    int query(const int l, const int r, int k) { return query(l, r, k, 1, 1, size); }
};

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &i : a) cin >> i;
    Segment_Tree st(n, a);
    int q;
    cin >> q;
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << st.query(l, r, k) << "\n";
    }
}