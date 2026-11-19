// Segment Tree Beats: range chmin / chmax / add / assign with range sum, min and max queries.
// Use when: "a[i] = min(a[i], x)" or "a[i] = max(a[i], x)" over a range, mixed with sum queries.
// Usage: SGTBeats t(a); t.update_min(l, r, x); t.add_val(l, r, x); t.query_sum(l, r);
// Handles: chmin, chmax, range add, range assign, range sum/min/max, negatives, n = 0.
// Time: amortized O(log^2 n) per update | O(log n) per query
// Indexing: 0-based, ranges HALF-OPEN [l, r) - the whole array is [0, n)
// Note: |values| must stay far below inf = 1e18 (a sentinel) and sums must fit in ll.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SGTBeats {
    static constexpr ll inf = (ll)1e18; // query_max / query_min of an empty range
    int n, n0;
    vector<ll> max_v, smax_v, max_c, min_v, smin_v, min_c, sum, len, ladd, lval;

    void update_node_max(int k, ll x) {
        sum[k] += (x - max_v[k]) * max_c[k];
        if (max_v[k] == min_v[k]) max_v[k] = min_v[k] = x;
        else if (max_v[k] == smin_v[k]) max_v[k] = smin_v[k] = x;
        else max_v[k] = x;
        if (lval[k] != inf && x < lval[k]) lval[k] = x;
    }
    void update_node_min(int k, ll x) {
        sum[k] += (x - min_v[k]) * min_c[k];
        if (max_v[k] == min_v[k]) max_v[k] = min_v[k] = x;
        else if (smax_v[k] == min_v[k]) min_v[k] = smax_v[k] = x;
        else min_v[k] = x;
        if (lval[k] != inf && lval[k] < x) lval[k] = x;
    }
    void push(int k) {
        if (n0 - 1 <= k) return;
        if (lval[k] != inf) {
            updateall(2 * k + 1, lval[k]);
            updateall(2 * k + 2, lval[k]);
            lval[k] = inf;
            return;
        }
        if (ladd[k] != 0) {
            addall(2 * k + 1, ladd[k]);
            addall(2 * k + 2, ladd[k]);
            ladd[k] = 0;
        }
        for (int c = 2 * k + 1; c <= 2 * k + 2; c++) {
            if (max_v[k] < max_v[c]) update_node_max(c, max_v[k]);
            if (min_v[c] < min_v[k]) update_node_min(c, min_v[k]);
        }
    }
    void update(int k) {
        int L = 2 * k + 1, R = 2 * k + 2;
        sum[k] = sum[L] + sum[R];
        if (max_v[L] < max_v[R]) {
            max_v[k] = max_v[R], max_c[k] = max_c[R];
            smax_v[k] = max(max_v[L], smax_v[R]);
        } else if (max_v[L] > max_v[R]) {
            max_v[k] = max_v[L], max_c[k] = max_c[L];
            smax_v[k] = max(smax_v[L], max_v[R]);
        } else {
            max_v[k] = max_v[L], max_c[k] = max_c[L] + max_c[R];
            smax_v[k] = max(smax_v[L], smax_v[R]);
        }
        if (min_v[L] < min_v[R]) {
            min_v[k] = min_v[L], min_c[k] = min_c[L];
            smin_v[k] = min(smin_v[L], min_v[R]);
        } else if (min_v[L] > min_v[R]) {
            min_v[k] = min_v[R], min_c[k] = min_c[R];
            smin_v[k] = min(min_v[L], smin_v[R]);
        } else {
            min_v[k] = min_v[L], min_c[k] = min_c[L] + min_c[R];
            smin_v[k] = min(smin_v[L], smin_v[R]);
        }
    }
    void addall(int k, ll x) {
        max_v[k] += x;
        if (smax_v[k] != -inf) smax_v[k] += x;
        min_v[k] += x;
        if (smin_v[k] != inf) smin_v[k] += x;
        sum[k] += len[k] * x;
        if (lval[k] != inf) lval[k] += x;
        else ladd[k] += x;
    }
    void updateall(int k, ll x) {
        max_v[k] = min_v[k] = x;
        smax_v[k] = -inf, smin_v[k] = inf;
        max_c[k] = min_c[k] = len[k];
        sum[k] = x * len[k];
        lval[k] = x, ladd[k] = 0;
    }

    void _update_min(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || max_v[k] <= x) return;
        if (a <= l && r <= b && smax_v[k] < x) return update_node_max(k, x);
        push(k);
        _update_min(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_min(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void _update_max(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || x <= min_v[k]) return;
        if (a <= l && r <= b && x < smin_v[k]) return update_node_min(k, x);
        push(k);
        _update_max(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_max(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void _add_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) return addall(k, x);
        push(k);
        _add_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _add_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    void _update_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) return updateall(k, x);
        push(k);
        _update_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }
    ll _query_max(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return -inf;
        if (a <= l && r <= b) return max_v[k];
        push(k);
        return max(_query_max(a, b, 2 * k + 1, l, (l + r) / 2),
                   _query_max(a, b, 2 * k + 2, (l + r) / 2, r));
    }
    ll _query_min(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return inf;
        if (a <= l && r <= b) return min_v[k];
        push(k);
        return min(_query_min(a, b, 2 * k + 1, l, (l + r) / 2),
                   _query_min(a, b, 2 * k + 2, (l + r) / 2, r));
    }
    ll _query_sum(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return 0;
        if (a <= l && r <= b) return sum[k];
        push(k);
        return _query_sum(a, b, 2 * k + 1, l, (l + r) / 2) +
               _query_sum(a, b, 2 * k + 2, (l + r) / 2, r);
    }

    SGTBeats(const vector<ll> &a) : n((int)a.size()) {
        n0 = 1;
        while (n0 < n) n0 <<= 1;
        for (auto *v : {&max_v, &smax_v, &max_c, &min_v, &smin_v, &min_c, &sum, &len, &ladd})
            v->assign(2 * n0, 0);
        lval.assign(2 * n0, inf);
        len[0] = n0;
        for (int i = 0; i < n0 - 1; ++i) len[2 * i + 1] = len[2 * i + 2] = (len[i] >> 1);
        for (int i = 0; i < n0; ++i) {
            int k = n0 - 1 + i;
            if (i < n) {
                max_v[k] = min_v[k] = sum[k] = a[i];
                smax_v[k] = -inf, smin_v[k] = inf;
                max_c[k] = min_c[k] = 1;
            } else {
                max_v[k] = smax_v[k] = -inf;
                min_v[k] = smin_v[k] = inf;
                max_c[k] = min_c[k] = 0;
            }
        }
        for (int i = n0 - 2; i >= 0; i--) update(i);
    }

    void update_min(int a, int b, ll x) { _update_min(x, a, b, 0, 0, n0); } // a[i] = min(a[i], x)
    void update_max(int a, int b, ll x) { _update_max(x, a, b, 0, 0, n0); } // a[i] = max(a[i], x)
    void add_val(int a, int b, ll x) { _add_val(x, a, b, 0, 0, n0); }       // a[i] += x
    void update_val(int a, int b, ll x) { _update_val(x, a, b, 0, 0, n0); } // a[i] = x
    ll query_max(int a, int b) { return _query_max(a, b, 0, 0, n0); }
    ll query_min(int a, int b) { return _query_min(a, b, 0, 0, n0); }
    ll query_sum(int a, int b) { return _query_sum(a, b, 0, 0, n0); }
};
