// Brute-force test for:
//   Data_Structure/Interval_Set.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace IS {
#include "../Data_Structure/Interval_Set.cpp"
}
int main() {
    mt19937_64 gen(818181);
    int getFail=0,sumFail=0,cntFail=0,runFail=0,edgeFail=0,checked=0;
    for (int it = 0; it < 4000; it++) {
        ll n = 1 + (ll)(gen() % 30);
        ll init = (ll)(gen() % 3);
        IS::Interval_Set st(n, init);
        vector<ll> a(n + 2, init);
        checked++;
        for (int r = 0; r < 40; r++) {
            ll l = 1 + (ll)(gen() % n), rr = 1 + (ll)(gen() % n);
            if (l > rr) swap(l, rr);
            int kind = (int)(gen() % 3);
            if (kind == 0) {                       // assign
                ll v = (ll)(gen() % 4);
                st.assign(l, rr, v);
                for (ll i = l; i <= rr; i++) a[i] = v;
            } else if (kind == 1) {                // range add
                ll d = (ll)(gen() % 7) - 3;
                st.add(l, rr, d);
                for (ll i = l; i <= rr; i++) a[i] += d;
            }
            // point reads everywhere
            for (ll i = 1; i <= n; i++) if (st.get(i) != a[i]) getFail++;
            // range sum and count
            ll ws = 0; for (ll i = l; i <= rr; i++) ws += a[i];
            if (st.sum(l, rr) != ws) sumFail++;
            ll target = (ll)(gen() % 4), wc = 0;
            for (ll i = l; i <= rr; i++) wc += (a[i] == target);
            if (st.count(l, rr, target) != wc) cntFail++;
            // the runs must tile [l, rr] exactly, in order, with the right values
            vector<pair<ll,ll>> seen;
            st.for_each_run(l, rr, [&](ll f, ll t, ll v){
                for (ll i = f; i <= t; i++) if (a[i] != v) runFail++;
                seen.push_back({f, t});
            });
            ll expect = l;
            for (auto [f,t] : seen) { if (f != expect || t < f) runFail++; expect = t + 1; }
            if (expect != rr + 1) runFail++;
        }
    }
    {   // edges: full-range assign collapses to one run; empty range is a no-op
        IS::Interval_Set s(10, 7);
        if (s.run_count() != 1) edgeFail++;
        s.assign(3, 5, 9); s.assign(1, 10, 4);
        if (s.run_count() != 1 || s.sum(1,10) != 40) edgeFail++;
        s.assign(6, 5, 99);                       // l > r
        if (s.sum(1,10) != 40) edgeFail++;
        if (s.get(1) != 4 || s.get(10) != 4) edgeFail++;
        IS::Interval_Set one(1, 5);
        one.assign(1,1,8);
        if (one.get(1) != 8 || one.sum(1,1) != 8) edgeFail++;
    }
    printf("Interval_Set: %d sequences\n", checked);
    printf("  point read after assigns : %d failures\n", getFail);
    printf("  range sum                : %d failures\n", sumFail);
    printf("  count of a value         : %d failures\n", cntFail);
    printf("  runs tile the range      : %d failures\n", runFail);
    printf("  edge cases               : %d failures\n", edgeFail);
}
