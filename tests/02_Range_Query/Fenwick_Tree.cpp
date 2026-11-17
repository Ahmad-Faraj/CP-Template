// Test for 02_Range_Query/Fenwick_Tree.cpp - every public method, 1-based and 0-based, against a naive array.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/02_Range_Query/Fenwick_Tree.cpp -o t && ./t
// Options:
//   -DWRAP_INT_LL            put "#define int long long" before the template (how it is used in contests)
//   -DTEMPLATE_PATH='"f.cpp"' test another copy of the template instead of the installed one

#include <bits/stdc++.h>
using namespace std;
#ifdef WRAP_INT_LL
#define int long long
#endif

#ifdef TEMPLATE_PATH
#include TEMPLATE_PATH
#else
#include "../../02_Range_Query/Fenwick_Tree.cpp"
#endif

static map<string, pair<long long, long long>> cov; // method -> {checks, mismatches}
static void check(const string &method, bool ok) {
    cov[method].first++;
    if (!ok) cov[method].second++;
}

static mt19937_64 rng(20261117);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

// Oracle: plain vector p[0 .. m-1]; the tree's index for p[i] is i + OB (OB = 1 for 1-based, 0 for 0-based).
static long long prefix(const vector<long long> &p, long long upto) { // p[0 .. upto], upto may be -1
    long long s = 0;
    for (long long i = 0; i <= upto; i++) s += p[i];
    return s;
}
static long long naive_lower(const vector<long long> &p, long long x, long long OB) { // first prefix >= x
    long long s = 0;
    for (size_t i = 0; i < p.size(); i++) {
        s += p[i];
        if (s >= x) return i + OB;
    }
    return (long long)p.size() + OB;
}
static long long naive_upper(const vector<long long> &p, long long x, long long OB) { // first prefix > x
    long long s = 0;
    for (size_t i = 0; i < p.size(); i++) {
        s += p[i];
        if (s > x) return i + OB;
    }
    return (long long)p.size() + OB;
}

template <bool OB> static void run(const string &tag) {
    auto name = [&](const char *m) { return tag + " " + m; };

    // Point model: add / set / get / sum(idx) / sum(l, r), random signed values.
    for (int trial = 0; trial < 3000; trial++) {
        int m = rnd(0, trial < 300 ? 6 : 50);
        vector<long long> p(m);
        bool from_vector = rnd(0, 1);
        if (from_vector)
            for (auto &x : p) x = rnd(-1000000000LL, 1000000000LL);
        FenwickTree<OB> ft = from_vector ? FenwickTree<OB>(p) : FenwickTree<OB>(m);
        for (int i = 0; i < m; i++) check(name(from_vector ? "FenwickTree(vector)" : "FenwickTree(m)"), ft.get(i + OB) == p[i]);
        check(name("sum(idx)"), ft.sum(OB - 1) == 0); // empty prefix
        int ops = m ? rnd(1, 80) : 0;
        for (int op = 0; op < ops; op++) {
            int t = rnd(0, 4), i = rnd(0, m - 1);
            if (t == 0) {
                long long d = rnd(-1000000000LL, 1000000000LL);
                p[i] += d;
                ft.add(i + OB, d);
                check(name("add"), ft.get(i + OB) == p[i]);
            } else if (t == 1) {
                long long v = rnd(-1000000000LL, 1000000000LL);
                p[i] = v;
                ft.set(i + OB, v);
                check(name("set"), ft.get(i + OB) == p[i]);
            } else if (t == 2) {
                check(name("get"), ft.get(i + OB) == p[i]);
            } else if (t == 3) {
                check(name("sum(idx)"), ft.sum(i + OB) == prefix(p, i));
            } else {
                int l = rnd(0, m - 1), r = rnd(l, m - 1);
                check(name("sum(l, r)"), ft.sum(l + OB, r + OB) == prefix(p, r) - prefix(p, l - 1));
            }
        }
        for (int l = 0; l < m; l++) {
            check(name("get"), ft.get(l + OB) == p[l]);
            check(name("sum(idx)"), ft.sum(l + OB) == prefix(p, l));
            for (int r = l; r < m; r++) check(name("sum(l, r)"), ft.sum(l + OB, r + OB) == prefix(p, r) - prefix(p, l - 1));
        }
    }

    // lower_bound / upper_bound: non-negative values (zeros frequent, so ties and flat runs appear).
    for (int trial = 0; trial < 3000; trial++) {
        int m = rnd(0, trial < 300 ? 6 : 50);
        vector<long long> p(m);
        for (auto &x : p) x = rnd(0, 2) ? rnd(0, 5) : 0;
        FenwickTree<OB> ft(p);
        int rounds = rnd(1, 20);
        for (int rd = 0; rd < rounds; rd++) {
            if (m && rnd(0, 1)) { // keep values non-negative while changing them
                int i = rnd(0, m - 1);
                if (rnd(0, 1)) {
                    long long d = rnd(0, 4);
                    p[i] += d, ft.add(i + OB, d);
                } else {
                    long long v = rnd(0, 6);
                    p[i] = v, ft.set(i + OB, v);
                }
            }
            long long total = prefix(p, m - 1);
            for (long long x = -2; x <= total + 2; x++) {
                check(name("lower_bound"), ft.lower_bound(x) == naive_lower(p, x, OB));
                check(name("upper_bound"), ft.upper_bound(x) == naive_upper(p, x, OB));
            }
        }
    }

    // update_range: the tree becomes a difference array, point value = sum(idx).
    for (int trial = 0; trial < 3000; trial++) {
        int m = rnd(1, trial < 300 ? 6 : 50);
        vector<long long> p(m, 0);
        bool from_vector = rnd(0, 1);
        FenwickTree<OB> ft(m);
        if (from_vector) { // start from given values: build from their difference array
            vector<long long> diff(m);
            for (int i = 0; i < m; i++) p[i] = rnd(-1000, 1000);
            for (int i = 0; i < m; i++) diff[i] = p[i] - (i ? p[i - 1] : 0);
            ft = FenwickTree<OB>(diff);
        }
        int ops = rnd(1, 40);
        for (int op = 0; op < ops; op++) {
            int l = rnd(0, m - 1), r = rnd(0, 3) ? rnd(l, m - 1) : m - 1; // r = last index often
            long long v = rnd(-1000000000LL, 1000000000LL);
            for (int i = l; i <= r; i++) p[i] += v;
            ft.update_range(l + OB, r + OB, v);
            for (int i = 0; i < m; i++) check(name("update_range"), ft.sum(i + OB) == p[i]);
        }
    }
}

signed main() {
    run<true>("1-based");
    run<false>("0-based");

    // Large n timing (2e5): build from vector, 2e5 x (add + set + sum(l, r) + lower_bound).
    {
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        vector<long long> p(N);
        for (auto &x : p) x = rnd(0, 1000000);
        FenwickTree<> ft(p);
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            int i = rnd(0, N - 1);
            long long d = rnd(0, 1000);
            p[i] += d, ft.add(i + 1, d);
            int j = rnd(0, N - 1);
            p[j] = rnd(0, 1000000), ft.set(j + 1, p[j]);
            int l = rnd(1, N), r = rnd(l, N);
            sink ^= ft.sum(l, r);
            sink ^= ft.lower_bound(rnd(0, 1000000000LL));
        }
        vector<long long> pre(N + 1, 0);
        for (int i = 0; i < N; i++) pre[i + 1] = pre[i] + p[i];
        for (int it = 0; it < 1000; it++) {
            int l = rnd(1, N), r = rnd(l, N);
            check("1-based sum_large", ft.sum(l, r) == pre[r] - pre[l - 1]);
            long long x = rnd(0, pre[N] + 1);
            check("1-based lower_bound_large", ft.lower_bound(x) == (long long)(std::lower_bound(pre.begin() + 1, pre.end(), x) - pre.begin()));
        }
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=%lld: build + 4 x %lld operations in %.1f ms (sink %lld)\n", (long long)N, (long long)N, ms, sink & 1);
    }

    bool pass = true;
    for (auto &[method, cm] : cov) {
        printf("coverage %-30s checks=%-9lld mismatches=%lld\n", method.c_str(), cm.first, cm.second);
        if (cm.first == 0 || cm.second != 0) pass = false;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
