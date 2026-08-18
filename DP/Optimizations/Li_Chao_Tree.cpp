#include "../../core.h"

/*
 * Topic: DP - Li Chao Tree
 * Description: Li Chao Tree reduces the time complexity of DP transitions of
 *   the form dp[i] = min/max(m[j] * x[i] + b[j]) from O(N^2) to O(N log(Range)).
 *   It is a data structure used to maintain a set of lines or line segments
 *   and query the minimum value among them at any given x.
 * Condition: Use this when NEITHER slopes m[j] nor queries x[i] are monotonic (i.e. both are random).
 * Time Complexity: O(log(Range)) per addition and query.
 * 
 * To convert to a MAXIMUM query Li Chao Tree:
 * 1. Change all 'inf' defaults to '-inf' in node creation and null returns.
 * 2. Change '<' to '>' in both `eval()` comparisons inside `add_segment`.
 * 3. Change `min` to `max` in the `query` function.
 */

const ll inf = 2e18;

struct LiChaoTree {
    struct node {
        ll m, c;
        node *left = nullptr;
        node *right = nullptr;
        node(ll m, ll c) : m(m), c(c) {}
        ll eval(ll x) { return m * x + c; }
    };

    int L, R;
    node *root;

    LiChaoTree(int L = -2e9, int R = 2e9) : L(L), R(R) { 
        root = new node(0, inf); // change inf to -inf for max
    }

    void add_segment(ll nw_m, ll nw_c, int l, int r, int L_query, int R_query, node*& cur) {
        if (l > r || r < L_query || l > R_query) return;
        if (cur == nullptr) cur = new node(0, inf); // change inf to -inf for max
        int mid = l + (r - l) / 2; 
        
        if (l >= L_query && r <= R_query) {
            bool lef = (nw_m * l + nw_c) < cur->eval(l); // change < to > for max
            bool m_mid = (nw_m * mid + nw_c) < cur->eval(mid); // change < to > for max
            if (m_mid) {
                swap(cur->m, nw_m);
                swap(cur->c, nw_c);
            }
            if (l == r) return;
            if (lef != m_mid) {
                add_segment(nw_m, nw_c, l, mid, L_query, R_query, cur->left);
            } else {
                add_segment(nw_m, nw_c, mid + 1, r, L_query, R_query, cur->right);
            }
            return;
        }
        
        if (max(l, L_query) <= min(mid, R_query)) {
            add_segment(nw_m, nw_c, l, mid, L_query, R_query, cur->left);
        }
        if (max(mid + 1, L_query) <= min(r, R_query)) {
            add_segment(nw_m, nw_c, mid + 1, r, L_query, R_query, cur->right);
        }
    }

    void add_line(ll m, ll c) { 
        add_segment(m, c, L, R, L, R, root); 
    }

    // y = mx + c restricted to segment [l_query, r_query]
    void add_segment(ll m, ll c, int l_query, int r_query) { 
        add_segment(m, c, L, R, l_query, r_query, root); 
    }

    ll query(ll x, int l, int r, node* cur) {
        if (cur == nullptr) return inf; // change inf to -inf for max
        ll ans = cur->eval(x);
        if (l == r) return ans;
        int mid = l + (r - l) / 2;
        if (x <= mid) ans = min(ans, query(x, l, mid, cur->left)); // change min to max
        else ans = min(ans, query(x, mid + 1, r, cur->right)); // change min to max
        return ans;
    }

    ll query(ll x) { 
        return query(x, L, R, root); 
    }
};