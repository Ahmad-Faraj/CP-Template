// Brute-force test for:
//   Math/Polynomial/FFT.cpp
//   Math/Polynomial/NTT.cpp
//   Math/Polynomial/NTT_Any_Mod.cpp
//   Math/Polynomial/FWHT.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace FT {
#include "../Math/Polynomial/FFT.cpp"
}
namespace NT {
#include "../Math/Polynomial/NTT.cpp"
}
namespace NA {
#include "../Math/Polynomial/NTT_Any_Mod.cpp"
}
namespace FW {
#include "../Math/Polynomial/FWHT.cpp"
}

// the definition: plain O(n*m) convolution
vector<ll> naive(const vector<ll> &a, const vector<ll> &b, ll mod = 0) {
    if (a.empty() || b.empty()) return {};
    vector<ll> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) {
            if (mod)
                c[i + j] = (ll)((c[i + j] + (__int128)a[i] * b[j]) % mod);
            else
                c[i + j] += a[i] * b[j];
        }
    if (mod)
        for (ll &x : c) x = ((x % mod) + mod) % mod;
    return c;
}

int main() {
    mt19937_64 gen(31337);
    int fftFail = 0, fftRealFail = 0, fftEdgeFail = 0, marginFail = 0;
    int nttFail = 0, nttPowFail = 0, nttEdgeFail = 0;
    int anyFail = 0, anyBigFail = 0;
    int xorFail = 0, andFail = 0, orFail = 0, invFail = 0, powFail = 0;
    int checked = 0;

    for (int it = 0; it < 3000; it++) {
        int n = (int)(gen() % 12), m = (int)(gen() % 12);
        vector<ll> a(n), b(m);
        for (ll &x : a) x = (ll)(gen() % 2001) - 1000;
        for (ll &x : b) x = (ll)(gen() % 2001) - 1000;
        checked++;

        // FFT, no modulus
        if (FT::multiply(a, b) != naive(a, b)) fftFail++;
        // FFT on reals: within tolerance of the exact integer answer
        {
            vector<double> da(a.begin(), a.end()), db(b.begin(), b.end());
            vector<double> got = FT::multiply_real(da, db);
            vector<ll> want = naive(a, b);
            if (got.size() != want.size()) fftRealFail++;
            else
                for (size_t i = 0; i < got.size(); i++)
                    if (fabs(got[i] - (double)want[i]) > 1e-6 * max(1.0, fabs((double)want[i]))) fftRealFail++;
        }
        // the rounding margin must be comfortably below 0.5
        if (!a.empty() && !b.empty() && FT::rounding_margin(a, b) > 0.25) marginFail++;

        // NTT under its own prime
        {
            ll mod = NT::NTT_MOD;
            if (NT::multiply(a, b) != naive(a, b, mod)) nttFail++;
        }
        // NTT_Any_Mod under several moduli, including a non-prime one
        for (ll mod : {(ll)1000000007, (ll)1000000009, (ll)998244353, (ll)1000000000, (ll)1}) {
            if (NA::multiply(a, b, mod) != naive(a, b, mod)) anyFail++;
        }
    }

    // ---------- edge cases ----------
    {
        vector<ll> empty_, one{5}, zeros{0, 0, 0};
        if (!FT::multiply(empty_, one).empty()) fftEdgeFail++;
        if (!FT::multiply(one, empty_).empty()) fftEdgeFail++;
        if (FT::multiply(one, one) != vector<ll>{25}) fftEdgeFail++;
        if (FT::multiply(zeros, one) != vector<ll>{0, 0, 0}) fftEdgeFail++;
        if (!NT::multiply(empty_, one).empty()) nttEdgeFail++;
        if (NT::multiply(one, one) != vector<ll>{25}) nttEdgeFail++;
        if (!NA::multiply(empty_, one, 1000000007).empty()) nttEdgeFail++;
    }

    // ---------- large coefficients: where FFT loses and NTT must not ----------
    {
        int n = 400;
        vector<ll> a(n), b(n);
        for (int i = 0; i < n; i++) a[i] = 1000000000 + i, b[i] = 999999999 - i;
        vector<ll> want = naive(a, b, NT::NTT_MOD);
        if (NT::multiply(a, b) != want) nttFail++;
        vector<ll> want7 = naive(a, b, 1000000007);
        if (NA::multiply(a, b, 1000000007) != want7) anyBigFail++;
        // and the same via three-mod CRT under a modulus above 1e18
        ll big = 1000000000000000003LL;
        if (NA::multiply(a, b, big) != naive(a, b, big)) anyBigFail++;
    }

    // ---------- poly_pow ----------
    for (int it = 0; it < 400; it++) {
        int n = 1 + (int)(gen() % 5);
        ll e = (ll)(gen() % 6);
        vector<ll> a(n);
        for (ll &x : a) x = (ll)(gen() % 20);
        vector<ll> got = NT::poly_pow(a, e);
        vector<ll> want{1 % NT::NTT_MOD};
        for (ll t = 0; t < e; t++) want = naive(want, a, NT::NTT_MOD);
        // poly_pow trims trailing structure only via keep_length; compare the overlap
        size_t common = min(got.size(), want.size());
        for (size_t i = 0; i < common; i++)
            if (got[i] != want[i]) nttPowFail++;
        for (size_t i = common; i < got.size(); i++)
            if (got[i] != 0) nttPowFail++;
        for (size_t i = common; i < want.size(); i++)
            if (want[i] != 0) nttPowFail++;
    }

    // ---------- FWHT against the definition ----------
    for (int it = 0; it < 3000; it++) {
        int bits = 1 + (int)(gen() % 5);
        size_t n = (size_t)1 << bits;
        ll mod = 1000000007;
        vector<ll> a(n), b(n);
        for (ll &x : a) x = (ll)(gen() % 1000);
        for (ll &x : b) x = (ll)(gen() % 1000);

        for (int which = 0; which < 3; which++) {
            FW::Combine how = which == 0 ? FW::WHT_XOR : (which == 1 ? FW::WHT_AND : FW::WHT_OR);
            vector<ll> got = FW::convolve(a, b, how, mod);
            // definition: for every pair, combine the indices
            vector<ll> want(n, 0);
            for (size_t i = 0; i < n; i++)
                for (size_t j = 0; j < n; j++) {
                    size_t k = which == 0 ? (i ^ j) : (which == 1 ? (i & j) : (i | j));
                    want[k] = (ll)((want[k] + (__int128)a[i] * b[j]) % mod);
                }
            if (got != want) {
                if (which == 0) xorFail++;
                else if (which == 1) andFail++;
                else orFail++;
            }
            // forward then inverse must be the identity
            vector<ll> t = a;
            FW::wht(t, how, mod);
            FW::inverse_wht(t, how, mod);
            if (t != a) invFail++;
        }

        // convolve_power with times = 2 must equal convolving with itself
        vector<ll> p2 = FW::convolve_power(a, 2, FW::WHT_XOR, mod);
        vector<ll> selfc = FW::convolve(a, a, FW::WHT_XOR, mod);
        if (p2 != selfc) powFail++;
        // times = 1 is the identity
        vector<ll> p1 = FW::convolve_power(a, 1, FW::WHT_XOR, mod);
        vector<ll> anorm = a;
        for (ll &x : anorm) x %= mod;
        if (p1 != anorm) powFail++;
    }
    // padding helper
    {
        vector<ll> v{1, 2, 3};
        if (FW::pad_to_power_of_two(v) != 4 || v.size() != 4 || v[3] != 0) powFail++;
        vector<ll> w{1, 2, 3, 4};
        if (FW::pad_to_power_of_two(w) != 4) powFail++;
    }

    cout << "Polynomial: " << checked << " random pairs\n";
    cout << "  FFT vs naive convolution       : " << fftFail << " failures\n";
    cout << "  FFT on reals                   : " << fftRealFail << " failures\n";
    cout << "  FFT rounding margin under 0.25 : " << marginFail << " failures\n";
    cout << "  FFT edge cases                 : " << fftEdgeFail << " failures\n";
    cout << "  NTT vs naive mod 998244353     : " << nttFail << " failures\n";
    cout << "  NTT poly_pow                   : " << nttPowFail << " failures\n";
    cout << "  NTT edge cases                 : " << nttEdgeFail << " failures\n";
    cout << "  NTT_Any_Mod, 5 moduli          : " << anyFail << " failures\n";
    cout << "  NTT_Any_Mod, huge coefficients : " << anyBigFail << " failures\n";
    cout << "  FWHT XOR convolution           : " << xorFail << " failures\n";
    cout << "  FWHT AND convolution           : " << andFail << " failures\n";
    cout << "  FWHT OR convolution            : " << orFail << " failures\n";
    cout << "  FWHT transform round trip      : " << invFail << " failures\n";
    cout << "  FWHT convolve_power            : " << powFail << " failures\n";
}
