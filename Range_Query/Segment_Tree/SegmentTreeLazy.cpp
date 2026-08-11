/*
    [1] Definition
    Data structure for range updates and range queries on an array using lazy propagation.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N)

    [3] Notes
    Uses 1-based indexing for queries.
    Supports range addition updates via lazy propagation.
    Default node value (0) is the identity for sum queries.
    Tree size is rounded up to the nearest power of 2.
*/

#include "../../core.h"

struct Node {
    int val, lazy = 0;
    bool isLazy = false;

    Node() : val(0) {}

    Node(int x) : val(x) {}

    // Apply lazy update to node
    void update(int v, int lx, int rx) {
        val += (rx - lx + 1) * v;
        lazy += v;
        isLazy = true;
    }
};

// 1-based indexing
class Segment_Tree {
   private:
    int size;
    vector<Node> tree;

    // Build tree recursively
    void init(const vector<int>& nums, int id, int lx, int rx) {
        if (lx == rx) {
            if (lx <= sz(nums)) tree[id] = Node(nums[lx - 1]);
            return;
        }

        int mid = (lx + rx) / 2;
        init(nums, 2 * id, lx, mid);
        init(nums, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    Node merge(Node& l, Node& r) { return Node(l.val + r.val); }

    // Push lazy updates to children
    void propagate(int lx, int rx, int id) {
        if (lx == rx || !tree[id].isLazy) return;

        int mid = (lx + rx) / 2;
        tree[2 * id].update(tree[id].lazy, lx, mid);
        tree[2 * id + 1].update(tree[id].lazy, mid + 1, rx);

        tree[id].lazy = tree[id].isLazy = 0;
    }

    // Update range [l, r]
    void update(const int l, const int r, const int v, int id, int lx, int rx) {
        propagate(lx, rx, id);

        if (l > rx || r < lx) return;

        if (l <= lx && r >= rx) {
            tree[id].update(v, lx, rx);
            return;
        }

        int mid = (lx + rx) / 2;
        update(l, r, v, 2 * id, lx, mid);
        update(l, r, v, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Query range [l, r]
    Node get(const int l, const int r, int id, int lx, int rx) {
        propagate(lx, rx, id);

        if (l > rx || r < lx) return Node();
        if (l <= lx && r >= rx) { return tree[id]; }

        int mid = (lx + rx) / 2;
        Node ln = get(l, r, 2 * id, lx, mid);
        Node rn = get(l, r, 2 * id + 1, mid + 1, rx);

        return merge(ln, rn);
    }

   public:
    // Initialize size to power of 2U+
    Segment_Tree(int n) {
        size = 1;
        while (size < n) size <<= 1;
        tree = vector<Node>(2 * size, Node());
    }

    // Build tree
    void init(const vector<int>& nums) { init(nums, 1, 1, size); }

    // Update range [l, r]
    void update(const int l, const int r, const int v) { update(l, r, v, 1, 1, size); }

    // Get result for range [l, r]
    int query(const int l, const int r) { return get(l, r, 1, 1, size).val; }
};