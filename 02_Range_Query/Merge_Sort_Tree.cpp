// Merge Sort Tree: counts the elements of a[l..r] that are strictly greater than k, online.
// Use when: "how many values in [l, r] exceed k" (online, so no offline sort + BIT).
// Usage: Merge_Sort_Tree st(n, a); st.query(l, r, k); (a is a 0-based vector)
// Handles: duplicates, negatives, any k, empty ranges (l > r gives 0).
// Not: frequent updates - update() re-merges whole nodes and costs O(n).
// Time: build O(n log n) | query O(log^2 n) | update O(n) | memory O(n log n)
// Indexing: 1-based positions in query/update; the vector passed to build() is 0-based
// Note: count <= k is (r - l + 1) - query(l, r, k); count < k is (r - l + 1) - query(l, r, k - 1).

#include <bits/stdc++.h>
using namespace std;

struct Merge_Sort_Tree {
    struct Node {
        vector<int> vec;
        Node(const vector<int> &V = vector<int>()) : vec(V) {}
        Node &operator=(const int rhs) {
            vec = vector<int>(1, rhs);
            return *this;
        }
    };

    int size;
    vector<Node> tree;

    Merge_Sort_Tree(int n = 0) {
        size = 1;
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size);
    }

    Merge_Sort_Tree(int n, const vector<int> &nums) : Merge_Sort_Tree(n) { build(nums); }

    Node operation(const Node &a, const Node &b) {
        Node res;
        res.vec.reserve(a.vec.size() + b.vec.size());
        merge(a.vec.begin(), a.vec.end(), b.vec.begin(), b.vec.end(), back_inserter(res.vec));
        return res;
    }

    void build(const vector<int> &nums, int idx, int lx, int rx) {
        if (lx > (int)nums.size()) return;
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

    void update(int index, int v) { update(index, v, 1, 1, size); } // a[index] = v, O(n)

    int query(int l, int r, int k, int idx, int lx, int rx) {
        if (lx > r || l > rx) return 0;
        if (lx >= l && rx <= r) {
            auto &v = tree[idx].vec;
            return (int)(v.end() - upper_bound(v.begin(), v.end(), k));
        }
        int mx = (lx + rx) / 2;
        return query(l, r, k, 2 * idx, lx, mx) + query(l, r, k, 2 * idx + 1, mx + 1, rx);
    }

    int query(int l, int r, int k) { return query(l, r, k, 1, 1, size); } // #{a[l..r] > k}
};
