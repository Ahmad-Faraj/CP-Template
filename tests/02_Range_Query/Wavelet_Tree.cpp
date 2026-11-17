// Test for 02_Range_Query/Wavelet_Tree.cpp against sorting / scanning the subarray.
// Every public method is checked on EVERY [l, r] of each small array: init (n = 0 .. 20, tight,
// wide and full-int value ranges), kth (every k), LTE, count, sum (thresholds at, between and
// outside the values), and the empty range l > r.
#include "../../02_Range_Query/Wavelet_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(31337);
void chk_n(const string &m, long long checks, long long bad) {
    cov[m].first += checks, cov[m].second += bad;
}
ll rnd(ll lo, ll hi) { return uniform_int_distribution<ll>(lo, hi)(rng); }

int main() {
    for (int it = 0; it < 3000; it++) {
        int n = it < 21 ? it : (int)rnd(1, 20);
        int mode = it % 4;
        // value range given to init, and the range values are drawn from
        ll LO, HI, vlo, vhi;
        if (mode == 0) LO = -3, HI = 3, vlo = -3, vhi = 3;               // tight, many duplicates
        else if (mode == 1) LO = -1000000009, HI = 1000000009, vlo = -50, vhi = 50;
        else if (mode == 2) LO = -1000000009, HI = 1000000009, vlo = -1e9, vhi = 1e9;
        else LO = INT_MIN, HI = INT_MAX, vlo = INT_MIN, vhi = INT_MAX; // full int range
        vector<int> a(n);
        for (auto &x : a) x = (int)rnd(vlo, vhi);
        if (mode == 3)
            for (auto &x : a)
                if (rng() % 4 == 0) x = rng() % 2 ? INT_MIN : INT_MAX;
        vector<int> work = a; // init() reorders its input
        wavelet_tree t;
        t.init(work.data(), work.data() + n, (int)LO, (int)HI);

        // thresholds: every value, its neighbours, the bounds, and outside the bounds
        vector<ll> ths = {LO, HI, (LO + HI) / 2};
        for (int x : a) ths.push_back(x), ths.push_back((ll)x - 1), ths.push_back((ll)x + 1);
        for (int s = 0; s < 3; s++) ths.push_back(rnd(LO, HI));
        if (mode != 3) ths.push_back(LO - 5), ths.push_back(HI + 5);
        vector<int> th;
        for (ll x : ths)
            if (x >= INT_MIN && x <= INT_MAX) th.push_back((int)x);

        ll ck[4] = {}, bd[4] = {};
        for (int l = 1; l <= n; l++)
            for (int r = l; r <= n; r++) {
                vector<int> sub(a.begin() + l - 1, a.begin() + r);
                sort(sub.begin(), sub.end());
                for (int k = 1; k <= r - l + 1; k++) ck[0]++, bd[0] += t.kth(l, r, k) != sub[k - 1];
                for (int x : th) {
                    ll lte = 0, eq = 0, sm = 0;
                    for (int v : sub) lte += v <= x, eq += v == x, sm += v <= x ? v : 0;
                    ck[1]++, bd[1] += t.LTE(l, r, x) != lte;
                    ck[2]++, bd[2] += t.count(l, r, x) != eq;
                    ck[3]++, bd[3] += t.sum(l, r, x) != sm;
                }
            }
        chk_n("kth", ck[0], bd[0]);
        chk_n("LTE", ck[1], bd[1]);
        chk_n("count", ck[2], bd[2]);
        chk_n("sum", ck[3], bd[3]);
        ll eb = 0, ec = 0; // empty ranges l > r, including n = 0
        for (int l = 1; l <= n + 1; l++) {
            int r = l - 1, x = (int)rnd(vlo, vhi);
            ec += 4;
            eb += (t.kth(l, r, 1) != 0) + (t.LTE(l, r, x) != 0) + (t.count(l, r, x) != 0) +
                  (t.sum(l, r, x) != 0);
        }
        chk_n("empty range (all four)", ec, eb);
    }
    {
        int n = 300000, q = 300000;
        vector<int> a(n);
        for (auto &x : a) x = (int)rnd(-1e9, 1e9);
        vector<int> work = a;
        auto t0 = clock();
        wavelet_tree t;
        t.init(work.data(), work.data() + n, -1000000009, 1000000009);
        double tb = double(clock() - t0) / CLOCKS_PER_SEC;
        ll sink = 0;
        for (int i = 0; i < q; i++) {
            int l = (int)rnd(1, n), r = (int)rnd(l, n), k = (int)rnd(1, r - l + 1);
            int x = (int)rnd(-1e9, 1e9);
            sink += t.kth(l, r, k) + t.LTE(l, r, x) + t.count(l, r, a[l - 1]) + t.sum(l, r, x);
        }
        printf("timing n=3e5: build %.3fs, 3e5 x (kth + LTE + count + sum) %.3fs (sink %lld)\n",
               tb, double(clock() - t0) / CLOCKS_PER_SEC - tb, sink & 1);
        ll bad = 0; // spot-check the big tree
        for (int i = 0; i < 200; i++) {
            int l = (int)rnd(1, n), r = (int)rnd(l, min(n, l + 3000)), x = (int)rnd(-1e9, 1e9);
            vector<int> sub(a.begin() + l - 1, a.begin() + r);
            sort(sub.begin(), sub.end());
            int k = (int)rnd(1, r - l + 1);
            ll sm = 0;
            for (int v : sub) sm += v <= x ? v : 0;
            bad += t.kth(l, r, k) != sub[k - 1] || t.sum(l, r, x) != sm ||
                   t.LTE(l, r, x) != upper_bound(sub.begin(), sub.end(), x) - sub.begin();
        }
        chk_n("n=3e5 spot checks (kth, sum, LTE)", 200, bad);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first,
               c.second);
        ok &= c.first > 0 && c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
