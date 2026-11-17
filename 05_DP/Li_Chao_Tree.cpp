// Li Chao Tree: minimum at x over inserted lines y = m x + c, each valid everywhere or on [l, r].
// Use when: CHT with slopes and queries in any order, or a line only counts on a range of x
// (Library Checker "Segment Add Get Min"), dp[i] = min over j of dp[j] + k[j] * x[i] online.
// Usage: LiChaoTree t(-1e9, 1e9); t.add_line({m, c}); t.add_segment({m, c}, l, r); t.query(x);
// Handles: any slope/query order, equal lines, negative m/c/x, segments [l, r] (inclusive),
// clipped to [L, R]. query returns INF when no line covers x. Max: insert {-m, -c}, negate.
// Time: add_line O(log C) | add_segment O(log^2 C) | query O(log C), C = R - L + 1
// Indexing: integer x in [L, R], fixed at construction (default [-1e9, 1e9])
// Note: nodes are made with new and never freed. Keep |m * x + c| < INF = 2e18 on [L, R].

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LiChaoTree {
    static constexpr ll INF = 2e18;
    struct Line {
        ll m, c;
        ll eval(ll x) const { return m * x + c; }
    };
    struct node {
        Line line;
        node *left = nullptr, *right = nullptr;
        node(Line line) : line(line) {}
        // node covers [l, r]; insert nw on the part of [L, R] inside it
        void add_segment(Line nw, ll l, ll r, ll L, ll R) {
            if (l > r || r < L || l > R) return;
            ll m = (l + 1 == r ? l : (l + r) / 2);
            if (l >= L && r <= R) {
                bool lef = nw.eval(l) < line.eval(l);
                bool mid = nw.eval(m) < line.eval(m);
                if (mid) swap(line, nw); // keep the winner at m here, push the loser to one side
                if (l == r) return;
                if (lef != mid) {
                    if (left == nullptr) left = new node(nw);
                    else left->add_segment(nw, l, m, L, R);
                } else {
                    if (right == nullptr) right = new node(nw);
                    else right->add_segment(nw, m + 1, r, L, R);
                }
                return;
            }
            if (max(l, L) <= min(m, R)) {
                if (left == nullptr) left = new node({0, INF});
                left->add_segment(nw, l, m, L, R);
            }
            if (max(m + 1, L) <= min(r, R)) {
                if (right == nullptr) right = new node({0, INF});
                right->add_segment(nw, m + 1, r, L, R);
            }
        }
        ll query(ll x, ll l, ll r) { // min over the lines on the root-to-x path
            ll ans = line.eval(x);
            if (l == r) return ans;
            ll m = (l + 1 == r ? l : (l + r) / 2);
            if (x <= m && left != nullptr) ans = min(ans, left->query(x, l, m));
            if (x > m && right != nullptr) ans = min(ans, right->query(x, m + 1, r));
            return ans;
        }
    };

    ll L, R;
    node *root;
    LiChaoTree(ll L = -1e9, ll R = 1e9) : L(L), R(R), root(new node({0, INF})) {}
    void add_line(Line line) { root->add_segment(line, L, R, L, R); }
    void add_segment(Line line, ll l, ll r) { root->add_segment(line, L, R, l, r); } // x in [l, r]
    ll query(ll x) { return root->query(x, L, R); } // INF if no line covers x; needs L <= x <= R
};
