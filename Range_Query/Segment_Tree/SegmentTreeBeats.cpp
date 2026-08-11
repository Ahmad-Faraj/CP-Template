/*
    [1] Definition
    Segment Tree Beats supporting range chmin ($a_i = \min(a_i, x)$), range chmax ($a_i = \max(a_i, x)$),
    range addition, range set, along with range sum, range min, and range max queries.

    [2] Time & Space Complexity
    Build: O(N)
    Update (Add / Set): O(log N)
    Update (Chmin / Chmax): O(log N) amortized
    Query (Sum / Min / Max): O(log N)
    Space: O(N)

    [3] Notes
    Uses 0-based indexing with half-open intervals [l, r) (right exclusive).
    Tracks largest, second largest, smallest, second smallest, counts, sum, and lazy tags.
*/

#include "../../core.h"

const int N = 2e5 + 9;

// Segment Tree Beats: range add/set/min/max updates & sum/min/max queries
// all queries are performed on [l, r) segment (right exclusive)
// 0 indexed
class SGTBeats {
   private:
    static constexpr ll INF = (1LL << 60);

    int n, n0;

    vector<ll> max_v, smax_v, max_c;
    vector<ll> min_v, smin_v, min_c;
    vector<ll> sum;
    vector<ll> ladd, lval;
    vector<int> len;

    // Apply: a[i] = min(a[i], x)
    void update_node_max(int k, ll x) {
        sum[k] += (x - max_v[k]) * max_c[k];

        if (max_v[k] == min_v[k]) {
            max_v[k] = min_v[k] = x;
        } else if (max_v[k] == smin_v[k]) {
            max_v[k] = smin_v[k] = x;
        } else {
            max_v[k] = x;
        }

        if (lval[k] != INF && x < lval[k]) lval[k] = x;
    }

    // Apply: a[i] = max(a[i], x)
    void update_node_min(int k, ll x) {
        sum[k] += (x - min_v[k]) * min_c[k];

        if (max_v[k] == min_v[k]) {
            max_v[k] = min_v[k] = x;
        } else if (smax_v[k] == min_v[k]) {
            min_v[k] = smax_v[k] = x;
        } else {
            min_v[k] = x;
        }

        if (lval[k] != INF && lval[k] < x) lval[k] = x;
    }

    // Push lazy tags
    void push(int k) {
        if (k >= n0 - 1) return;

        int lc = 2 * k + 1;
        int rc = 2 * k + 2;

        // Set
        if (lval[k] != INF) {
            updateall(lc, lval[k]);
            updateall(rc, lval[k]);

            lval[k] = INF;

            return;
        }

        // Add
        if (ladd[k] != 0) {
            addall(lc, ladd[k]);
            addall(rc, ladd[k]);

            ladd[k] = 0;
        }

        // Chmin constraint
        if (max_v[lc] > max_v[k]) update_node_max(lc, max_v[k]);

        if (max_v[rc] > max_v[k]) update_node_max(rc, max_v[k]);

        // Chmax constraint
        if (min_v[lc] < min_v[k]) update_node_min(lc, min_v[k]);

        if (min_v[rc] < min_v[k]) update_node_min(rc, min_v[k]);
    }

    // Merge children
    void update(int k) {
        int lc = 2 * k + 1;
        int rc = 2 * k + 2;

        sum[k] = sum[lc] + sum[rc];

        // Maximum
        if (max_v[lc] < max_v[rc]) {
            max_v[k] = max_v[rc];
            max_c[k] = max_c[rc];

            smax_v[k] = max(max_v[lc], smax_v[rc]);
        } else if (max_v[lc] > max_v[rc]) {
            max_v[k] = max_v[lc];
            max_c[k] = max_c[lc];

            smax_v[k] = max(smax_v[lc], max_v[rc]);
        } else {
            max_v[k] = max_v[lc];

            max_c[k] = max_c[lc] + max_c[rc];

            smax_v[k] = max(smax_v[lc], smax_v[rc]);
        }

        // Minimum
        if (min_v[lc] < min_v[rc]) {
            min_v[k] = min_v[lc];
            min_c[k] = min_c[lc];

            smin_v[k] = min(smin_v[lc], min_v[rc]);
        } else if (min_v[lc] > min_v[rc]) {
            min_v[k] = min_v[rc];
            min_c[k] = min_c[rc];

            smin_v[k] = min(min_v[lc], smin_v[rc]);
        } else {
            min_v[k] = min_v[lc];

            min_c[k] = min_c[lc] + min_c[rc];

            smin_v[k] = min(smin_v[lc], smin_v[rc]);
        }
    }

    // a[i] = min(a[i], x)
    void _update_min(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || max_v[k] <= x) return;

        if (a <= l && r <= b && smax_v[k] < x) {
            update_node_max(k, x);
            return;
        }

        push(k);

        int m = (l + r) / 2;

        _update_min(x, a, b, 2 * k + 1, l, m);

        _update_min(x, a, b, 2 * k + 2, m, r);

        update(k);
    }

    // a[i] = max(a[i], x)
    void _update_max(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || x <= min_v[k]) return;

        if (a <= l && r <= b && x < smin_v[k]) {
            update_node_min(k, x);
            return;
        }

        push(k);

        int m = (l + r) / 2;

        _update_max(x, a, b, 2 * k + 1, l, m);

        _update_max(x, a, b, 2 * k + 2, m, r);

        update(k);
    }

    // a[i] += x
    void addall(int k, ll x) {
        max_v[k] += x;
        min_v[k] += x;

        if (smax_v[k] != -INF) smax_v[k] += x;

        if (smin_v[k] != INF) smin_v[k] += x;

        sum[k] += 1LL * len[k] * x;

        if (lval[k] != INF)
            lval[k] += x;
        else
            ladd[k] += x;
    }

    // a[i] = x
    void updateall(int k, ll x) {
        max_v[k] = x;
        min_v[k] = x;

        smax_v[k] = -INF;
        smin_v[k] = INF;

        max_c[k] = len[k];
        min_c[k] = len[k];

        sum[k] = 1LL * len[k] * x;

        lval[k] = x;
        ladd[k] = 0;
    }

    // a[i] += x
    void _add_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            addall(k, x);
            return;
        }

        push(k);

        int m = (l + r) / 2;

        _add_val(x, a, b, 2 * k + 1, l, m);

        _add_val(x, a, b, 2 * k + 2, m, r);

        update(k);
    }

    // a[i] = x
    void _update_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;

        if (a <= l && r <= b) {
            updateall(k, x);
            return;
        }

        push(k);

        int m = (l + r) / 2;

        _update_val(x, a, b, 2 * k + 1, l, m);

        _update_val(x, a, b, 2 * k + 2, m, r);

        update(k);
    }

    // Maximum
    ll _query_max(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return -INF;

        if (a <= l && r <= b) return max_v[k];

        push(k);

        int m = (l + r) / 2;

        return max(_query_max(a, b, 2 * k + 1, l, m), _query_max(a, b, 2 * k + 2, m, r));
    }

    // Minimum
    ll _query_min(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return INF;

        if (a <= l && r <= b) return min_v[k];

        push(k);

        int m = (l + r) / 2;

        return min(_query_min(a, b, 2 * k + 1, l, m), _query_min(a, b, 2 * k + 2, m, r));
    }

    // Sum
    ll _query_sum(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return 0;

        if (a <= l && r <= b) return sum[k];

        push(k);

        int m = (l + r) / 2;

        return _query_sum(a, b, 2 * k + 1, l, m) + _query_sum(a, b, 2 * k + 2, m, r);
    }

   public:
    SGTBeats(int n, const ll* a = nullptr) : n(n) {
        n0 = 1;

        while (n0 < n) n0 <<= 1;

        int sz = 2 * n0;

        max_v.assign(sz, -INF);
        smax_v.assign(sz, -INF);

        min_v.assign(sz, INF);
        smin_v.assign(sz, INF);

        sum.assign(sz, 0);

        ladd.assign(sz, 0);
        lval.assign(sz, INF);

        max_c.assign(sz, 0);
        min_c.assign(sz, 0);

        len.assign(sz, 0);

        // Length of root
        len[0] = n0;

        // Length of every node
        for (int i = 0; i < n0 - 1; i++) {
            len[2 * i + 1] = len[i] / 2;
            len[2 * i + 2] = len[i] / 2;
        }

        // Build leaves
        for (int i = 0; i < n0; i++) {
            int k = n0 - 1 + i;

            if (i < n) {
                ll v = (a != nullptr ? a[i] : 0);

                max_v[k] = v;
                min_v[k] = v;
                sum[k] = v;

                smax_v[k] = -INF;
                smin_v[k] = INF;

                max_c[k] = 1;
                min_c[k] = 1;
            } else {
                // Dummy leaves
                max_v[k] = -INF;
                smax_v[k] = -INF;

                min_v[k] = INF;
                smin_v[k] = INF;

                max_c[k] = 0;
                min_c[k] = 0;

                sum[k] = 0;
            }
        }

        // Build
        for (int k = n0 - 2; k >= 0; k--) update(k);
    }

    // interface queries

    // a[i] = min(a[i], x)
    void update_min(int l, int r, ll x) { _update_min(x, l, r, 0, 0, n0); }

    // a[i] = max(a[i], x)
    void update_max(int l, int r, ll x) { _update_max(x, l, r, 0, 0, n0); }

    // a[i] += x
    void add_val(int l, int r, ll x) { _add_val(x, l, r, 0, 0, n0); }

    // a[i] = x
    void update_val(int l, int r, ll x) { _update_val(x, l, r, 0, 0, n0); }

    // Maximum on [l, r)
    ll query_max(int l, int r) { return _query_max(l, r, 0, 0, n0); }

    // Minimum on [l, r)
    ll query_min(int l, int r) { return _query_min(l, r, 0, 0, n0); }

    // Sum on [l, r)
    ll query_sum(int l, int r) { return _query_sum(l, r, 0, 0, n0); }
};
