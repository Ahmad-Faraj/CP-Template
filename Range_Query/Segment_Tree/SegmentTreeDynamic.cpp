/*
    [1] Definition
    Dynamic Segment Tree allocating nodes on the fly as needed.
    Ideal for range queries and point updates over an extremely large or sparse index space.

    [2] Time & Space Complexity
    Build: O(1)
    Update: O(log(Rx - Lx))
    Query: O(log(Rx - Lx))
    Space: O(Q log(Rx - Lx))

    [3] Notes
    Uses 1-based indexing (or any custom coordinate range [Lx, Rx]).
    Allocates memory dynamically; avoids upfront O(N) array allocation.
    Default value (0) is the identity for sum queries.
*/

#include "../../core.h"

// 1-Based Indexing
// Use when working with huge coordinate intervals with relatively few elements
class Dynamic_Segment_Tree {
   private:
    // Data payload stored inside each segment node
    struct Node {
        int val;

        Node(int _val = 0) : val(_val) {}

        Node operator=(int x) {
            val = x;
            return *this;
        }
    };

    // Pointer-based tree node holding child pointers and data payload
    struct TreeNode {
        Node data;
        TreeNode *left, *right;

        TreeNode() : data(), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;
    int Lx, Rx;

    // Combine left and right node payloads
    Node merge(Node& a, Node& b) {
        Node res;
        res.val = a.val + b.val;
        return res;
    }

    // Recursively update or insert node on path to idx
    void update(TreeNode*& treenode, int idx, int val, int lx, int rx) {
        if (!treenode) treenode = new TreeNode();
        if (lx == rx) {
            treenode->data.val += val;
            return;
        }
        int mx = lx + (rx - lx) / 2;
        if (idx <= mx)
            update(treenode->left, idx, val, lx, mx);
        else
            update(treenode->right, idx, val, mx + 1, rx);

        Node leftNode = treenode->left ? treenode->left->data : Node(0);
        Node rightNode = treenode->right ? treenode->right->data : Node(0);
        treenode->data = merge(leftNode, rightNode);
    }

    // Query range [l, r] recursively
    Node query(TreeNode* treenode, int l, int r, int lx, int rx) {
        if (!treenode || lx > r || rx < l) return Node();
        if (lx >= l && rx <= r) return (treenode ? treenode->data : Node(0));
        int mx = lx + (rx - lx) / 2;
        Node leftNode = query(treenode->left, l, r, lx, mx);
        Node rightNode = query(treenode->right, l, r, mx + 1, rx);
        return merge(leftNode, rightNode);
    }

   public:
    Dynamic_Segment_Tree(int lx = -1e9, int rx = 1e9) : Lx(lx), Rx(rx) { root = new TreeNode(); }

    // Point update: add val at position idx
    void update(int idx, int val) { update(root, idx, val, Lx, Rx); }

    // Range sum query in range [l, r]
    int query(int l, int r) { return query(root, l, r, Lx, Rx).val; }
};
