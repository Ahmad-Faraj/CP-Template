// Dynamic 2D Segment Tree: rectangle sum over a huge sparse coordinate space, nodes allocated on demand.
// Use when: points sit at coordinates up to 1e9 and you need "sum of the rectangle (x1,y1)..(x2,y2)".
// Handles: point assign, rectangle sum, negative coordinates. Only touched points cost memory. No lazy.
// Time: update O(log^2 C) | query O(log^2 C), C = coordinate span
// Indexing: raw coordinates inside the [r_min, r_max] x [c_min, c_max] given to the constructor, inclusive
// Note: update() assigns at a point, it does not add. Nodes are never freed.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SegTree2D {
    int R_max_dim, C_max_dim, R_min_dim, C_min_dim;

    ll merge(ll x, ll y) { return x + y; }

    struct X_NODE {
        int s, e;
        X_NODE *left = nullptr, *right = nullptr;
        ll value = 0LL;
        X_NODE(int s, int e) : s(s), e(e) { value = 0; }
        ~X_NODE() {
            delete left;
            delete right;
        }
    };

    struct Y_NODE {
        Y_NODE *left = nullptr, *right = nullptr;
        X_NODE xtree;
        Y_NODE(int c_min = -1e9, int c_max = 1e9) : xtree(c_min, c_max) {}
        ~Y_NODE() {
            delete left;
            delete right;
        }
    };

    Y_NODE *root = nullptr;

    SegTree2D(int r_min, int r_max, int c_min, int c_max)
        : R_max_dim(r_max), C_max_dim(c_max), R_min_dim(r_min), C_min_dim(c_min) {
        root = new Y_NODE(C_min_dim, C_max_dim);
    }

    void update_y(X_NODE *node, int q, ll k) {
        int s = node->s, e = node->e;
        if (s == e) {
            node->value = k;
            return;
        }
        int m = s + (e - s) / 2;
        X_NODE **child_ptr = (q <= m) ? &(node->left) : &(node->right);
        if (*child_ptr == nullptr) {
            *child_ptr = new X_NODE(q, q);
            (*child_ptr)->value = k;
        } else if ((*child_ptr)->s <= q && q <= (*child_ptr)->e) {
            update_y(*child_ptr, q, k);
        } else {
            int new_s = s, new_e = e, new_m = m;
            do {
                if (q <= new_m)
                    new_e = new_m;
                else
                    new_s = new_m + 1;
                new_m = new_s + (new_e - new_s) / 2;
            } while ((q <= new_m) == ((*child_ptr)->e <= new_m));
            X_NODE *new_node = new X_NODE(new_s, new_e);
            if ((*child_ptr)->e <= new_m)
                new_node->left = *child_ptr;
            else
                new_node->right = *child_ptr;
            *child_ptr = new_node;
            update_y(*child_ptr, q, k);
        }
        ll left_val = node->left ? node->left->value : 0;
        ll right_val = node->right ? node->right->value : 0;
        node->value = merge(left_val, right_val);
    }

    ll query_y(X_NODE *node, int s, int e) {
        if (node == nullptr || node->s > e || node->e < s) return 0;
        if (s <= node->s && node->e <= e) return node->value;
        return merge(query_y(node->left, s, e), query_y(node->right, s, e));
    }

    void update_x(Y_NODE *node, int s, int e, int p, int q, ll k) {
        if (s == e) {
            update_y(&node->xtree, q, k);
            return;
        }
        int m = s + (e - s) / 2;
        if (p <= m) {
            if (node->left == nullptr) node->left = new Y_NODE(C_min_dim, C_max_dim);
            update_x(node->left, s, m, p, q, k);
        } else {
            if (node->right == nullptr) node->right = new Y_NODE(C_min_dim, C_max_dim);
            update_x(node->right, m + 1, e, p, q, k);
        }
        ll left_val = node->left ? query_y(&node->left->xtree, q, q) : 0;
        ll right_val = node->right ? query_y(&node->right->xtree, q, q) : 0;
        update_y(&node->xtree, q, merge(left_val, right_val));
    }

    ll query_x(Y_NODE *node, int s, int e, int p, int q, int u, int v) {
        if (node == nullptr || s > u || e < p) return 0;
        if (p <= s && e <= u) return query_y(&node->xtree, q, v);
        int m = s + (e - s) / 2;
        return merge(query_x(node->left, s, m, p, q, u, v), query_x(node->right, m + 1, e, p, q, u, v));
    }

    void update(int x, int y, ll val) { update_x(root, R_min_dim, R_max_dim, x, y, val); } // cell (x, y) = val

    ll query(int x1, int y1, int x2, int y2) { // sum over rows [x1,x2] x cols [y1,y2]
        return query_x(root, R_min_dim, R_max_dim, x1, y1, x2, y2);
    }
};

// Standard problem: n weighted points at sparse coordinates, then q rectangle-sum queries
void solve() {
    int n, q;
    cin >> n >> q;
    SegTree2D st(0, 1e9, 0, 1e9);
    for (int i = 0; i < n; i++) {
        int x, y;
        ll w;
        cin >> x >> y >> w;
        st.update(x, y, w);
    }
    while (q--) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << st.query(x1, y1, x2, y2) << '\n';
    }
}
