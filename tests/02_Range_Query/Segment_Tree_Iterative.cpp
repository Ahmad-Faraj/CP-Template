// Test for 02_Range_Query/Segment_Tree_Iterative.cpp - every public member against a naive oracle.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/02_Range_Query/Segment_Tree_Iterative.cpp -o t && ./t
// Options:
//   -DWRAP_INT_LL            put "#define int long long" before the template (how it is used in contests)
//   -DTEMPLATE_PATH='"f.cpp"' test another copy of the template instead of the installed one
//   -DAFFINE                 non-commutative merge (composition of affine maps). The template's merge() is
//                            hard-coded, so make a copy with merge/none swapped and pass it as TEMPLATE_PATH:
//     sed -e 's/ll none = 0;/ll none = AFF_ID;/' -e 's/return a + b;/return aff_merge(a, b);/'
//         02_Range_Query/Segment_Tree_Iterative.cpp > /tmp/seg_affine.cpp
//     g++ -std=c++17 -O1 -g -fsanitize=address,undefined -DAFFINE -DTEMPLATE_PATH='"/tmp/seg_affine.cpp"'
//         tests/02_Range_Query/Segment_Tree_Iterative.cpp -o t && ./t
//   (each command is one line, wrapped here)
//   The sum merge cannot see the order of the operands; the affine merge can.

#include <bits/stdc++.h>
using namespace std;
#ifdef WRAP_INT_LL
#define int long long
#endif

#ifdef AFFINE
// x -> A*x + B (mod P), packed as A << 32 | B. aff_merge(f, g) = apply f, then g.
const long long P = 1000000007LL;
const long long AFF_ID = 1LL << 32;
long long aff_pack(long long A, long long B) { return (A << 32) | B; }
long long aff_merge(long long f, long long g) {
    long long fa = f >> 32, fb = f & 0xFFFFFFFFLL, ga = g >> 32, gb = g & 0xFFFFFFFFLL;
    return aff_pack(fa * ga % P, (ga * fb + gb) % P);
}
#endif

#ifdef TEMPLATE_PATH
#include TEMPLATE_PATH
#else
#include "../../02_Range_Query/Segment_Tree_Iterative.cpp"
#endif

static map<string, pair<long long, long long>> cov; // method -> {checks, mismatches}
static void check(const string &method, bool ok) {
    cov[method].first++;
    if (!ok) cov[method].second++;
}

static mt19937_64 rng(20261117);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

static long long oracle_identity() {
#ifdef AFFINE
    return AFF_ID;
#else
    return 0;
#endif
}
static long long oracle_merge(long long a, long long b) {
#ifdef AFFINE
    return aff_merge(a, b);
#else
    return a + b;
#endif
}
static long long random_value() {
#ifdef AFFINE
    return aff_pack(rnd(1, P - 1), rnd(0, P - 1));
#else
    return rnd(-1000000000LL, 1000000000LL);
#endif
}
static long long naive(const vector<long long> &a, long long l, long long r) { // left-to-right fold
    long long ret = oracle_identity();
    for (long long i = l; i <= r; i++) ret = oracle_merge(ret, a[i]);
    return ret;
}

static void check_all_ranges(SegTree &st, const vector<long long> &a) {
    long long m = a.size();
    for (long long l = 0; l < m; l++)
        for (long long r = l; r < m; r++) check("query", st.query(l, r) == naive(a, l, r));
}

signed main() {
    // merge(): the template's merge must equal the oracle's merge, and (AFFINE) must really be non-commutative.
    {
        SegTree probe(1);
        bool non_commutative_seen = false;
        for (int it = 0; it < 2000; it++) {
            long long x = random_value(), y = random_value();
            check("merge", probe.merge(x, y) == oracle_merge(x, y));
            check("merge", probe.merge(probe.none, x) == x && probe.merge(x, probe.none) == x);
            if (probe.merge(x, y) != probe.merge(y, x)) non_commutative_seen = true;
        }
        check("none", probe.none == oracle_identity());
#ifdef AFFINE
        check("merge_is_non_commutative", non_commutative_seen);
#else
        (void)non_commutative_seen;
#endif
    }

    // SegTree(m) alone: every range of a fresh tree is the identity; then updates without build().
    for (int m = 1; m <= 40; m++) {
        SegTree st(m);
        vector<long long> a(m, oracle_identity());
        for (int l = 0; l < m; l++)
            for (int r = l; r < m; r++) check("SegTree(m)", st.query(l, r) == oracle_identity());
        for (int it = 0; it < 3 * m; it++) {
            int i = rnd(0, m - 1);
            a[i] = random_value();
            st.update(i, a[i]);
            check("update", st.query(i, i) == a[i]);
        }
        check_all_ranges(st, a);
    }

    // Random trials: build() from a full or partial vector, then mixed update / query, then build() again.
    for (int trial = 0; trial < 4000; trial++) {
        int m = rnd(1, trial < 200 ? 8 : 64);
        SegTree st(m);
        int given = rnd(0, 3) ? m : rnd(0, m); // sometimes build from a shorter vector
        vector<long long> a(m, oracle_identity()), init(given);
        for (int i = 0; i < given; i++) a[i] = init[i] = random_value();
        st.build(init);
        for (int l = 0; l < m; l++) check("build", st.query(l, l) == a[l]);
        if (m <= 16) check_all_ranges(st, a);
        int ops = rnd(1, 60);
        for (int op = 0; op < ops; op++) {
            if (rnd(0, 2) == 0) {
                int i = rnd(0, m - 1);
                a[i] = random_value();
                st.update(i, a[i]);
                check("update", st.query(i, i) == a[i]);
            } else {
                int l = rnd(0, m - 1), r = rnd(l, m - 1);
                check("query", st.query(l, r) == naive(a, l, r));
            }
        }
        check("query", st.query(0, m - 1) == naive(a, 0, m - 1));
        if (rnd(0, 3) == 0) { // rebuild over a tree that already had updates
            for (int i = 0; i < m; i++) a[i] = random_value();
            st.build(a);
            check_all_ranges(st, a);
            check("build", true);
        }
    }

    // Large n timing (2e5): build + 2e5 updates + 2e5 queries, spot-checked against prefix folds.
    {
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        vector<long long> a(N);
        for (auto &x : a) x = random_value();
        SegTree st(N);
        st.build(a);
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            int i = rnd(0, N - 1);
            a[i] = random_value();
            st.update(i, a[i]);
            int l = rnd(0, N - 1), r = rnd(l, min(N - 1, l + 50));
            long long got = st.query(l, r);
            sink ^= got;
            if (it % 1000 == 0) check("query_large", got == naive(a, l, r));
        }
        check("query_large", st.query(0, N - 1) == naive(a, 0, N - 1));
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=%lld: build + updates + queries (%lld each) in %.1f ms (sink %lld)\n", (long long)N, (long long)N, ms, sink & 1);
    }

    bool pass = true;
    for (auto &[method, cm] : cov) {
        printf("coverage %-26s checks=%-9lld mismatches=%lld\n", method.c_str(), cm.first, cm.second);
        if (cm.first == 0 || cm.second != 0) pass = false;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
