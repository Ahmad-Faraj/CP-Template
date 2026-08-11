/*
    [1] Definition
    Data structure for range queries and point updates on a 2D matrix.

    [2] Time & Space Complexity
    Build: O(N * M)
    Update: O(log N * log M)
    Query: O(log N * log M)
    Space: O(N * M)

    [3] Notes
    Uses 1-based indexing for queries.
    Default node value (0) is the identity for sum queries.
    Tree dimensions are rounded up to the nearest power of 2.
*/

#include "../../core.h"

// Tree node
struct Node {
    int val;

    Node() { val = 0; }

    Node(int x) : val(x) {}

    void update(int x) { val += x; }
};

// 1-based indexing
class Segment_Tree_2D {
   private:
    int size_x, size_y;
    vector<vector<Node>> tree;

    void init_y(const vector<vector<int>>& grid, int vx, int lx, int rx, int vy, int ly, int ry) {
        if (ly == ry) {
            if (lx == rx) {
                if (lx <= sz(grid) && ly <= sz(grid[0])) tree[vx][vy] = Node(grid[lx - 1][ly - 1]);
                return;
            }
            tree[vx][vy] = merge(tree[2 * vx][vy], tree[2 * vx + 1][vy]);
            return;
        }

        int mid = (ly + ry) / 2;
        init_y(grid, vx, lx, rx, 2 * vy, ly, mid);
        init_y(grid, vx, lx, rx, 2 * vy + 1, mid + 1, ry);

        tree[vx][vy] = merge(tree[vx][2 * vy], tree[vx][2 * vy + 1]);
    }

    void init_x(const vector<vector<int>>& grid, int vx, int lx, int rx) {
        if (lx != rx) {
            int mid = (lx + rx) / 2;
            init_x(grid, 2 * vx, lx, mid);
            init_x(grid, 2 * vx + 1, mid + 1, rx);
        }
        init_y(grid, vx, lx, rx, 1, 1, size_y);
    }

    // Combine left and right nodes
    Node merge(Node& l, Node& r) { return Node(l.val + r.val); }

    // Update node at index
    void set_y(int vx, int lx, int rx, int vy, int ly, int ry, int x, int y, int v) {
        if (ly == ry) {
            if (lx == rx)
                tree[vx][vy].val = v;
            else
                tree[vx][vy] = merge(tree[2 * vx][vy], tree[2 * vx + 1][vy]);
            return;
        }

        int mid = (ly + ry) / 2;
        if (y <= mid)
            set_y(vx, lx, rx, 2 * vy, ly, mid, x, y, v);
        else
            set_y(vx, lx, rx, 2 * vy + 1, mid + 1, ry, x, y, v);

        tree[vx][vy] = merge(tree[vx][2 * vy], tree[vx][2 * vy + 1]);
    }

    void set_x(int vx, int lx, int rx, int x, int y, int v) {
        if (lx != rx) {
            int mid = (lx + rx) / 2;
            if (x <= mid)
                set_x(2 * vx, lx, mid, x, y, v);
            else
                set_x(2 * vx + 1, mid + 1, rx, x, y, v);
        }
        set_y(vx, lx, rx, 1, 1, size_y, x, y, v);
    }

    // Query range [lx, rx] x [ly, ry]
    Node get_y(const int ly, const int ry, int vx, int vy, int tly, int try_) {
        if (ly > try_ || ry < tly) return Node();

        if (ly <= tly && ry >= try_) return tree[vx][vy];

        int mid = (tly + try_) / 2;

        Node ln = get_y(ly, ry, vx, 2 * vy, tly, mid);
        Node rn = get_y(ly, ry, vx, 2 * vy + 1, mid + 1, try_);

        return merge(ln, rn);
    }

    Node get_x(const int lx, const int rx, const int ly, const int ry, int vx, int tlx, int trx) {
        if (lx > trx || rx < tlx) return Node();

        if (lx <= tlx && rx >= trx) return get_y(ly, ry, vx, 1, 1, size_y);

        int mid = (tlx + trx) / 2;

        Node ln = get_x(lx, rx, ly, ry, 2 * vx, tlx, mid);
        Node rn = get_x(lx, rx, ly, ry, 2 * vx + 1, mid + 1, trx);

        return merge(ln, rn);
    }

   public:
    Segment_Tree_2D(int n, int m) {
        size_x = 1;
        while (size_x < n) size_x <<= 1;
        size_y = 1;
        while (size_y < m) size_y <<= 1;
        tree = vector<vector<Node>>(2 * size_x, vector<Node>(2 * size_y, Node()));
    }

    // Build tree
    void init(const vector<vector<int>>& grid) { init_x(grid, 1, 1, size_x); }

    // Update value at index (x, y)
    void update(const int x, const int y, const int v) { set_x(1, 1, size_x, x, y, v); }

    // Get result for range [lx, rx] x [ly, ry]
    int query(const int lx, const int rx, const int ly, const int ry) { return get_x(lx, rx, ly, ry, 1, 1, size_x).val; }
};