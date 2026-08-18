// Brute-force test for:
//   Math/Polynomial/Lagrange_Interpolation.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
namespace LI {
#include "../Math/Polynomial/Lagrange_Interpolation.cpp"
}
const ll MOD = 1000000007;
ll PW(ll b, ll e) { b %= MOD; ll r = 1; for (; e; e >>= 1, b = b * b % MOD) if (e & 1) r = r * b % MOD; return r; }

int main() {
    mt19937_64 gen(31337);
    int arbFail = 0, eqFail = 0, spotFail = 0, powFail = 0, bigFail = 0, degFail = 0, cases = 0;

    // ---- against a polynomial we KNOW, evaluated directly
    for (int it = 0; it < 4000; it++) {
        int deg = (int)(gen() % 8);            // degree 0..7
        vector<ll> coef(deg + 1);
        for (ll &c : coef) c = (ll)(gen() % MOD);
        auto eval = [&](ll x) { ll r = 0, p = 1; x %= MOD; for (int i = 0; i <= deg; i++) { r = (r + coef[i] * p) % MOD; p = p * x % MOD; } return r; };
        cases++;

        // arbitrary distinct points
        vector<pair<ll, ll>> pts;
        set<ll> used;
        while ((int)pts.size() <= deg) {
            ll x = (ll)(gen() % 2000);
            if (used.count(x)) continue;
            used.insert(x);
            pts.push_back({x, eval(x)});
        }
        ll q = (ll)(gen() % MOD);
        if (LI::interpolate(pts, q) != eval(q)) arbFail++;
        // asking for a point that IS a sample must not divide by zero
        if (LI::interpolate(pts, pts[0].first) != pts[0].second) spotFail++;

        // equally spaced, with a random offset and stride
        ll a = (ll)(gen() % 50), d = 1 + (ll)(gen() % 7);
        vector<ll> y(deg + 1);
        for (int i = 0; i <= deg; i++) y[i] = eval(a + d * i);
        if (LI::interpolate_equal(y, q, a, d) != eval(q)) eqFail++;
        if (LI::interpolate_equal(y, a + d * 2, a, d) != y[min(2, deg)] * (deg >= 2) + eval(a + d * 2) * (deg < 2)) {
            if (deg >= 2) spotFail++;
        }
        // more points than the degree needs must still work
        vector<ll> y2(deg + 4);
        for (int i = 0; i < deg + 4; i++) y2[i] = eval(a + d * i);
        if (LI::interpolate_equal(y2, q, a, d) != eval(q)) degFail++;
    }

    // ---- sum_of_powers against a direct loop
    for (int k = 0; k <= 20; k++)
        for (int n = 0; n <= 60; n++) {
            ll want = 0;
            for (int i = 1; i <= n; i++) want = (want + PW(i, k)) % MOD;
            if (LI::sum_of_powers(n, k) != want) powFail++;
        }

    // ---- huge n, where the whole point is that a loop is impossible: check against a closed form
    {
        // k = 1: n(n+1)/2   k = 2: n(n+1)(2n+1)/6   k = 3: (n(n+1)/2)^2
        ll inv2 = PW(2, MOD - 2), inv6 = PW(6, MOD - 2);
        for (ll n : {(ll)1e18, (ll)9e18, (ll)123456789012345LL, (ll)1000000006, (ll)1000000007, (ll)1000000008}) {
            ll m = n % MOD, m1 = (m + 1) % MOD;
            if (LI::sum_of_powers(n, 1) != m * m1 % MOD * inv2 % MOD) bigFail++;
            if (LI::sum_of_powers(n, 2) != m * m1 % MOD * ((2 * m + 1) % MOD) % MOD * inv6 % MOD) bigFail++;
            ll t = m * m1 % MOD * inv2 % MOD;
            if (LI::sum_of_powers(n, 3) != t * t % MOD) bigFail++;
        }
    }

    printf("Lagrange_Interpolation  (%d random polynomials)\n", cases);
    printf("  interpolate, arbitrary pts : %d failures\n", arbFail);
    printf("  interpolate_equal          : %d failures\n", eqFail);
    printf("  x lands on a sample point  : %d failures\n", spotFail);
    printf("  extra points beyond degree : %d failures\n", degFail);
    printf("  sum_of_powers, k<=20 n<=60 : %d failures\n", powFail);
    printf("  sum_of_powers at n ~ 9e18  : %d failures\n", bigFail);
}
