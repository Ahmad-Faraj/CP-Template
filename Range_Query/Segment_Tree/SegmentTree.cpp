/*
    [1] Definition
    Data structure for range queries and point updates on an array.

    [2] Time & Space Complexity
    Build: O(N)
    Update: O(log N)
    Query: O(log N)
    Space: O(N)

    [3] Notes
    Uses 1-based indexing for queries.
    Default node value (0) is the identity for sum queries.
    Tree size is rounded up to the nearest power of 2.
*/

#include "../../core.h"

// Tree node
struct Node {
    int val;

    Node() { val = 0; }  // Default value

    Node(int x) : val(x) {}

    void update(int x) { val += x; }
};

// 1-based indexing
class Segment_Tree {
   private:
    int size;
    vector<Node> tree;

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

    // Combine left and right nodes
    Node merge(Node& l, Node& r) { return Node(l.val + r.val); }

    // Update node at index
    void set(const int idx, const int v, int id, int lx, int rx) {
        if (lx == rx) {
            tree[id].val = v;
            return;
        }

        int mid = (lx + rx) / 2;
        if (idx <= mid)
            set(idx, v, 2 * id, lx, mid);
        else
            set(idx, v, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Query range [l, r]
    Node get(const int l, const int r, int id, int lx, int rx) {
        if (l > rx || r < lx) return Node();

        if (l <= lx && r >= rx) return tree[id];

        int mid = (lx + rx) / 2;

        Node ln = get(l, r, 2 * id, lx, mid);
        Node rn = get(l, r, 2 * id + 1, mid + 1, rx);

        return merge(ln, rn);
    }

   public:

    Segment_Tree(int n) {
        size = 1;
        while (size < n) size <<= 1;
        tree = vector<Node>(2 * size, Node());
    }

    // Build tree
    void init(const vector<int>& nums) { init(nums, 1, 1, size); }

    // Update value at index
    void update(const int idx, const int v) { set(idx, v, 1, 1, size); }

    // Get result for range [l, r]
    int query(const int l, const int r) { return get(l, r, 1, 1, size).val; }
};

