// Segment Tree Beats: range chmin / chmax / add / assign, with range sum, min and max queries.
// Use when: an update is "a[i] = min(a[i], x)" or "a[i] = max(a[i], x)" over a range, alongside sum queries.
// Handles: chmin, chmax, range add, range assign, range sum/min/max, negatives. Sizes itself to n.
// Time: amortized O(log^2 n) per update | O(log n) per query
// Indexing: 0-based and ranges are HALF-OPEN [l, r) - r is excluded, unlike every other file here
// Note: half-open ranges. The whole array is query_sum(0, n). Passing an inclusive r silently reads one element short.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SGTBeats {
    const ll inf = 1e18;
    int n, n0;
    vector<ll> max_v, smax_v, max_c;
    vector<ll> min_v, smin_v, min_c;
    vector<ll> sum, len, ladd, lval;

    void update_node_max(int k, ll x) {
        sum[k] += (x - max_v[k]) * max_c[k];
        if (max_v[k] == min_v[k]) {
            max_v[k] = min_v[k] = x;
        } else if (max_v[k] == smin_v[k]) {
            max_v[k] = smin_v[k] = x;
        } else {
            max_v[k] = x;
        }
        if (lval[k] != inf && x < lval[k]) lval[k] = x;
    }

    void update_node_min(int k, ll x) {
        sum[k] += (x - min_v[k]) * min_c[k];
        if (max_v[k] == min_v[k]) {
            max_v[k] = min_v[k] = x;
        } else if (smax_v[k] == min_v[k]) {
            min_v[k] = smax_v[k] = x;
        } else {
            min_v[k] = x;
        }
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
        if (max_v[k] < max_v[2 * k + 1]) update_node_max(2 * k + 1, max_v[k]);
        if (min_v[2 * k + 1] < min_v[k]) update_node_min(2 * k + 1, min_v[k]);
        if (max_v[k] < max_v[2 * k + 2]) update_node_max(2 * k + 2, max_v[k]);
        if (min_v[2 * k + 2] < min_v[k]) update_node_min(2 * k + 2, min_v[k]);
    }

    void update(int k) {
        sum[k] = sum[2 * k + 1] + sum[2 * k + 2];
        if (max_v[2 * k + 1] < max_v[2 * k + 2]) {
            max_v[k] = max_v[2 * k + 2];
            max_c[k] = max_c[2 * k + 2];
            smax_v[k] = max(max_v[2 * k + 1], smax_v[2 * k + 2]);
        } else if (max_v[2 * k + 1] > max_v[2 * k + 2]) {
            max_v[k] = max_v[2 * k + 1];
            max_c[k] = max_c[2 * k + 1];
            smax_v[k] = max(smax_v[2 * k + 1], max_v[2 * k + 2]);
        } else {
            max_v[k] = max_v[2 * k + 1];
            max_c[k] = max_c[2 * k + 1] + max_c[2 * k + 2];
            smax_v[k] = max(smax_v[2 * k + 1], smax_v[2 * k + 2]);
        }
        if (min_v[2 * k + 1] < min_v[2 * k + 2]) {
            min_v[k] = min_v[2 * k + 1];
            min_c[k] = min_c[2 * k + 1];
            smin_v[k] = min(smin_v[2 * k + 1], min_v[2 * k + 2]);
        } else if (min_v[2 * k + 1] > min_v[2 * k + 2]) {
            min_v[k] = min_v[2 * k + 2];
            min_c[k] = min_c[2 * k + 2];
            smin_v[k] = min(min_v[2 * k + 1], smin_v[2 * k + 2]);
        } else {
            min_v[k] = min_v[2 * k + 1];
            min_c[k] = min_c[2 * k + 1] + min_c[2 * k + 2];
            smin_v[k] = min(smin_v[2 * k + 1], smin_v[2 * k + 2]);
        }
    }

    void _update_min(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || max_v[k] <= x) return;
        if (a <= l && r <= b && smax_v[k] < x) {
            update_node_max(k, x);
            return;
        }
        push(k);
        _update_min(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_min(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }

    void _update_max(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a || x <= min_v[k]) return;
        if (a <= l && r <= b && x < smin_v[k]) {
            update_node_min(k, x);
            return;
        }
        push(k);
        _update_max(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_max(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }

    void addall(int k, ll x) {
        max_v[k] += x;
        if (smax_v[k] != -inf) smax_v[k] += x;
        min_v[k] += x;
        if (smin_v[k] != inf) smin_v[k] += x;
        sum[k] += len[k] * x;
        if (lval[k] != inf)
            lval[k] += x;
        else
            ladd[k] += x;
    }

    void updateall(int k, ll x) {
        max_v[k] = x;
        smax_v[k] = -inf;
        min_v[k] = x;
        smin_v[k] = inf;
        max_c[k] = min_c[k] = len[k];
        sum[k] = x * len[k];
        lval[k] = x;
        ladd[k] = 0;
    }

    void _add_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            addall(k, x);
            return;
        }
        push(k);
        _add_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _add_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }

    void _update_val(ll x, int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return;
        if (a <= l && r <= b) {
            updateall(k, x);
            return;
        }
        push(k);
        _update_val(x, a, b, 2 * k + 1, l, (l + r) / 2);
        _update_val(x, a, b, 2 * k + 2, (l + r) / 2, r);
        update(k);
    }

    ll _query_max(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return -inf;
        if (a <= l && r <= b) return max_v[k];
        push(k);
        return max(_query_max(a, b, 2 * k + 1, l, (l + r) / 2), _query_max(a, b, 2 * k + 2, (l + r) / 2, r));
    }

    ll _query_min(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return inf;
        if (a <= l && r <= b) return min_v[k];
        push(k);
        return min(_query_min(a, b, 2 * k + 1, l, (l + r) / 2), _query_min(a, b, 2 * k + 2, (l + r) / 2, r));
    }

    ll _query_sum(int a, int b, int k, int l, int r) {
        if (b <= l || r <= a) return 0;
        if (a <= l && r <= b) return sum[k];
        push(k);
        return _query_sum(a, b, 2 * k + 1, l, (l + r) / 2) + _query_sum(a, b, 2 * k + 2, (l + r) / 2, r);
    }

    SGTBeats(int n, ll *a) : n(n) {
        n0 = 1;
        while (n0 < n) n0 <<= 1;
        int m = 2 * n0;
        max_v.assign(m, 0), smax_v.assign(m, 0), max_c.assign(m, 0);
        min_v.assign(m, 0), smin_v.assign(m, 0), min_c.assign(m, 0);
        sum.assign(m, 0), len.assign(m, 0), ladd.assign(m, 0), lval.assign(m, inf);

        len[0] = n0;
        for (int i = 0; i < n0 - 1; ++i) len[2 * i + 1] = len[2 * i + 2] = (len[i] >> 1);
        for (int i = 0; i < n; ++i) {
            max_v[n0 - 1 + i] = min_v[n0 - 1 + i] = sum[n0 - 1 + i] = (a != nullptr ? a[i] : 0);
            smax_v[n0 - 1 + i] = -inf;
            smin_v[n0 - 1 + i] = inf;
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 1;
        }
        for (int i = n; i < n0; ++i) {
            max_v[n0 - 1 + i] = smax_v[n0 - 1 + i] = -inf;
            min_v[n0 - 1 + i] = smin_v[n0 - 1 + i] = inf;
            max_c[n0 - 1 + i] = min_c[n0 - 1 + i] = 0;
        }
        for (int i = n0 - 2; i >= 0; i--) update(i);
    }

    void update_min(int a, int b, ll x) { _update_min(x, a, b, 0, 0, n0); }  // a[i] = min(a[i], x) on [a, b)
    void update_max(int a, int b, ll x) { _update_max(x, a, b, 0, 0, n0); }  // a[i] = max(a[i], x) on [a, b)
    void add_val(int a, int b, ll x) { _add_val(x, a, b, 0, 0, n0); }        // a[i] += x on [a, b)
    void update_val(int a, int b, ll x) { _update_val(x, a, b, 0, 0, n0); }  // a[i] = x on [a, b)
    ll query_max(int a, int b) { return _query_max(a, b, 0, 0, n0); }        // max over [a, b)
    ll query_min(int a, int b) { return _query_min(a, b, 0, 0, n0); }        // min over [a, b)
    ll query_sum(int a, int b) { return _query_sum(a, b, 0, 0, n0); }        // sum over [a, b)
};

// Standard problem: type 0 chmin, 1 chmax, 2 range add, 3 range sum, all on the half-open range [l, r)
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (auto &x : a) cin >> x;
    SGTBeats t(n, a.data());
    while (q--) {
        int ty, l, r;
        cin >> ty >> l >> r;
        ll x;
        if (ty < 3) cin >> x;
        if (ty == 0)
            t.update_min(l, r, x);
        else if (ty == 1)
            t.update_max(l, r, x);
        else if (ty == 2)
            t.add_val(l, r, x);
        else
            cout << t.query_sum(l, r) << '\n';
    }
}
