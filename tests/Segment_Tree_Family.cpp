// Brute-force test for:
//   Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp
//   Range_Query/Segment_Tree/Segment_Tree_Persistent.cpp
//   Range_Query/Segment_Tree/Segment_Tree_Lazy_Persistent.cpp
//   Range_Query/Segment_Tree/Segment_Tree_Dynamic.cpp
//   Range_Query/Segment_Tree/Segment_Tree_Beats.cpp
//   Range_Query/Segment_Tree/Segment_Tree_2D.cpp
// Every one is compared against a plain array kept alongside it. The persistent ones are
// also asked for OLD versions after later writes, which is the thing they exist to do.
// Run with: py tools/test.py

#include "harness.h"
using namespace std;
using ll = long long;

namespace LZ {
#include "../Range_Query/Segment_Tree/Segment_Tree_Lazy.cpp"
}
namespace PS {
#include "../Range_Query/Segment_Tree/Segment_Tree_Persistent.cpp"
}
namespace LP {
#include "../Range_Query/Segment_Tree/Segment_Tree_Lazy_Persistent.cpp"
}
namespace DY {
#include "../Range_Query/Segment_Tree/Segment_Tree_Dynamic.cpp"
}
namespace BT {
#include "../Range_Query/Segment_Tree/Segment_Tree_Beats.cpp"
}
namespace S2 {
#include "../Range_Query/Segment_Tree/Segment_Tree_2D.cpp"
}

int main() {
    name("Segment tree family");
    mt19937_64 gen(918273645);
    ll lazyFail = 0, pstFail = 0, pstOld = 0, lpFail = 0, lpOld = 0;
    ll dynFail = 0, beatsFail = 0, beatsMinMax = 0, s2Fail = 0;
    ll cases = 0;

    // ---------- Lazy: range add, range sum ----------
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen() % 12), q = 1 + (int)(gen() % 15);
        vector<ll> a(n);
        for (auto &x : a) x = (ll)(gen() % 100) - 50;
        LZ::Lazy_Segment_Tree st;
        st.init(n);
        st.build(a);
        vector<ll> ref = a;
        cases++;
        while (q--) {
            int l = 1 + (int)(gen() % n), r = 1 + (int)(gen() % n);
            if (l > r) swap(l, r);
            if (gen() % 2) {
                ll v = (ll)(gen() % 21) - 10;
                st.update(l, r, v);
                for (int i = l; i <= r; i++) ref[i - 1] += v;
            } else {
                ll want = 0;
                for (int i = l; i <= r; i++) want += ref[i - 1];
                if (st.query(l, r) != want) lazyFail++;
            }
        }
    }

    // ---------- Persistent: point assign, and OLD versions must not move ----------
    for (int it = 0; it < 2000; it++) {
        int n = 1 + (int)(gen() % 10), q = 1 + (int)(gen() % 10);
        vector<int> a(n);
        for (auto &x : a) x = (int)(gen() % 50);
        PS::PST<int> pst(n, 1, n);
        pst.build(a);
        // roots is seeded with a sentinel, so build() is version 1 - history[v] is version v+1
        vector<vector<int>> history{a};
        for (int step = 0; step < q; step++) {
            int idx = 1 + (int)(gen() % n), val = (int)(gen() % 50);
            pst.update(idx, val);
            vector<int> next = history.back();
            next[idx - 1] = val;
            history.push_back(next);
        }
        for (int v = 0; v < (int)history.size(); v++)
            for (int l = 1; l <= n; l++)
                for (int r = l; r <= n; r++) {
                    int want = 0;
                    for (int i = l; i <= r; i++) want += history[v][i - 1];
                    int got = pst.query(l, r, v + 1);
                    if (got != want) (v + 1 == (int)history.size() ? pstFail : pstOld)++;
                }
        for (int v = 0; v < (int)history.size(); v++)
            for (int i = 1; i <= n; i++)
                if (pst.get(v + 1, i) != history[v][i - 1]) pstOld++;
    }

    // ---------- Lazy persistent: range add creating versions, old versions must survive ----------
    for (int it = 0; it < 1500; it++) {
        int n = 1 + (int)(gen() % 9), q = 1 + (int)(gen() % 8);
        vector<int> a(n);
        for (auto &x : a) x = (int)(gen() % 30);
        LP::PersistentSegTree<int> pst(n, a);
        vector<vector<int>> history{a};
        for (int step = 0; step < q; step++) {
            int l = 1 + (int)(gen() % n), r = 1 + (int)(gen() % n);
            if (l > r) swap(l, r);
            int v = (int)(gen() % 20);
            int base = (int)(gen() % history.size()); // branch off an arbitrary version
            pst.update(base, l, r, v);
            vector<int> next = history[base];
            for (int i = l; i <= r; i++) next[i - 1] += v;
            history.push_back(next);
        }
        for (int v = 0; v < (int)history.size(); v++)
            for (int l = 1; l <= n; l++)
                for (int r = l; r <= n; r++) {
                    int want = 0;
                    for (int i = l; i <= r; i++) want += history[v][i - 1];
                    int got = pst.query(v, l, r);
                    if (got != want) (v + 1 == (int)history.size() ? lpFail : lpOld)++;
                }
    }

    // ---------- Dynamic: sparse indices, point add, range sum ----------
    for (int it = 0; it < 2000; it++) {
        int q = 1 + (int)(gen() % 14);
        DY::Dynamic_Segment_Tree dst(1, 1000000000);
        map<int, int> ref;
        vector<int> touched;
        while (q--) {
            if (gen() % 2 || touched.empty()) {
                int idx = 1 + (int)(gen() % 1000000000), val = (int)(gen() % 20);
                dst.update(idx, val);
                ref[idx] += val;
                touched.push_back(idx);
            } else {
                int l = touched[gen() % touched.size()], r = touched[gen() % touched.size()];
                if (l > r) swap(l, r);
                int want = 0;
                for (auto &kv : ref)
                    if (kv.first >= l && kv.first <= r) want += kv.second;
                if (dst.query(l, r) != want) dynFail++;
            }
        }
    }

    // ---------- Beats: chmin, chmax, range add, against a plain array ----------
    for (int it = 0; it < 2000; it++) {
        int n = 1 + (int)(gen() % 10), q = 1 + (int)(gen() % 14);
        vector<ll> a(n);
        for (auto &x : a) x = (ll)(gen() % 200) - 100;
        BT::SGTBeats t(n, a.data());
        vector<ll> ref = a;
        while (q--) {
            int l = (int)(gen() % n), r = (int)(gen() % n);
            if (l > r) swap(l, r);
            r++; // Beats uses [a, b)
            int op = (int)(gen() % 5);
            ll x = (ll)(gen() % 200) - 100;
            if (op == 0) {
                t.update_min(l, r, x);
                for (int i = l; i < r; i++) ref[i] = min(ref[i], x);
            } else if (op == 1) {
                t.update_max(l, r, x);
                for (int i = l; i < r; i++) ref[i] = max(ref[i], x);
            } else if (op == 2) {
                t.add_val(l, r, x);
                for (int i = l; i < r; i++) ref[i] += x;
            } else if (op == 3) {
                ll want = 0;
                for (int i = l; i < r; i++) want += ref[i];
                if (t.query_sum(l, r) != want) beatsFail++;
            } else {
                ll wantMax = LLONG_MIN, wantMin = LLONG_MAX;
                for (int i = l; i < r; i++) wantMax = max(wantMax, ref[i]), wantMin = min(wantMin, ref[i]);
                if (t.query_max(l, r) != wantMax || t.query_min(l, r) != wantMin) beatsMinMax++;
            }
        }
    }

    // ---------- 2D: rectangle sums on a grid ----------
    for (int it = 0; it < 1200; it++) {
        int n = 1 + (int)(gen() % 6), m = 1 + (int)(gen() % 6);
        vector<vector<int>> a(n, vector<int>(m));
        for (auto &row : a)
            for (auto &x : row) x = (int)(gen() % 50);
        S2::SegTree2D st(n, m, a);
        for (int x1 = 1; x1 <= n; x1++)
            for (int x2 = x1; x2 <= n; x2++)
                for (int y1 = 1; y1 <= m; y1++)
                    for (int y2 = y1; y2 <= m; y2++) {
                        int want = 0;
                        for (int i = x1; i <= x2; i++)
                            for (int j = y1; j <= y2; j++) want += a[i - 1][j - 1];
                        if (st.query(x1, x2, y1, y2) != want) s2Fail++;
                    }
    }

    check("Lazy: range add / range sum", lazyFail, cases);
    check("Persistent: newest version", pstFail);
    check("Persistent: OLD versions unchanged", pstOld);
    check("Lazy persistent: newest version", lpFail);
    check("Lazy persistent: OLD versions", lpOld);
    check("Dynamic: sparse point add / range sum", dynFail);
    check("Beats: chmin / chmax / add vs array", beatsFail);
    check("Beats: range max and min", beatsMinMax);
    check("2D: every rectangle", s2Fail);
    return report();
}
