#include <bits/stdc++.h>
using namespace std;
// Dynamic Segment Tree
// segment tree on the fly specify the range of the segment tree by passing the range in the constructor lx and rx
struct Dynamic_Segment_Tree
{
    struct Node
    {
        int val;
        Node(int _val = 0) : val(_val) {}
        Node operator=(int x)
        {
            val = x;
            return *this;
        }
    };
    struct TreeNode
    {
        Node data;
        TreeNode *left, *right;
        TreeNode() : data(), left(nullptr), right(nullptr) {}
    };

    TreeNode *root;
    int Lx, Rx;

    Dynamic_Segment_Tree(int lx = -1e9, int rx = 1e9) : Lx(lx), Rx(rx)
    {
        root = new TreeNode();
    }

    Node merge(Node &a, Node &b)
    {
        Node res;
        res.val = a.val + b.val;
        return res;
    }

    void update(TreeNode *&treenode, int idx, int val, int lx, int rx)
    {
        if (!treenode)
            treenode = new TreeNode();
        if (lx == rx)
        {
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

    void update(int idx, int val)
    {
        update(root, idx, val, Lx, Rx);
    }

    Node query(TreeNode *treenode, int l, int r, int lx, int rx)
    {
        if (!treenode || lx > r || rx < l)
            return Node();
        if (lx >= l && rx <= r)
            return (treenode ? treenode->data : Node(0));
        int mx = lx + (rx - lx) / 2;
        Node leftNode = query(treenode->left, l, r, lx, mx);
        Node rightNode = query(treenode->right, l, r, mx + 1, rx);
        return merge(leftNode, rightNode);
    }

    int query(int l, int r)
    {
        return query(root, l, r, Lx, Rx).val;
    }
};

void solve()
{
    int n, q;
    cin >> n >> q;
    vector<int> a(n);
    Dynamic_Segment_Tree dst(1, 1e9);

    for (int i = 0; i < n; i++)
    {
        cin >> a[i];
        dst.update(a[i], 1);
    }

    while (q--)
    {
        char type;
        cin >> type;
        if (type == '!')
        {
            int idx, val;
            cin >> idx >> val;
            dst.update(a[idx - 1], -1);
            a[idx - 1] = val;
            dst.update(val, 1);
        }
        else
        {
            int l, r;
            cin >> l >> r;
            cout << dst.query(l, r) << '\n';
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}