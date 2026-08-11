/*
    [1] Definition
    Merge Sort Tree (Segment Tree storing a sorted vector at each node).
    Supports counting elements strictly greater than k in a given range [l, r].

    [2] Time & Space Complexity
    Build: O(N log N)
    Update: O(N) -- re-merging vectors along the path to root
    Query: O(log^2 N) -- binary search (upper_bound) on O(log N) segment nodes
    Space: O(N log N)

    [3] Notes
    Uses 1-based indexing for queries and updates.
    Leaves contain single-element vectors; parent nodes merge sorted child vectors.
    Query uses upper_bound to count elements strictly greater than k.
*/

#include "../../core.h"

// 1-based (query / update --> log^2(n))
struct Node {
    vector<int> vec;

    Node(const vector<int>& v = vector<int>()) : vec(v) {}

    // Convenience assignment operator to convert an integer into a single-element node
    Node operator=(const int rhs) {
        vec = vector<int>(1, rhs);
        return *this;
    }
};

class Segment_Tree {
   private:
    vector<Node> tree;  // Array representation of the segment tree
    int size;           // Next power of 2 size covering array size

    // Build the merge sort tree recursively
    void init(const vector<int>& nums, int id, int lx, int rx) {
        if (lx == rx) {
            if (lx <= sz(nums)) tree[id] = nums[lx - 1];
            return;
        }

        int mid = (lx + rx) / 2;
        init(nums, 2 * id, lx, mid);
        init(nums, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Standard two-pointer merge of two sorted child vectors
    Node merge(Node& l, Node& r) {
        int i = 0, j = 0;
        Node res;
        while (i < sz(l.vec) && j < sz(r.vec)) {
            if (l.vec[i] < r.vec[j])
                res.vec.push_back(l.vec[i++]);
            else
                res.vec.push_back(r.vec[j++]);
        }

        while (i < sz(l.vec)) res.vec.push_back(l.vec[i++]);
        while (j < sz(r.vec)) res.vec.push_back(r.vec[j++]);

        return res;
    }

    // Point update: set element at index idx to v and re-merge vectors along path
    void set(const int idx, const int v, int id, int lx, int rx) {
        if (lx == rx) {
            tree[id] = v;
            return;
        }

        int mid = (lx + rx) / 2;
        if (idx <= mid)
            set(idx, v, 2 * id, lx, mid);
        else
            set(idx, v, 2 * id + 1, mid + 1, rx);

        tree[id] = merge(tree[2 * id], tree[2 * id + 1]);
    }

    // Range query: count number of elements strictly greater than k in range [l, r]
    int query(const int l, const int r, const int k, int id, int lx, int rx) {
        if (r < lx || l > rx) return 0;
        if (lx >= l && rx <= r) {
            auto it = tree[id].vec.end() - upper_bound(all(tree[id].vec), k);
            return it;
        }

        int mid = (lx + rx) / 2;

        int ln = query(l, r, k, 2 * id, lx, mid);
        int rn = query(l, r, k, 2 * id + 1, mid + 1, rx);

        return ln + rn;
    }

   public:
    // Initialize segment tree size padded to the nearest power of 2
    Segment_Tree(int n = 0) {
        size = 1;
        while (size < n) size <<= 1;
        tree = vector<Node>(2 * size, Node());
    }

    // Build tree using initial array values
    void init(const vector<int>& nums) { init(nums, 1, 1, size); }

    // Update value at index idx
    void update(const int idx, int val) { set(idx, val, 1, 1, size); }

    // Count elements strictly greater than k in range [l, r]
    int query(const int l, const int r, const int k) { return query(l, r, k, 1, 1, size); }
};