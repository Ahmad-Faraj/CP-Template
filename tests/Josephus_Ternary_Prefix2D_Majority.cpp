// Brute-force test for:
//   Misc/Josephus.cpp
//   Misc/Ternary_Search.cpp
//   Range_Query/Prefix_Sum_2D.cpp
//   Misc/Majority_Element.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace JO { 
#include "../Misc/Josephus.cpp"
}
namespace TS {
#include "../Misc/Ternary_Search.cpp"
}
namespace P2 {
#include "../Range_Query/Prefix_Sum_2D.cpp"
}
namespace MJ {
#include "../Misc/Majority_Element.cpp"
}
int main() {
    mt19937_64 gen(1357);
    int josKill=0, josSurv=0, josK2=0, tsIntFail=0, tsRealFail=0, p2Fail=0, mjFail=0, mjkFail=0;

    // ---- Josephus against direct simulation ----
    for (ll n = 1; n <= 60; n++) for (ll k = 1; k <= 8; k++) {
        vector<ll> order = JO::removal_order(n, k);
        for (ll m = 1; m <= n; m++) if (JO::kth_killed(n, k, m) != order[m-1]) josKill++;
        if (JO::survivor(n, k) != order.back()) josSurv++;
        if (k == 2 && JO::survivor_k2(n) != order.back()) josK2++;
    }

    // ---- ternary search against a full scan ----
    for (int it = 0; it < 4000; it++) {
        ll a = 1 + (ll)(gen()%10), b = (ll)(gen()%41)-20, c = (ll)(gen()%21)-10;
        ll lo = -30 + (ll)(gen()%20), hi = lo + (ll)(gen()%60);
        auto f = [&](ll x){ return a*x*x + b*x + c; };            // convex, so unimodal
        ll got = TS::ternary_min(lo, hi, f), want = lo;
        for (ll x = lo; x <= hi; x++) if (f(x) < f(want)) want = x;
        if (f(got) != f(want)) tsIntFail++;
        auto g = [&](ll x){ return -f(x); };
        ll gotMax = TS::ternary_max(lo, hi, g), wantMax = lo;
        for (ll x = lo; x <= hi; x++) if (g(x) > g(wantMax)) wantMax = x;
        if (g(gotMax) != g(wantMax)) tsIntFail++;
        // a V shape with a flat bottom, which trips naive ternary search
        ll flatL = lo + (hi-lo)/3, flatR = lo + 2*(hi-lo)/3;
        auto v = [&](ll x){ return x < flatL ? flatL - x : (x > flatR ? x - flatR : 0LL); };
        ll gv = TS::ternary_min(lo, hi, v);
        if (v(gv) != 0) tsIntFail++;
    }
    { // real-valued: minimise (x-2.5)^2 + 1
        double x = TS::ternary_min_real(-10, 10, [](double t){ return (t-2.5)*(t-2.5)+1; });
        if (fabs(x - 2.5) > 1e-6) tsRealFail++;
        double y = TS::ternary_max_real(-10, 10, [](double t){ return -(t+1.25)*(t+1.25); });
        if (fabs(y + 1.25) > 1e-6) tsRealFail++;
    }

    // ---- 2D prefix sum against a direct double loop ----
    for (int it = 0; it < 3000; it++) {
        int n = 1 + (int)(gen()%8), m = 1 + (int)(gen()%8);
        vector<vector<ll>> g(n, vector<ll>(m));
        for (auto &r : g) for (ll &v : r) v = (ll)(gen()%41)-20;
        P2::Prefix_Sum_2D pre(g);
        for (int t = 0; t < 20; t++) {
            int x1=1+(int)(gen()%n), x2=1+(int)(gen()%n), y1=1+(int)(gen()%m), y2=1+(int)(gen()%m);
            if (x1>x2) swap(x1,x2); if (y1>y2) swap(y1,y2);
            ll want=0; for (int i=x1;i<=x2;i++) for (int j=y1;j<=y2;j++) want += g[i-1][j-1];
            if (pre.sum(x1,y1,x2,y2) != want) p2Fail++;
        }
        for (int i=1;i<=n;i++) for (int j=1;j<=m;j++) if (pre.get(i,j) != g[i-1][j-1]) p2Fail++;
        if (pre.sum(3,3,1,1) != 0) p2Fail++;                       // reversed range
        if (pre.sum(-5,-5,n+9,m+9) != pre.sum(1,1,n,m)) p2Fail++;  // clipped
    }

    // ---- majority against counting ----
    for (int it = 0; it < 20000; it++) {
        int n = (int)(gen()%14);
        int range = (it%2) ? 2 : 5;                                 // sometimes forces a majority
        vector<int> a(n);
        for (int &x : a) x = (int)(gen()%range);
        map<int,int> cnt; for (int x : a) cnt[x]++;
        int wantVal=-1; for (auto &[v,c] : cnt) if (c*2 > n) wantVal = v;
        auto got = MJ::majority(a);
        if (wantVal == -1) { if (got) mjFail++; }
        else if (!got || *got != wantVal) mjFail++;
        for (int k = 2; k <= 4; k++) {
            vector<int> wk; for (auto &[v,c] : cnt) if ((ll)c*k > n) wk.push_back(v);
            sort(wk.begin(), wk.end());
            if (MJ::majority_k(a, k) != wk) mjkFail++;
        }
    }
    printf("Batch: Josephus / Ternary_Search / Prefix_Sum_2D / Majority_Element\n");
    printf("  josephus kth_killed vs simulation : %d failures\n", josKill);
    printf("  josephus survivor                 : %d failures\n", josSurv);
    printf("  josephus k=2 closed form          : %d failures\n", josK2);
    printf("  ternary int (incl. flat bottom)   : %d failures\n", tsIntFail);
    printf("  ternary real                      : %d failures\n", tsRealFail);
    printf("  prefix sum 2D vs double loop      : %d failures\n", p2Fail);
    printf("  majority vs counting              : %d failures\n", mjFail);
    printf("  majority_k (Misra-Gries)          : %d failures\n", mjkFail);
}
