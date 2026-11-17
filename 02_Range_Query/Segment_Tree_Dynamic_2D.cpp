#include <bits/stdc++.h>
using namespace std;
#define ll long long

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
        : R_min_dim(r_min), R_max_dim(r_max), C_min_dim(c_min), C_max_dim(c_max) {
        root = new Y_NODE(C_min_dim, C_max_dim);
    }
    SegTree2D() {}

    void update_y(X_NODE *node, int q, ll k) {
        int s = node->s, e = node->e;
        if (s == e) {
            node->value = k; // apply update
            return;
        }

        int m = s + (e - s) / 2;
        X_NODE **child_ptr = (q <= m) ? &(node->left) : &(node->right);

        if (*child_ptr == nullptr) {
            *child_ptr = new X_NODE(q, q);
            (*child_ptr)->value = k; // apply update
        } else if ((*child_ptr)->s <= q && q <= (*child_ptr)->e) {
            update_y(*child_ptr, q, k);
        } else {
            int new_s = s, new_e = e;
            int new_m = m;
            do {
                if (q <= new_m)
                    new_e = new_m;
                else
                    new_s = new_m + 1;
                new_m = new_s + (new_e - new_s) / 2;
            } while ((q <= new_m) == ((*child_ptr)->e <= new_m));

            X_NODE *new_node = new X_NODE(new_s, new_e);
            if ((*child_ptr)->e <= new_m) {
                new_node->left = *child_ptr;
            } else {
                new_node->right = *child_ptr;
            }
            *child_ptr = new_node;
            update_y(*child_ptr, q, k);
        }

        ll left_val = node->left ? node->left->value : 0;
        ll right_val = node->right ? node->right->value : 0;
        node->value = merge(left_val, right_val);
    }

    ll query_y(X_NODE *node, int s, int e) {
        if (node == nullptr || node->s > e || node->e < s) {
            return 0;
        }
        if (s <= node->s && node->e <= e) {
            return node->value;
        }
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
        if (node == nullptr || s > u || e < p) {
            return 0;
        }
        if (p <= s && e <= u) {
            return query_y(&node->xtree, q, v);
        }
        int m = s + (e - s) / 2;
        return merge(query_x(node->left, s, m, p, q, u, v), query_x(node->right, m + 1, e, p, q, u, v));
    }

    void update(int x, int y, long long val) { update_x(root, R_min_dim, R_max_dim, x, y, val); }

    ll query(int x1, int y1, int x2, int y2) { return query_x(root, R_min_dim, R_max_dim, x1, y1, x2, y2); }
};

void solve() {
    int n, q;
    cin >> n >> q;
    SegTree2D st(0, 1e9, 0, 1e9);
    for (int i = 0; i < n; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        st.update(x, y, w);
    }
    for (int i = 0; i < q; i++) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        cout << st.query(x1, y1, x2 - 1, y2 - 1) << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0), cin.tie(0);
    solve();
    return 0;
}