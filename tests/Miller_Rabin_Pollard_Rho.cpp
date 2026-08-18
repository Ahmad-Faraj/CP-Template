// Brute-force test for:
//   Math/Number_Theory/Miller_Rabin_Pollard_Rho.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long; using ull = unsigned long long;
namespace MR {
#include "../Math/Number_Theory/Miller_Rabin_Pollard_Rho.cpp"
}
int main() {
    // primality against a sieve, exhaustively
    const int LIM = 200000;
    vector<char> comp(LIM + 1, 0);
    for (int i = 2; (ll)i * i <= LIM; i++) if (!comp[i]) for (ll j = (ll)i*i; j <= LIM; j += i) comp[j] = 1;
    int primeFail = 0;
    for (int n = 0; n <= LIM; n++) {
        bool want = n >= 2 && !comp[n];
        if (MR::is_prime(n) != want) primeFail++;
    }

    mt19937_64 gen(99);
    int factFail = 0, divFail = 0, bigFail = 0, checked = 0;
    // factorization: multiplies back, every factor prime, matches trial division
    for (int it = 0; it < 4000; it++) {
        ull n = 1 + gen() % 2000000;
        auto f = MR::factorize(n);
        __uint128_t prod = 1;
        for (auto [p, e] : f) { if (!MR::is_prime(p)) factFail++; for (int i = 0; i < e; i++) prod *= p; }
        if (prod != n) factFail++;
        // trial division reference
        vector<pair<ull,int>> want; ull m = n;
        for (ull p = 2; p * p <= m; p++) { int e = 0; while (m % p == 0) m /= p, e++; if (e) want.push_back({p, e}); }
        if (m > 1) want.push_back({m, 1});
        if (n > 1 && f != want) factFail++;
        checked++;
        // divisors
        if (n <= 200000) {
            auto got = MR::divisors(n);
            vector<ull> wd; for (ull d = 1; d <= n; d++) if (n % d == 0) wd.push_back(d);
            if (got != wd) divFail++;
        }
    }
    // the whole point: numbers far beyond trial division
    struct C { ull n; const char *what; };
    vector<C> cases = {
        {1000000007ULL, "prime"}, {1000000009ULL, "prime"},
        {(ull)1000000007ULL * 1000000009ULL, "semiprime ~1e18"},
        {(ull)999999937ULL * 999999937ULL, "square of a large prime"},
        {2305843009213693951ULL, "2^61 - 1, prime"},
        {9223372036854775783ULL, "largest prime below 2^63"},
        {1000000000000000000ULL, "1e18"},
        {(ull)4294967291ULL * 4294967279ULL, "two 32-bit primes"},
    };
    for (auto &c : cases) {
        auto f = MR::factorize(c.n);
        __uint128_t prod = 1;
        for (auto [p, e] : f) { if (!MR::is_prime(p)) bigFail++; for (int i = 0; i < e; i++) prod *= p; }
        if (prod != c.n) bigFail++;
    }
    if (!MR::is_prime(2305843009213693951ULL)) bigFail++;
    if (!MR::is_prime(9223372036854775783ULL)) bigFail++;
    if (MR::is_prime((ull)1000000007ULL * 1000000009ULL)) bigFail++;

    auto t0 = chrono::steady_clock::now();
    auto hard = MR::factorize((ull)1000000007ULL * 1000000009ULL);
    auto ms = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - t0).count();

    printf("Miller_Rabin_Pollard_Rho\n");
    printf("  is_prime vs sieve to 200000   : %d failures\n", primeFail);
    printf("  factorize vs trial division   : %d failures (%d numbers)\n", factFail, checked);
    printf("  divisors                      : %d failures\n", divFail);
    printf("  numbers up to 9.2e18          : %d failures\n", bigFail);
    printf("  1e9+7 x 1e9+9 factored in     : %lld ms -> %llu * %llu\n", (long long)ms,
           (unsigned long long)hard[0].first, (unsigned long long)hard[1].first);
}
