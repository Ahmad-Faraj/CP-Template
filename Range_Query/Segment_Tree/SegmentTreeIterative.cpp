/*
    [1] Definition
    Iterative data structure for range queries and point updates on an array.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N)

    [3] Notes
    Uses 0-based indexing for queries.
    Iterative implementation (non-recursive).
    Default value (0) is the identity for sum queries.
    Tree size is rounded up to the nearest power of 2.
*/

#include "../../core.h"

// 0-based indexing
class Segment_Tree {
   public:
    int Size, DEFAULT = 0;
    vector<int> tree;

      int merge(int l, int r) { return (l + r); }

    // Initialize tree size
    void init(int n) {
        Size = 1;
        while (Size < n) Size <<= 1;

        tree.assign(Size * 2, DEFAULT);
    }

    // Update value at index
    void update(int idx, int val) {
        idx += Size;
        tree[idx] = val;

        for (idx >>= 1; idx >= 1; idx >>= 1) tree[idx] = merge(tree[idx << 1], tree[idx << 1 | 1]);
    }

    // Query range [l, r]
    int query(int l, int r) {
        l += Size, r += Size;
        int ans = DEFAULT;

        while (l <= r) {
            if (l & 1) ans = merge(ans, tree[l++]);
            if (!(r & 1)) ans = merge(ans, tree[r--]);
            l >>= 1, r >>= 1;
        }

        return ans;
    }

    Segment_Tree(int n = 1e5) { init(n); }
};