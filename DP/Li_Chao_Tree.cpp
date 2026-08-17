// Li Chao Tree: minimum over a set of lines / segments at a query x, inserted online.
// Use when: CHT but a line is only valid on a sub-range of x, or you want the query bounded to a range.
// Handles: arbitrary slope and query order, lines restricted to [l, r], negative m/c/x. No deletion, not persistent.
// Time: add_line O(log C) | add_segment O(log^2 C) | query O(log C), C = R - L
// Indexing: 0-based (x is any integer inside the [L, R] passed to the constructor)
// Note: min-tree. For maximum insert {-m, -c} and negate the answer. Returns inf when no line covers x.

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll inf = 2e18;

struct Line {
    ll m, c;
    ll eval(ll x) { return m * x + c; }
};

struct node {
    Line line;
    node *left = nullptr;
    node *right = nullptr;
    node(Line line) : line(line) {}
    void add_segment(Line nw, int l, int r, int L, int R) {
        if (l > r || r < L || l > R) return;
        int m = (l + 1 == r ? l : (l + r) / 2);
        if (l >= L and r <= R) {
            bool lef = nw.eval(l) < line.eval(l);
            bool mid = nw.eval(m) < line.eval(m);
            if (mid) swap(line, nw);
            if (l == r) return;
            if (lef != mid) {
                if (left == nullptr)
                    left = new node(nw);
                else
                    left->add_segment(nw, l, m, L, R);
            } else {
                if (right == nullptr)
                    right = new node(nw);
                else
                    right->add_segment(nw, m + 1, r, L, R);
            }
            return;
        }
        if (max(l, L) <= min(m, R)) {
            if (left == nullptr) left = new node({0, inf});
            left->add_segment(nw, l, m, L, R);
        }
        if (max(m + 1, L) <= min(r, R)) {
            if (right == nullptr) right = new node({0, inf});
            right->add_segment(nw, m + 1, r, L, R);
        }
    }
    ll query_segment(ll x, int l, int r, int L, int R) {
        if (l > r || r < L || l > R) return inf;
        int m = (l + 1 == r ? l : (l + r) / 2);
        if (l >= L and r <= R) {
            ll ans = line.eval(x);
            if (l < r) {
                if (x <= m && left != nullptr) ans = min(ans, left->query_segment(x, l, m, L, R));
                if (x > m && right != nullptr) ans = min(ans, right->query_segment(x, m + 1, r, L, R));
            }
            return ans;
        }
        ll ans = inf;
        if (max(l, L) <= min(m, R)) {
            if (left == nullptr) left = new node({0, inf});
            ans = min(ans, left->query_segment(x, l, m, L, R));
        }
        if (max(m + 1, L) <= min(r, R)) {
            if (right == nullptr) right = new node({0, inf});
            ans = min(ans, right->query_segment(x, m + 1, r, L, R));
        }
        return ans;
    }
};

struct LiChaoTree {
    int L, R;
    node *root;
    LiChaoTree(int L, int R) : L(L), R(R) { root = new node({0, inf}); }
    void add_line(Line line) { root->add_segment(line, L, R, L, R); }                     // y = mx + c for every x
    void add_segment(Line line, int l, int r) { root->add_segment(line, L, R, l, r); }    // y = mx + c for x in [l, r]
    ll query(ll x) { return root->query_segment(x, L, R, L, R); }                         // min over all inserted at x
    ll query_segment(ll x, int l, int r) { return root->query_segment(x, l, r, L, R); } // min over those on [l, r]
};

void solve() {
    LiChaoTree t((int)-1e9, (int)1e9);
    int n, q;
    cin >> n >> q;
    for (int i = 0; i < n; i++) {
        ll l, r, a, b;
        cin >> l >> r >> a >> b;
        t.add_segment({a, b}, l, r - 1);
    }
    while (q--) {
        int ty;
        cin >> ty;
        if (ty == 0) {
            ll l, r, a, b;
            cin >> l >> r >> a >> b;
            t.add_segment({a, b}, l, r - 1);
        } else {
            ll x;
            cin >> x;
            ll ans = t.query(x);
            if (ans >= inf)
                cout << "INFINITY\n";
            else
                cout << ans << '\n';
        }
    }
}
