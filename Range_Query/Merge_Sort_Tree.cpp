// Merge Sort Tree: counts how many elements of [l, r] are strictly greater than k, on a static array.
// Use when: "how many elements in [l, r] exceed k", or the same below k by flipping the bound in query().
// Handles: any k, duplicates, negatives. Updates cost O(n) - treat this as a static structure.
// Time: build O(n log n) | query O(log^2 n) | update O(n) | memory O(n log n)
// Indexing: 1-based queries; the array handed to build() is 0-based
// Note: query() counts strictly greater than k. If updates are frequent, rebuild instead.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

struct Merge_Sort_Tree {
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

    Merge_Sort_Tree(int n = 0) {
        size = 1, DEFAULT = vector<int>();
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
    }

    Merge_Sort_Tree(int n, const vector<int> &nums) : Merge_Sort_Tree(n) { build(nums); }

    Node operation(const Node &a, const Node &b) {
        Node res;
        res.vec.reserve(sz(a.vec) + sz(b.vec));
        merge(a.vec.begin(), a.vec.end(), b.vec.begin(), b.vec.end(), back_inserter(res.vec));
        return res;
    }

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

    void update(int index, int v) { update(index, v, 1, 1, size); } // a[index] = v, costs O(n)

    int query(int l, int r, int k, int idx, int lx, int rx) {
        if (lx > r || l > rx) return 0;
        if (lx >= l && rx <= r)
            return (int)(tree[idx].vec.end() - upper_bound(tree[idx].vec.begin(), tree[idx].vec.end(), k));
        int mx = (lx + rx) / 2;
        return query(l, r, k, 2 * idx, lx, mx) + query(l, r, k, 2 * idx + 1, mx + 1, rx);
    }

    int query(int l, int r, int k) { return query(l, r, k, 1, 1, size); } // count of a[l .. r] greater than k
};

// Standard problem: for each query report how many elements of [l, r] are greater than k
void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    for (auto &i : a) cin >> i;
    Merge_Sort_Tree st(n, a);
    int q;
    cin >> q;
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        cout << st.query(l, r, k) << '\n';
    }
}
