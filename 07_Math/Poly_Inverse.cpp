// Poly Inverse: the first n terms of 1 / a(x) as a formal power series modulo 998244353.
// Use when: "inverse of a power series", dividing generating functions, b = 1 / (1 - f) counts.
// Usage: vector<ll> b = poly_inverse(a, n);  // a(x) * b(x) == 1 (mod x^n)
// Handles: coefficients outside [0, MOD) incl. negatives (reduced first), any |a| vs n, n = 0.
// Time: O(n log n)
// Indexing: 0-based; b[k] = coefficient of x^k, |b| = n
// Note: needs a[0] % MOD != 0 (otherwise no inverse: returns {}). Paste NTT.cpp first and drop the
//       #include line below.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#include "NTT.cpp" // delete when pasting: NTT.cpp must already be above

vector<ll> poly_inverse(const vector<ll> &a, int n) {
    const ll MOD = NTT::MOD;
    if (n <= 0) return {};
    if (a.empty() || (a[0] % MOD + MOD) % MOD == 0) return {};
    vector<ll> b{NTT::mod_pow((a[0] % MOD + MOD) % MOD, MOD - 2, MOD)};
    for (int k = 1; k < n; k *= 2) {
        vector<ll> ak(a.begin(), a.begin() + min<size_t>(a.size(), 2 * k));
        vector<ll> c = NTT::multiply(ak, b);
        c.resize(2 * k);
        for (ll &x : c) x = x ? MOD - x : 0;
        c[0] = (c[0] + 2) % MOD;
        b = NTT::multiply(b, c);
        b.resize(2 * k);
    }
    b.resize(n);
    return b;
}
