// Dynamic Segment Tree: range sum over a huge coordinate range, allocating nodes only where touched.
// Use when: indices reach 1e9 and values arrive online, so coordinate compression is not possible.
// Handles: point add, range sum, negative coordinates. Allocates O(q log C) nodes. No lazy, no range updates.
// Time: update O(log C) | query O(log C), C = Rx - Lx
// Indexing: 1-based over the [lx, rx] passed to the constructor
// Note: update() adds to a position, it does not assign. Nodes are never freed.

#include <bits/stdc++.h>
using namespace std;

struct Dynamic_Segment_Tree {
    struct Node {
        int val;
        Node(int _val = 0) : val(_val) {}
        Node operator=(int x) {
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

    void update(TreeNode *&treenode, int idx, int val, int lx, int rx) {
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

    void update(int idx, int val) { update(root, idx, val, Lx, Rx); } // a[idx] += val

    Node query(TreeNode *treenode, int l, int r, int lx, int rx) {
        if (!treenode || lx > r || rx < l) return Node();
        if (lx >= l && rx <= r) return treenode->data;
        int mx = lx + (rx - lx) / 2;
        Node leftNode = query(treenode->left, l, r, lx, mx);
        Node rightNode = query(treenode->right, l, r, mx + 1, rx);
        return merge(leftNode, rightNode);
    }

    int query(int l, int r) { return query(root, l, r, Lx, Rx).val; } // sum over a[l .. r]
};

// Standard problem: hold a multiset of values up to 1e9, count how many fall in [l, r], with point changes
void solve() {
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    Dynamic_Segment_Tree dst(1, 1e9);
    for (int i = 0; i < n; i++) cin >> a[i], dst.update(a[i], 1);
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int idx, val;
            cin >> idx >> val;
            dst.update(a[idx - 1], -1);
            a[idx - 1] = val;
            dst.update(val, 1);
        } else {
            int l, r;
            cin >> l >> r;
            cout << dst.query(l, r) << '\n';
        }
    }
}
