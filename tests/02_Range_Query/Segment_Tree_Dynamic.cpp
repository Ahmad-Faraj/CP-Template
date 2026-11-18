// Test for 02_Range_Query/Segment_Tree_Dynamic.cpp - every public member against a std::map oracle.
//
// Run (from the repo root):
//   g++ -std=c++17 -O1 -g -fsanitize=address,undefined tests/02_Range_Query/Segment_Tree_Dynamic.cpp -o t
// Options: -DWRAP_INT_LL (template after "#define int long long"), -DTEMPLATE_PATH='"f.cpp"'.

#include <bits/stdc++.h>
using namespace std;
#ifdef WRAP_INT_LL
#define int long long
#endif
#ifdef TEMPLATE_PATH
#include TEMPLATE_PATH
#else
#include "../../02_Range_Query/Segment_Tree_Dynamic.cpp"
#endif

static map<string, pair<long long, long long>> cov;
static void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
static mt19937_64 rng(911);
static long long rnd(long long lo, long long hi) { return uniform_int_distribution<long long>(lo, hi)(rng); }

static long long naive(const map<long long, long long> &a, long long l, long long r) {
    long long s = 0;
    for (auto it = a.lower_bound(l); it != a.end() && it->first <= r; ++it) s += it->second;
    return s;
}

signed main() {
    { // merge()
        Dynamic_Segment_Tree st(1, 4);
        for (int it = 0; it < 1000; it++) {
            Dynamic_Segment_Tree::Node x(rnd(-1e12, 1e12)), y(rnd(-1e12, 1e12));
            chk("merge", st.merge(x, y).val == x.val + y.val);
        }
    }
    // small ranges [Lx, Rx] (often negative), every query range checked
    for (int trial = 0; trial < 3000; trial++) {
        long long Lx = rnd(-30, 30), Rx = Lx + rnd(0, trial < 200 ? 3 : 40);
        Dynamic_Segment_Tree st(Lx, Rx);
        map<long long, long long> a;
        for (long long l = Lx; l <= Rx; l++) chk("constructor(empty)", st.query(l, Rx) == 0);
        int ops = rnd(1, 60);
        while (ops--) {
            long long x = rnd(Lx, Rx), v = rng() % 4 ? rnd(-1e9, 1e9) : rnd(-2, 2);
            a[x] += v;
            st.update(x, v);
            chk("update", st.query(x, x) == a[x]);
            long long l = rnd(Lx, Rx), r = rnd(l, Rx);
            chk("query", st.query(l, r) == naive(a, l, r));
        }
        for (long long l = Lx; l <= Rx; l++)
            for (long long r = l; r <= Rx; r++) chk("query", st.query(l, r) == naive(a, l, r));
        for (long long l = Lx + 1; l <= Rx; l++) chk("query(l>r)", st.query(l, l - 1) == 0);
        chk("query(outside)", st.query(Lx - 5, Rx + 5) == naive(a, Lx, Rx)); // clipped
    }
    // huge ranges: default [-1e9, 1e9], [1, 1e9], [INT_MIN, INT_MAX]; extreme coordinates
    for (int trial = 0; trial < 300; trial++) {
        int kind = trial % 3;
        long long Lx = kind == 0 ? -1000000000LL : kind == 1 ? 1 : INT_MIN;
        long long Rx = kind == 0 ? 1000000000LL : kind == 1 ? 1000000000LL : INT_MAX;
        Dynamic_Segment_Tree st0, st1(1, 1e9), st2(INT_MIN, INT_MAX);
        Dynamic_Segment_Tree &st = kind == 0 ? st0 : kind == 1 ? st1 : st2;
        map<long long, long long> a;
        vector<long long> pts = {Lx, Rx, Lx + 1, Rx - 1, max(0LL, Lx), max(-1LL, Lx)};
        for (int k = 0; k < 30; k++) pts.push_back(rnd(Lx, Rx));
        for (int ops = 0; ops < 200; ops++) {
            long long x = pts[rnd(0, pts.size() - 1)], v = rnd(-1e9, 1e9);
            a[x] += v;
            st.update(x, v);
            long long l = pts[rnd(0, pts.size() - 1)], r = pts[rnd(0, pts.size() - 1)];
            if (l > r) swap(l, r);
            if (rng() % 3 == 0) l = rnd(Lx, Rx), r = rnd(l, Rx);
            chk(kind == 0 ? "query(default range)" : "query(huge range)", st.query(l, r) == naive(a, l, r));
        }
        chk("query(huge range)", st.query(Lx, Rx) == naive(a, Lx, Rx));
    }
    { // CSES 1144 Salary Queries, official sample: expect 3 3
        vector<long long> p = {3, 7, 2, 2, 5};
        Dynamic_Segment_Tree st(1, 1e9);
        for (auto x : p) st.update(x, 1);
        vector<long long> got;
        got.push_back(st.query(2, 3));
        st.update(p[2], -1), p[2] = 6, st.update(6, 1);
        got.push_back(st.query(2, 5));
        chk("samples", got == vector<long long>{3, 3});
    }
    { // timing: n = q = 2e5 on [1, 1e9] (CSES 1144 shape)
        const int N = 200000;
        auto t0 = chrono::steady_clock::now();
        Dynamic_Segment_Tree st(1, 1e9);
        map<long long, long long> a;
        long long sink = 0;
        for (int it = 0; it < N; it++) {
            long long x = rnd(1, 1e9);
            a[x]++;
            st.update(x, 1);
            long long l = rnd(1, 1e9), r = rnd(l, 1e9);
            long long got = st.query(l, r);
            sink += got;
            if (it % 2000 == 0) chk("query_large", got == naive(a, l, r));
        }
        double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
        printf("timing n=q=%lld on [1, 1e9]: %.1f ms (sink %lld)\n", (long long)N, ms, sink & 1);
    }
    bool pass = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        pass &= c.first > 0 && c.second == 0;
    }
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
