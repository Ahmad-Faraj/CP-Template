// Brute-force test for:
//   Math/Diophantine/LDE.cpp
//   Math/Diophantine/LDE_N_Variables.cpp
//   Math/Diophantine/LDE_N_Variables_Non_Negative.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace L2 {
#include "../Math/Diophantine/LDE.cpp"
}
namespace LN {
#include "../Math/Diophantine/LDE_N_Variables.cpp"
}
namespace LP {
#include "../Math/Diophantine/LDE_N_Variables_Non_Negative.cpp"
}

int main() {
    mt19937_64 gen(1234567);
    int anyFail = 0, cntFail = 0, listFail = 0, degFail = 0;
    int nFail = 0, nNoneFail = 0;
    int canFail = 0, cntRangeFail = 0, gapFail = 0;
    int boxChecked = 0, nChecked = 0, nnChecked = 0;

    // ---------- two variables: count and list against an exhaustive box scan ----------
    for (int it = 0; it < 20000; it++) {
        ll a = (ll)(gen() % 21) - 10, b = (ll)(gen() % 21) - 10, c = (ll)(gen() % 41) - 20;
        ll minx = (ll)(gen() % 21) - 10, maxx = minx + (ll)(gen() % 12);
        ll miny = (ll)(gen() % 21) - 10, maxy = miny + (ll)(gen() % 12);

        // brute force the whole box
        ll want = 0;
        vector<pair<ll, ll>> wantList;
        for (ll x = minx; x <= maxx; x++)
            for (ll y = miny; y <= maxy; y++)
                if (a * x + b * y == c) want++, wantList.push_back({x, y});

        ll got = L2::count_solutions(a, b, c, minx, maxx, miny, maxy);
        if (got != want) {
            if (a == 0 || b == 0) degFail++;
            else cntFail++;
        }
        boxChecked++;

        // any_solution must be a real solution whenever one exists anywhere
        ll x0, y0, g;
        bool ok = L2::any_solution(a, b, c, x0, y0, g);
        bool wantOk = (a == 0 && b == 0) ? (c == 0) : (c % __gcd(a < 0 ? -a : a, b < 0 ? -b : b) == 0);
        if (ok != wantOk) anyFail++;
        if (ok && a * x0 + b * y0 != c) anyFail++;

        // list_solutions must return exactly the brute-force set
        vector<pair<ll, ll>> gotList = L2::list_solutions(a, b, c, minx, maxx, miny, maxy);
        sort(gotList.begin(), gotList.end());
        sort(wantList.begin(), wantList.end());
        if (gotList != wantList) listFail++;
    }

    // ---------- n variables ----------
    for (int it = 0; it < 20000; it++) {
        int n = 1 + (int)(gen() % 5);
        vector<ll> a(n);
        for (ll &v : a) v = (ll)(gen() % 21) - 10;
        ll c = (ll)(gen() % 61) - 30;
        vector<ll> x = LN::any_solution(a, c);
        nChecked++;

        ll g = 0;
        for (ll v : a) g = __gcd(g, v < 0 ? -v : v);
        bool wantOk = (g == 0) ? (c == 0) : (c % g == 0);

        if (!wantOk) {
            if (!x.empty()) nNoneFail++;
        } else {
            if ((int)x.size() != n) { nFail++; continue; }
            __int128 total = 0;
            for (int i = 0; i < n; i++) total += (__int128)a[i] * x[i];
            if (total != c) nFail++;
        }
    }

    // ---------- non-negative: against a reachability DP ----------
    for (int it = 0; it < 1500; it++) {
        int n = 1 + (int)(gen() % 4);
        vector<ll> a(n);
        for (ll &v : a) v = 1 + (ll)(gen() % 12);
        LP::LDE_Non_Negative lde(a);
        nnChecked++;

        const int LIM = 200;
        vector<char> reach(LIM + 1, 0);
        reach[0] = 1;
        for (int k = 1; k <= LIM; k++)
            for (ll v : a)
                if (k - v >= 0 && reach[k - v]) { reach[k] = 1; break; }

        for (int k = 0; k <= LIM; k++)
            if (lde.can(k) != (bool)reach[k]) canFail++;

        // count over random ranges
        for (int t = 0; t < 5; t++) {
            ll l = (ll)(gen() % (LIM + 1)), r = l + (ll)(gen() % (LIM + 1 - l));
            ll want = 0;
            for (ll k = l; k <= r; k++) want += reach[k];
            if (lde.count(l, r) != want) cntRangeFail++;
        }
        // smallest_unreachable
        ll wantSmall = -1;
        for (int k = 0; k <= LIM; k++)
            if (!reach[k]) { wantSmall = k; break; }
        if (wantSmall != -1 && lde.smallest_unreachable() != wantSmall) gapFail++;
        // largest_unreachable, when every residue is reachable
        ll got = lde.largest_unreachable();
        if (got >= 0) {
            if (got > LIM) { /* beyond the scan, skip */ }
            else {
                if (reach[got]) gapFail++;                       // it must really be unreachable
                for (ll k = got + 1; k <= LIM; k++)
                    if (!reach[k]) gapFail++;                    // and nothing above it may be
            }
        }
    }

    // ---------- fixed cases ----------
    {
        // 2x + 3y = 7 has (2, 1) among its solutions
        ll x, y, g;
        if (!L2::any_solution(2, 3, 7, x, y, g) || 2 * x + 3 * y != 7) anyFail++;
        // 2x + 4y = 7 is unsolvable: gcd 2 does not divide 7
        if (L2::any_solution(2, 4, 7, x, y, g)) anyFail++;
        // coins 3 and 5: 1, 2, 4, 7 unreachable; 8 and up all reachable
        LP::LDE_Non_Negative coins(vector<ll>{3, 5});
        for (ll k : {(ll)1, (ll)2, (ll)4, (ll)7})
            if (coins.can(k)) canFail++;
        for (ll k : {(ll)0, (ll)3, (ll)5, (ll)6, (ll)8, (ll)9, (ll)10, (ll)11})
            if (!coins.can(k)) canFail++;
        if (coins.largest_unreachable() != 7) gapFail++; // Frobenius number of 3 and 5
        if (coins.smallest_unreachable() != 1) gapFail++;
        // n variables: 6x + 10y + 15z = 1 is solvable since gcd is 1
        vector<ll> v = LN::any_solution(vector<ll>{6, 10, 15}, 1);
        if (v.size() != 3 || 6 * v[0] + 10 * v[1] + 15 * v[2] != 1) nFail++;
    }

    cout << "Diophantine\n";
    cout << "  any_solution (2 vars)          : " << anyFail << " failures\n";
    cout << "  count_solutions in a box       : " << cntFail << " failures (" << boxChecked << " boxes)\n";
    cout << "  ... degenerate a or b == 0     : " << degFail << " failures\n";
    cout << "  list_solutions matches the box : " << listFail << " failures\n";
    cout << "  n variables: solution is valid : " << nFail << " failures (" << nChecked << " systems)\n";
    cout << "  n variables: unsolvable spotted: " << nNoneFail << " failures\n";
    cout << "  non-negative can()             : " << canFail << " failures (" << nnChecked << " coin sets)\n";
    cout << "  non-negative count(l, r)       : " << cntRangeFail << " failures\n";
    cout << "  smallest / largest unreachable : " << gapFail << " failures\n";
}
