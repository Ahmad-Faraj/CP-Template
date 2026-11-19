// Dynamic Segment Tree: point add, range sum over a huge index range; nodes made only when touched.
// Use when: indices up to 1e9 (or negative) arrive online, so coordinate compression is impossible.
// Usage: Dynamic_Segment_Tree st(1, 1e9); st.update(x, +1); st.query(l, r);
// Handles: negative coordinates, the full [-1e9, 1e9] default range, l > r (returns 0).
// Not: range updates, assignment (update() ADDS to a position).
// Time: update O(log C) | query O(log C), C = Rx - Lx + 1; memory O(q log C) nodes
// Indexing: positions are the integers in [Lx, Rx] given to the constructor (inclusive)
// Note: update() outside [Lx, Rx] corrupts the tree. Nodes are never freed (~60 bytes each).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Dynamic_Segment_Tree {
    struct Node {
        ll val;
        Node(ll _val = 0) : val(_val) {}
        Node operator=(ll x) {
            val = x;
            return *this;
        }
    };
    struct TreeNode {
        Node data;
        TreeNode *left, *right;
        TreeNode() : data(), left(nullptr), right(nullptr) {}
    };

    TreeNode *root;
    int Lx, Rx;

    Dynamic_Segment_Tree(int lx = -1e9, int rx = 1e9) : Lx(lx), Rx(rx) { root = new TreeNode(); }

    Node merge(Node &a, Node &b) {
        Node res;
        res.val = a.val + b.val;
        return res;
    }

    static int mid(int lx, int rx) { return (int)(((ll)lx + rx) >> 1); }

    void update(TreeNode *&treenode, int idx, ll val, int lx, int rx) {
        if (!treenode) treenode = new TreeNode();
        if (lx == rx) {
            treenode->data.val += val;
            return;
        }
        int mx = mid(lx, rx);
        if (idx <= mx)
            update(treenode->left, idx, val, lx, mx);
        else
            update(treenode->right, idx, val, mx + 1, rx);

        Node leftNode = treenode->left ? treenode->left->data : Node(0);
        Node rightNode = treenode->right ? treenode->right->data : Node(0);
        treenode->data = merge(leftNode, rightNode);
    }

    void update(int idx, ll val) { update(root, idx, val, Lx, Rx); } // a[idx] += val

    Node query(TreeNode *treenode, int l, int r, int lx, int rx) {
        if (!treenode || lx > r || rx < l) return Node();
        if (lx >= l && rx <= r) return treenode->data;
        int mx = mid(lx, rx);
        Node leftNode = query(treenode->left, l, r, lx, mx);
        Node rightNode = query(treenode->right, l, r, mx + 1, rx);
        return merge(leftNode, rightNode);
    }

    ll query(int l, int r) { return query(root, l, r, Lx, Rx).val; } // sum of a[l .. r]
};
