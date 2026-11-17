// Test for 02_Range_Query/Sparse_Table.cpp - every public member against a naive scan.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/02_Range_Query/Sparse_Table.cpp -o t && ./t
// Options:
//   -DWRAP_INT_LL            put "#define int long long" before the template (how it is used in contests)
//   -DTEMPLATE_PATH='"f.cpp"' test another copy of the template instead of the installed one
//   -DAFFINE                 query_any() with a non-commutative, non-idempotent merge (composition of affine maps),
//                            which catches overlapping, missing and out-of-order blocks that min() hides.
//                            The template's merge() is hard-coded, so make a copy with merge/none swapped:
//     sed -e 's/T none = numeric_limits<T>::max();/T none = AFF_ID;/' -e 's/return min(a, b);/return aff_merge(a, b);/'
//         02_Range_Query/Sparse_Table.cpp > /tmp/sparse_affine.cpp
//     g++ -std=c++17 -O1 -g -fsanitize=address,undefined -DAFFINE -DTEMPLATE_PATH='"/tmp/sparse_affine.cpp"'
//         tests/02_Range_Query/Sparse_Table.cpp -o t && ./t
//   (each command is one line, wrapped here). In this mode query() is not tested: it is only for idempotent merges.

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
#include "../../02_Range_Query/Sparse_Table.cpp"
#endif

static map<string, pair<long long, long long>> cov; // method -> {checks, mismatches}
static void check(const string &method, bool ok) {
    cov[method].first++;
    if (!ok) cov[method].second++;
}

static mt19937_64 rng(20261117);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

#ifdef AFFINE
static void run_affine() {
    {
        SparseTable<long long> probe(vector<long long>{AFF_ID});
        bool non_commutative_seen = false;
        for (int it = 0; it < 2000; it++) {
            long long x = aff_pack(rnd(1, P - 1), rnd(0, P - 1)), y = aff_pack(rnd(1, P - 1), rnd(0, P - 1));
            check("merge", probe.merge(x, y) == aff_merge(x, y));
            if (probe.merge(x, y) != probe.merge(y, x)) non_commutative_seen = true;
        }
        check("none", probe.none == AFF_ID);
        check("merge_is_non_commutative", non_commutative_seen);
    }
    for (int trial = 0; trial < 3000; trial++) {
        int m = rnd(1, trial < 300 ? 8 : 70);
        vector<long long> a(m);
        for (auto &x : a) x = aff_pack(rnd(1, P - 1), rnd(0, P - 1));
        SparseTable<long long> st(a);
        for (int l = 0; l < m; l++) {
            long long acc = AFF_ID;
            for (int r = l; r < m; r++) {
                acc = aff_merge(acc, a[r]);
                check("query_any", st.query_any(l, r) == acc);
            }
        }
    }
}
#else
template <typename T> static void run(const string &tag, T lo, T hi) {
    auto name = [&](const char *m) { return tag + " " + m; };
    {
        SparseTable<T> probe(vector<T>{T(0)});
        for (int it = 0; it < 2000; it++) {
            T x = rnd(lo, hi), y = rnd(lo, hi);
            check(name("merge"), probe.merge(x, y) == min(x, y));
        }
        check(name("none"), probe.none == numeric_limits<T>::max());
    }
    {
        SparseTable<T> empty(vector<T>{}); // n = 0 must construct without touching memory
        check(name("SparseTable(empty)"), empty.n == 0);
    }
    for (int trial = 0; trial < 4000; trial++) {
        int m = rnd(1, trial < 300 ? 8 : 70);
        vector<T> a(m);
        int spread = rnd(0, 2); // 0: few distinct values (ties), 1: full range, 2: include the type's extremes
        for (auto &x : a) {
            if (spread == 0) x = rnd(-3, 3);
            else if (spread == 1) x = rnd(lo, hi);
            else x = rnd(0, 3) == 0 ? (rnd(0, 1) ? numeric_limits<T>::min() : numeric_limits<T>::max()) : (T)rnd(lo, hi);
        }
        SparseTable<T> st(a);
        check(name("SparseTable(vector)"), st.n == m);
        for (int l = 0; l < m; l++) {
            T mn = a[l];
            for (int r = l; r < m; r++) {
                mn = min(mn, a[r]);
                check(name("query"), st.query(l, r) == mn);
                check(name("query_any"), st.query_any(l, r) == mn);
            }
        }
    }
}
#endif

signed main() {
#ifdef AFFINE
    run_affine();
#else
    run<int>("int", -1000000000, 1000000000);
    run<long long>("ll", -(long long)4e18, (long long)4e18);

    // Large n timing (2e5): build + 2e5 query + 2e5 query_any, spot-checked with a naive scan.
    {
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        vector<long long> a(N);
        for (auto &x : a) x = rnd(-(long long)1e18, (long long)1e18);
        SparseTable<long long> st(a);
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            int l = rnd(0, N - 1), r = rnd(l, N - 1);
            sink ^= st.query(l, r) ^ st.query_any(l, r);
            if (it % 2000 == 0) {
                long long mn = *min_element(a.begin() + l, a.begin() + r + 1);
                check("ll query_large", st.query(l, r) == mn);
                check("ll query_any_large", st.query_any(l, r) == mn);
            }
        }
        long long mn = *min_element(a.begin(), a.end());
        check("ll query_large", st.query(0, N - 1) == mn);
        check("ll query_any_large", st.query_any(0, N - 1) == mn);
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=%lld: build + %lld query + %lld query_any in %.1f ms (sink %lld)\n", (long long)N, (long long)N, (long long)N, ms, sink & 1);
    }
#endif

    bool pass = true;
    for (auto &[method, cm] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", method.c_str(), cm.first, cm.second);
        if (cm.first == 0 || cm.second != 0) pass = false;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
