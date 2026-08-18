// Brute-force test for:
//   Range_Query/Fenwick_Tree_Range.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace FR {
#include "../Range_Query/Fenwick_Tree_Range.cpp"
}
int main() {
    mt19937_64 gen(31337);
    int sumFail = 0, getFail = 0, setFail = 0, buildFail = 0, checked = 0;
    for (int it = 0; it < 20000; it++) {
        int n = 1 + (int)(gen() % 12);
        vector<ll> a(n + 1, 0), init(n);
        for (int i = 0; i < n; i++) { init[i] = (ll)(gen() % 21) - 10; a[i + 1] = init[i]; }
        FR::Fenwick_Tree_Range bit(init);            // built from an array
        for (int i = 1; i <= n; i++) if (bit.get(i) != a[i]) buildFail++;
        checked++;
        for (int r = 0; r < 30; r++) {
            int l = 1 + (int)(gen() % n), rr = 1 + (int)(gen() % n);
            if (l > rr) swap(l, rr);
            int op = (int)(gen() % 4);
            if (op == 0) {                            // range add
                ll x = (ll)(gen() % 21) - 10;
                bit.update(l, rr, x);
                for (int i = l; i <= rr; i++) a[i] += x;
            } else if (op == 1) {                     // point add
                ll x = (ll)(gen() % 21) - 10;
                bit.update(l, x); a[l] += x;
            } else if (op == 2) {                     // point set
                ll x = (ll)(gen() % 21) - 10;
                bit.set(l, x); a[l] = x;
            }
            ll want = 0;
            for (int i = l; i <= rr; i++) want += a[i];
            if (bit.query(l, rr) != want) sumFail++;
            for (int i = 1; i <= n; i++) if (bit.get(i) != a[i]) getFail++;
            if (bit.query(rr, l - 1) != 0 && l > rr) setFail++;   // empty range
        }
    }
    // large values must not overflow: n=1000, repeated +1e12 over the whole range
    {
        int n = 1000;
        FR::Fenwick_Tree_Range bit(n);
        for (int i = 0; i < 1000; i++) bit.update(1, n, 1000000000000LL);
        ll want = (ll)1000 * 1000000000000LL * n;
        if (bit.query(1, n) != want) sumFail++;
    }
    printf("Fenwick_Tree_Range: %d arrays\n", checked);
    printf("  range sum after mixed updates : %d failures\n", sumFail);
    printf("  point read matches            : %d failures\n", getFail);
    printf("  build from an array           : %d failures\n", buildFail);
    printf("  empty range returns 0         : %d failures\n", setFail);
}
