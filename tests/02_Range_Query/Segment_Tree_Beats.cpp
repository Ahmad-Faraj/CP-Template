// Test for 02_Range_Query/Segment_Tree_Beats.cpp against a plain array updated by naive loops.
// Every public method is checked: constructor, update_min, update_max, add_val, update_val,
// query_sum, query_min, query_max (including empty ranges), on half-open [l, r).
#include "../../02_Range_Query/Segment_Tree_Beats.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(4242);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); }

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = it < 200 ? it % 4 : (int)rnd(1, it % 3 ? 12 : 70);
        ll V = vector<ll>{2, 5, 30, 1000, (ll)1e12}[rng() % 5];
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-V, V);
        SGTBeats t(a);
        int q = (int)rnd(1, 60);
        for (int s = 0; s < q; s++) {
            int l = (int)rnd(0, n), r = (int)rnd(0, n);
            if (l > r) swap(l, r);
            ll x = rnd(-V, V);
            int ty = (int)rnd(0, 6);
            if (ty == 0) {
                t.update_min(l, r, x);
                for (int i = l; i < r; i++) a[i] = min(a[i], x);
            } else if (ty == 1) {
                t.update_max(l, r, x);
                for (int i = l; i < r; i++) a[i] = max(a[i], x);
            } else if (ty == 2) {
                x = rnd(-V / 2, V / 2);
                t.add_val(l, r, x);
                for (int i = l; i < r; i++) a[i] += x;
            } else if (ty == 3) {
                t.update_val(l, r, x);
                for (int i = l; i < r; i++) a[i] = x;
            } else {
                ll s0 = 0, mx = -SGTBeats::inf, mn = SGTBeats::inf;
                for (int i = l; i < r; i++) s0 += a[i], mx = max(mx, a[i]), mn = min(mn, a[i]);
                string tag = l == r ? " (empty)" : "";
                chk("query_sum" + tag, t.query_sum(l, r) == s0);
                chk("query_max" + tag, t.query_max(l, r) == mx);
                chk("query_min" + tag, t.query_min(l, r) == mn);
            }
            const char *op[] = {"update_min", "update_max", "add_val", "update_val"};
            if (ty <= 3) chk(op[ty], true); // counted here, verified by the queries that follow
        }
        // full sweep of every single point and every prefix after the operations
        bool ok = true;
        ll pre = 0;
        for (int i = 0; i < n; i++) {
            pre += a[i];
            ok &= t.query_sum(i, i + 1) == a[i] && t.query_max(i, i + 1) == a[i] &&
                  t.query_min(i, i + 1) == a[i] && t.query_sum(0, i + 1) == pre;
        }
        chk("final point/prefix sweep", ok);
    }
    // attribute failures: one update kind at a time, each followed by a query
    const char *nm[] = {"only update_min", "only update_max", "only add_val", "only update_val"};
    for (int kind = 0; kind < 4; kind++) {
        for (int it = 0; it < 3000; it++) {
            int n = (int)rnd(1, 40);
            vector<ll> a(n);
            for (auto &x : a) x = rnd(-20, 20);
            SGTBeats t(a);
            bool ok = true;
            for (int s = 0; s < 30; s++) {
                int l = (int)rnd(0, n), r = (int)rnd(0, n);
                if (l > r) swap(l, r);
                ll x = rnd(-20, 20);
                if (kind == 0) t.update_min(l, r, x);
                if (kind == 1) t.update_max(l, r, x);
                if (kind == 2) t.add_val(l, r, x);
                if (kind == 3) t.update_val(l, r, x);
                for (int i = l; i < r; i++)
                    a[i] = kind == 0   ? min(a[i], x)
                           : kind == 1 ? max(a[i], x)
                           : kind == 2 ? a[i] + x
                                       : x;
                int ql = (int)rnd(0, n - 1), qr = (int)rnd(ql + 1, n);
                ll s0 = 0, mx = LLONG_MIN, mn = LLONG_MAX;
                for (int i = ql; i < qr; i++) s0 += a[i], mx = max(mx, a[i]), mn = min(mn, a[i]);
                ok &= t.query_sum(ql, qr) == s0 && t.query_max(ql, qr) == mx &&
                      t.query_min(ql, qr) == mn;
            }
            chk(nm[kind], ok);
        }
    }
    {
        int n = 200000, q = 200000;
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-1000000000, 1000000000);
        auto t0 = clock();
        SGTBeats t(a);
        ll sink = 0;
        for (int s = 0; s < q; s++) {
            int l = (int)rnd(0, n - 1), r = (int)rnd(l + 1, n);
            ll x = rnd(-1000000000, 1000000000);
            int ty = s % 5;
            if (ty == 0) t.update_min(l, r, x);
            else if (ty == 1) t.update_max(l, r, x);
            else if (ty == 2) t.add_val(l, r, x / 1000);
            else if (ty == 3) sink += t.query_sum(l, r);
            else sink += t.query_max(l, r) ^ t.query_min(l, r);
        }
        printf("n=q=2e5 mixed chmin/chmax/add/sum/max/min: %.3fs (sink %lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, sink & 1);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first,
               c.second);
        ok &= c.first > 0 && c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
