// Test for 02_Range_Query/Segment_Tree.cpp - every public member against a naive array.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/02_Range_Query/Segment_Tree.cpp -o t && ./t
// Options:
//   -DWRAP_INT_LL             put "#define int long long" before the template (contest usage)
//   -DTEMPLATE_PATH='"f.cpp"' test another copy of the template
//   -DAFFINE                  non-commutative merge (composition of affine maps). merge() is hard-coded,
//                             so make a copy with merge/Default swapped and pass it as TEMPLATE_PATH:
//     sed -e 's/Default = 0;/Default = AFF_ID;/' -e 's/Node res = a.val + b.val;/Node res = aff_merge(a.val, b.val);/'
//         02_Range_Query/Segment_Tree.cpp > /tmp/seg_affine.cpp
//     g++ -std=c++17 -O2 -DAFFINE -DTEMPLATE_PATH='"/tmp/seg_affine.cpp"' tests/02_Range_Query/Segment_Tree.cpp
//   (each command is one line, wrapped here). A sum cannot see operand order; the affine merge can.

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
#include "../../02_Range_Query/Segment_Tree.cpp"
#endif

static map<string, pair<long long, long long>> cov; // method -> {checks, mismatches}
static void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
static mt19937_64 rng(20261118);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

static long long ident() {
#ifdef AFFINE
    return AFF_ID;
#else
    return 0;
#endif
}
static long long op(long long a, long long b) {
#ifdef AFFINE
    return aff_merge(a, b);
#else
    return a + b;
#endif
}
static long long rval() {
#ifdef AFFINE
    return aff_pack(rnd(1, P - 1), rnd(0, P - 1));
#else
    return rng() % 3 ? rnd(-1000000000LL, 1000000000LL) : rnd(-3, 3);
#endif
}
// a is 1-based (a[0] unused); left-to-right fold
static long long naive(const vector<long long> &a, long long l, long long r) {
    long long x = ident();
    for (long long i = l; i <= r; i++) x = op(x, a[i]);
    return x;
}
static void all_ranges(SegTree &st, const vector<long long> &a, const string &tag) {
    long long n = (long long)a.size() - 1;
    for (long long l = 1; l <= n; l++)
        for (long long r = l; r <= n; r++) chk(tag, st.query(l, r) == naive(a, l, r));
}

signed main() {
    { // merge() and Default
        SegTree st;
        st.init(1);
        bool noncomm = false;
        for (int it = 0; it < 2000; it++) {
            SegTree::Node x(rval()), y(rval()), d = st.Default;
            chk("merge", st.merge(x, y).val == op(x.val, y.val));
            chk("Default", st.merge(d, x).val == x.val && st.merge(x, d).val == x.val);
            noncomm |= st.merge(x, y).val != st.merge(y, x).val;
        }
#ifdef AFFINE
        chk("merge_non_commutative", noncomm);
#else
        (void)noncomm;
#endif
    }
    // init(n) alone, then updates only; l > r returns Default
    for (int n = 1; n <= 40; n++) {
        SegTree st;
        st.init(n);
        vector<long long> a(n + 1, ident());
        all_ranges(st, a, "init");
        for (int it = 0; it < 3 * n; it++) {
            int i = rnd(1, n);
            a[i] = rval();
            st.update(i, a[i]);
            chk("update", st.query(i, i) == a[i]);
        }
        all_ranges(st, a, "query");
        for (int l = 2; l <= n; l++) chk("query(l>r)", st.query(l, l - 1) == ident());
    }
    // random trials: build (full or partial), mixed updates / queries, full rebuild
    for (int trial = 0; trial < 4000; trial++) {
        int n = rnd(1, trial < 300 ? 8 : 70);
        SegTree st;
        st.init(n);
        int given = rnd(0, 3) ? n : rnd(0, n);
        vector<long long> a(n + 1, ident()), init(given);
        for (int i = 0; i < given; i++) a[i + 1] = init[i] = rval();
        st.build(init);
        if (n <= 20) all_ranges(st, a, "build");
        else
            for (int i = 1; i <= n; i++) chk("build", st.query(i, i) == a[i]);
        int ops = rnd(1, 80);
        while (ops--) {
            if (rnd(0, 2) == 0) {
                int i = rnd(1, n);
                a[i] = rng() % 5 ? rval() : a[rnd(1, n)]; // duplicates sometimes
                st.update(i, a[i]);
                chk("update", st.query(i, i) == a[i]);
            } else {
                int l = rnd(1, n), r = rnd(l, n);
                chk("query", st.query(l, r) == naive(a, l, r));
            }
        }
        chk("query", st.query(1, n) == naive(a, 1, n));
        if (rnd(0, 3) == 0) {
            vector<long long> full(n);
            for (int i = 0; i < n; i++) a[i + 1] = full[i] = rval();
            st.build(full);
            all_ranges(st, a, "build(rebuild)");
        }
    }
#ifndef AFFINE
    { // CSES 1648 Dynamic Range Sum Queries, official sample: expect 14 2 11
        SegTree st;
        st.init(8);
        st.build({3, 2, 4, 5, 1, 1, 5, 3});
        vector<long long> got;
        got.push_back(st.query(1, 4));
        got.push_back(st.query(5, 6));
        st.update(3, 1);
        got.push_back(st.query(1, 4));
        chk("samples", got == vector<long long>{14, 2, 11});
    }
#endif
    { // timing, n = q = 2e5
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        vector<long long> a(N + 1), init(N);
        for (int i = 0; i < N; i++) a[i + 1] = init[i] = rval();
        SegTree st;
        st.init(N);
        st.build(init);
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            int i = rnd(1, N);
            a[i] = rval();
            st.update(i, a[i]);
            int l = rnd(1, N), r = rnd(l, N);
            long long got = st.query(l, r);
            sink ^= got;
            if (it % 2000 == 0) chk("query_large", got == naive(a, l, r));
        }
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=q=%lld: build + n updates + n queries in %.1f ms (sink %lld)\n", (long long)N, ms,
               sink & 1);
    }
    bool pass = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        pass &= c.first > 0 && c.second == 0;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
