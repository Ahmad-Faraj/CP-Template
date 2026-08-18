// Brute-force test for:
//   Math/Number_Theory/Mod_Arithmetic.cpp
//   Math/Number_Theory/Exponentiation.cpp
//   Math/Number_Theory/CRT.cpp
//   Math/Number_Theory/Extended_GCD.cpp
//   Math/Number_Theory/Discrete_Log.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace MA {
#include "../Math/Number_Theory/Mod_Arithmetic.cpp"
}
namespace EX {
#include "../Math/Number_Theory/Exponentiation.cpp"
}
namespace CR {
#include "../Math/Number_Theory/CRT.cpp"
}
namespace EG {
#include "../Math/Number_Theory/Extended_GCD.cpp"
}
namespace DL {
#include "../Math/Number_Theory/Discrete_Log.cpp"
}

int main() {
    mt19937_64 gen(20260817);
    int normFail = 0, arithFail = 0, powFail = 0, invFail = 0, strFail = 0, bigMulFail = 0;
    int matFail = 0, permFail = 0, intPowFail = 0;
    int crtFail = 0, crtBadFail = 0, crtNonCoFail = 0;
    int egFail = 0, ldeFail = 0, lcmFail = 0;
    int dlFail = 0, dlNoneFail = 0, dlKthFail = 0;
    int dlChecked = 0, crtChecked = 0;

    // ---------- modular arithmetic against __int128 ground truth ----------
    for (int it = 0; it < 200000; it++) {
        ll m = 1 + (ll)(gen() % 1000);
        ll a = (ll)(gen() % 4000) - 2000, b = (ll)(gen() % 4000) - 2000;
        auto tn = [&](ll v) { return (ll)(((__int128)v % m + m) % m); };
        if (MA::norm(a, m) != tn(a)) normFail++;
        if (MA::add_mod(a, b, m) != tn((__int128)a + b)) arithFail++;
        if (MA::sub_mod(a, b, m) != tn((__int128)a - b)) arithFail++;
        if (MA::mul_mod(a, b, m) != (ll)(((__int128)tn(a) * tn(b)) % m)) arithFail++;
        // power against repeated multiplication
        ll e = (ll)(gen() % 12);
        ll want = 1 % m;
        for (int k = 0; k < e; k++) want = (ll)((__int128)want * tn(a) % m);
        if (MA::pow_mod(a, e, m) != want) powFail++;
        if (EX::pow_mod(a, e, m) != want) powFail++;
        // inverse: either -1 (and gcd != 1) or a genuine inverse
        ll iv = MA::inv_mod(a, m);
        ll g = __gcd(tn(a), m);
        if (iv == -1) {
            if (g == 1 && m > 1) invFail++;
        } else {
            if (MA::mul_mod(a, iv, m) != 1 % m) invFail++;
            if (g != 1) invFail++;
        }
        if (EG::inv_mod(a, m) != iv) invFail++; // the two files must agree
    }

    // mul_mod must survive a modulus near 9e18, where plain ll would overflow
    {
        ll m = 9000000000000000031LL;
        ll a = 8999999999999999999LL, b = 7777777777777777777LL;
        ll got = MA::mul_mod(a, b, m);
        ll want = (ll)((__int128)(a % m) * (b % m) % m);
        if (got != want) bigMulFail++;
        if (MA::pow_mod(a, 5, m) != (ll)([&] {
                __int128 r = 1;
                for (int i = 0; i < 5; i++) r = r * (a % m) % m;
                return (ll)r;
            }())) bigMulFail++;
    }

    // mod_of_string against a value we can compute directly
    for (int it = 0; it < 5000; it++) {
        ll v = (ll)(gen() % 1000000000000000000ULL);
        ll m = 1 + (ll)(gen() % 1000000);
        if (MA::mod_of_string(m, to_string(v)) != v % m) strFail++;
    }

    // ---------- matrix and permutation exponentiation ----------
    for (int it = 0; it < 4000; it++) {
        int k = 1 + (int)(gen() % 4);
        ll m = 2 + (ll)(gen() % 1000);
        EX::Matrix base(k, m);
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++) base.a[i][j] = (ll)(gen() % m);
        ll e = (ll)(gen() % 9);
        EX::Matrix got = EX::pow_matrix(base, e);
        // reference: multiply e times, longhand
        EX::Matrix want = EX::Matrix::identity(k, m);
        for (int t = 0; t < e; t++) {
            EX::Matrix c(k, m);
            for (int i = 0; i < k; i++)
                for (int j = 0; j < k; j++) {
                    __int128 s = 0;
                    for (int q = 0; q < k; q++) s += (__int128)want.a[i][q] * base.a[q][j];
                    c.a[i][j] = (ll)(s % m);
                }
            want = c;
        }
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                if (got.a[i][j] != want.a[i][j]) matFail++;

        // permutation power against repeated application
        int sz = 1 + (int)(gen() % 6);
        vector<int> p(sz);
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), gen);
        vector<int> a(sz);
        for (int i = 0; i < sz; i++) a[i] = (int)(gen() % 100);
        ll kk = (ll)(gen() % 10);
        vector<int> gotp = EX::pow_permutation(a, p, kk);
        vector<int> wantp = a;
        for (int t = 0; t < kk; t++) wantp = EX::apply_permutation(wantp, p);
        if (gotp != wantp) permFail++;
    }
    // pow_int against pow for small values
    for (ll b = 0; b <= 10; b++)
        for (ll e = 0; e <= 6; e++) {
            ll want = 1;
            for (int i = 0; i < e; i++) want *= b;
            if (EX::pow_int(b, e) != want) intPowFail++;
        }

    // ---------- CRT ----------
    for (int it = 0; it < 30000; it++) {
        int k = 1 + (int)(gen() % 4);
        vector<ll> mods(k), rems(k);
        for (int i = 0; i < k; i++) {
            mods[i] = 1 + (ll)(gen() % 30); // small, and deliberately NOT coprime
            rems[i] = (ll)(gen() % 60);
        }
        bool ok;
        auto [r, M] = CR::crt(rems, mods, ok);
        // brute force: scan for the smallest x satisfying everything
        ll lcm = 1;
        for (ll x : mods) lcm = lcm / __gcd(lcm, x) * x;
        ll found = -1;
        for (ll x = 0; x < lcm; x++) {
            bool all = true;
            for (int i = 0; i < k; i++)
                if (x % mods[i] != rems[i] % mods[i]) { all = false; break; }
            if (all) { found = x; break; }
        }
        crtChecked++;
        if (found == -1) {
            if (ok) crtBadFail++; // claimed solvable when it is not
        } else {
            if (!ok) { crtBadFail++; continue; }
            if (r != found) crtFail++;
            if (M != lcm) crtFail++;
            // and every solution of the reported congruence must satisfy the system
            for (ll t = 0; t < 3; t++) {
                ll x = r + t * M;
                for (int i = 0; i < k; i++)
                    if (x % mods[i] != rems[i] % mods[i]) crtNonCoFail++;
            }
        }
    }

    // ---------- extended gcd and LDE ----------
    for (int it = 0; it < 100000; it++) {
        ll a = (ll)(gen() % 2001) - 1000, b = (ll)(gen() % 2001) - 1000;
        ll x, y;
        ll g = EG::extended_gcd(a, b, x, y);
        if (g < 0) egFail++;
        if ((__int128)a * x + (__int128)b * y != g) egFail++;
        if (g != (ll)__gcd(a < 0 ? -a : a, b < 0 ? -b : b)) egFail++;
        // lcm
        ll l = EG::lcm_of(a, b);
        if (a == 0 || b == 0) {
            if (l != 0) lcmFail++;
        } else if (l % a != 0 || l % b != 0)
            lcmFail++;
        // LDE
        ll c = (ll)(gen() % 400) - 200;
        ll sx, sy;
        bool solvable = EG::linear_solution(a, b, c, sx, sy);
        bool want = (g == 0) ? (c == 0) : (c % g == 0);
        if (solvable != want) ldeFail++;
        if (solvable && (__int128)a * sx + (__int128)b * sy != c) ldeFail++;
    }

    // ---------- discrete log ----------
    for (int it = 0; it < 8000; it++) {
        ll m = 2 + (ll)(gen() % 60);
        ll a = (ll)(gen() % m), b = (ll)(gen() % m);
        ll got = DL::discrete_log(a, b, m);
        // brute force the smallest x >= 1 up to a safe bound
        ll want = -1;
        ll cur = 1 % m;
        for (ll x = 1; x <= 4 * m + 8; x++) {
            cur = cur * a % m;
            if (cur == b % m) { want = x; break; }
        }
        dlChecked++;
        if (want == -1) {
            if (got != -1) dlNoneFail++;
        } else if (got != want)
            dlFail++;
        // allow_zero
        ll got0 = DL::discrete_log(a, b, m, true);
        ll want0 = (1 % m == b % m) ? 0 : want;
        if (got0 != want0) dlFail++;
        // kth solution must actually satisfy the congruence
        if (want != -1) {
            for (ll k = 1; k <= 3; k++) {
                ll x = DL::kth_solution(a, b, m, k);
                if (x == -1) continue;
                if (DL::pow_mod(a, x, m) != b % m) dlKthFail++;
            }
        }
    }

    cout << "Number_Theory batch A\n";
    cout << "  norm / add / sub / mul vs int128 : " << normFail + arithFail << " failures\n";
    cout << "  pow_mod (both files)             : " << powFail << " failures\n";
    cout << "  inv_mod, and the two files agree : " << invFail << " failures\n";
    cout << "  modulus near 9e18                : " << bigMulFail << " failures\n";
    cout << "  mod_of_string                    : " << strFail << " failures\n";
    cout << "  pow_matrix vs longhand           : " << matFail << " failures\n";
    cout << "  pow_permutation                  : " << permFail << " failures\n";
    cout << "  pow_int                          : " << intPowFail << " failures\n";
    cout << "  CRT value and modulus            : " << crtFail << " failures (" << crtChecked << " systems)\n";
    cout << "  CRT solvability verdict          : " << crtBadFail << " failures\n";
    cout << "  CRT non-coprime moduli hold      : " << crtNonCoFail << " failures\n";
    cout << "  extended_gcd identity            : " << egFail << " failures\n";
    cout << "  lcm_of                           : " << lcmFail << " failures\n";
    cout << "  linear_solution                  : " << ldeFail << " failures\n";
    cout << "  discrete_log value               : " << dlFail << " failures (" << dlChecked << " cases)\n";
    cout << "  discrete_log reports -1 correctly: " << dlNoneFail << " failures\n";
    cout << "  kth_solution satisfies a^x = b   : " << dlKthFail << " failures\n";
}
