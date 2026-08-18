// Brute-force test for:
//   Math/Number_Theory/Sieve.cpp
//   Math/Number_Theory/Divisors_And_Factors.cpp
//   Math/Number_Theory/Euler_Phi.cpp
//   Math/Number_Theory/Fibonacci.cpp
//   Math/Number_Theory/Base_Conversion.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

namespace SV {
#include "../Math/Number_Theory/Sieve.cpp"
}
namespace DF {
#include "../Math/Number_Theory/Divisors_And_Factors.cpp"
}
namespace PH {
#include "../Math/Number_Theory/Euler_Phi.cpp"
}
namespace FB {
#include "../Math/Number_Theory/Fibonacci.cpp"
}
namespace BC {
#include "../Math/Number_Theory/Base_Conversion.cpp"
}

int main() {
    mt19937_64 gen(9001);
    int primeFail = 0, spfFail = 0, factSieveFail = 0, divSieveFail = 0, mobFail = 0;
    int factFail = 0, divFail = 0, cntFail = 0, sumFail = 0, totCntFail = 0, totSumFail = 0;
    int phiFail = 0, phiTabFail = 0, phiFacFail = 0, phiSumFail = 0;
    int fibFail = 0, fibMatFail = 0, recFail = 0, fibHugeFail = 0;
    int b2sFail = 0, s2bFail = 0, roundFail = 0, validFail = 0;

    const int LIM = 3000;
    SV::Sieve sv(LIM);
    vector<ll> phitab = PH::phi_table(LIM);

    // ---------- sieve ----------
    for (int x = 0; x <= LIM; x++) {
        bool want = SV::is_prime(x);
        if (sv.prime(x) != want) primeFail++;
        if (x >= 2 && sv.spf[x] == 0) spfFail++;
        if (x >= 2) {
            // spf really is the smallest prime factor
            int s = -1;
            for (int p = 2; (ll)p * p <= x || p <= x; p++)
                if (x % p == 0) { s = p; break; }
            if (sv.spf[x] != s) spfFail++;
        }
        if (x >= 1) {
            // factorize multiplies back to x, and every base is prime with a positive exponent
            ll prod = 1;
            for (auto [p, e] : sv.factorize(x)) {
                if (!SV::is_prime(p) || e < 1) factSieveFail++;
                for (int i = 0; i < e; i++) prod *= p;
            }
            if (prod != x) factSieveFail++;
            // prime_factors with repetition multiplies back too
            ll prod2 = 1;
            for (int p : sv.prime_factors(x)) prod2 *= p;
            if (prod2 != x) factSieveFail++;
            // divisors: exactly the values dividing x, ascending, no duplicates
            vector<int> got = sv.divisors(x), want2;
            for (int d = 1; d <= x; d++)
                if (x % d == 0) want2.push_back(d);
            if (got != want2) divSieveFail++;
            // mobius against its definition
            int mu = 1;
            bool squarefree = true;
            for (auto [p, e] : sv.factorize(x)) {
                (void)p;
                if (e > 1) squarefree = false;
                mu = -mu;
            }
            int wantMu = x == 1 ? 1 : (squarefree ? mu : 0);
            if (sv.mobius(x) != wantMu) mobFail++;
        }
    }
    // primes_up_to must match the struct's list
    if (SV::primes_up_to(LIM) != sv.primes) primeFail++;

    // ---------- divisors and factors, against the sieve ----------
    for (int it = 0; it < 4000; it++) {
        ll n = 1 + (ll)(gen() % LIM);
        // factorize agrees with the sieve's
        auto a = DF::factorize(n);
        auto b = sv.factorize((int)n);
        if (a.size() != b.size()) factFail++;
        else
            for (size_t i = 0; i < a.size(); i++)
                if (a[i].first != b[i].first || a[i].second != b[i].second) factFail++;
        // divisors agree, and are ascending
        vector<ll> dv = DF::divisors(n);
        vector<int> dv2 = sv.divisors((int)n);
        if (dv.size() != dv2.size()) divFail++;
        else
            for (size_t i = 0; i < dv.size(); i++)
                if (dv[i] != dv2[i]) divFail++;
        if (!is_sorted(dv.begin(), dv.end())) divFail++;
        // counts and sums against the explicit list
        ll wantCnt = (ll)dv.size(), wantSum = 0;
        for (ll d : dv) wantSum += d;
        if (DF::count_divisors(n) != wantCnt) cntFail++;
        if (DF::sum_divisors(n) != wantSum) sumFail++;
        // prime_factors with repetition
        ll prod = 1;
        for (ll p : DF::prime_factors(n)) prod *= p;
        if (prod != n) factFail++;
    }
    // range totals against a direct double loop
    for (ll n = 1; n <= 300; n++) {
        ll wantC = 0, wantS = 0;
        for (ll i = 1; i <= n; i++)
            for (ll d = 1; d <= i; d++)
                if (i % d == 0) wantC++, wantS += d;
        if (DF::total_divisor_count(n) != wantC) totCntFail++;
        if (DF::total_divisor_sum(n) != wantS) totSumFail++;
    }
    // a large single factorization
    {
        ll big = 999999999989LL; // prime
        auto f = DF::factorize(big);
        if (f.size() != 1 || f[0].first != big || f[0].second != 1) factFail++;
        ll composite = 1000000007LL * 3;
        auto g = DF::factorize(composite);
        ll prod = 1;
        for (auto [p, e] : g)
            for (int i = 0; i < e; i++) prod *= p;
        if (prod != composite) factFail++;
    }

    // ---------- Euler phi ----------
    for (int n = 1; n <= LIM; n++) {
        // definition: count of i in 1..n with gcd(i, n) == 1
        ll want = 0;
        for (int i = 1; i <= n; i++)
            if (__gcd(i, n) == 1) want++;
        if (PH::phi(n) != want) phiFail++;
        if (phitab[n] != want) phiTabFail++;
        if (PH::phi_from_factors(DF::factorize(n)) != want) phiFacFail++;
        if (n <= 200) {
            ll s = 0;
            for (int i = 1; i <= n; i++) s += want, want = want; // placeholder, replaced below
            (void)s;
        }
    }
    {
        // phi_sum against a direct sum
        ll want = 0;
        for (int i = 1; i <= 500; i++) {
            ll c = 0;
            for (int j = 1; j <= i; j++)
                if (__gcd(i, j) == 1) c++;
            want += c;
        }
        if (PH::phi_sum(500) != want) phiSumFail++;
    }

    // ---------- Fibonacci ----------
    for (int it = 0; it < 3000; it++) {
        ll n = (ll)(gen() % 400);
        ll m = 1 + (ll)(gen() % 100000);
        ll want = FB::fib_linear(n, m);
        if (FB::fib(n, m) != want) fibFail++;
        if (FB::fib_matrix(n, m) != want) fibMatFail++;
        // fib_pair must give consecutive terms
        auto [a, b] = FB::fib_pair(n, m);
        if (a != want) fibFail++;
        if (b != FB::fib_linear(n + 1, m)) fibFail++;
        // linear_recurrence with p = q = 1 and g0, g1 = 0, 1 IS Fibonacci
        if (FB::linear_recurrence(1, 1, 0, 1, n, m) != want) recFail++;
    }
    // a general recurrence: g(n) = 2g(n-1) + 3g(n-2), g0 = 1, g1 = 2
    for (ll n = 0; n <= 40; n++) {
        ll m = 1000000007;
        ll g0 = 1, g1 = 2, prev = g0, cur = g1;
        ll want = (n == 0) ? g0 : (n == 1 ? g1 : 0);
        for (ll i = 2; i <= n; i++) {
            ll nxt = (2 * cur + 3 * prev) % m;
            prev = cur, cur = nxt;
        }
        if (n >= 2) want = cur;
        if (FB::linear_recurrence(2, 3, g0, g1, n, m) != want % m) recFail++;
    }
    // huge n must not hang or overflow
    {
        ll m = 1000000007;
        ll got = FB::fib(1000000000000000000LL, m);
        if (got < 0 || got >= m) fibHugeFail++;
        // F(n) mod m must satisfy the identity F(2k) = F(k)*(2F(k+1) - F(k))
        ll k = 123456789012345LL;
        auto [a, b] = FB::fib_pair(k, m);
        ll want2k = (ll)((__int128)a * (((__int128)2 * b - a + 2 * m) % m) % m);
        if (FB::fib(2 * k, m) != want2k) fibHugeFail++;
    }

    // ---------- base conversion ----------
    for (int it = 0; it < 40000; it++) {
        ll v = (ll)(gen() % 2000000000ULL) - 1000000000LL;
        int base = 2 + (int)(gen() % 35);
        string s = BC::to_base(v, base);
        if (BC::from_base(s, base) != v) roundFail++;
        if (!BC::valid_in_base(s, base)) validFail++;
        // base 10 must match to_string
        if (base == 10 && s != to_string(v)) b2sFail++;
        // base 16 must match printf for non-negative values
        if (base == 16 && v >= 0) {
            char buf[64];
            snprintf(buf, sizeof buf, "%llX", (unsigned long long)v);
            if (s != string(buf)) b2sFail++;
        }
        // lower case input must parse the same
        string lower = s;
        for (char &c : lower) c = (char)tolower((unsigned char)c);
        if (BC::from_base(lower, base) != v) s2bFail++;
    }
    if (BC::to_base(0, 2) != "0" || BC::to_base(0, 36) != "0") b2sFail++;
    if (BC::from_base("0", 7) != 0) s2bFail++;
    if (BC::to_base(255, 16) != "FF") b2sFail++;
    if (BC::from_base("ff", 16) != 255) s2bFail++;
    if (BC::to_base(-255, 16) != "-FF") b2sFail++;
    if (BC::valid_in_base("12", 2)) validFail++; // '2' is not a base-2 digit

    cout << "Number_Theory batch B\n";
    cout << "  is_prime / sieve prime agree   : " << primeFail << " failures\n";
    cout << "  smallest prime factor table    : " << spfFail << " failures\n";
    cout << "  sieve factorize multiplies back: " << factSieveFail << " failures\n";
    cout << "  sieve divisors vs direct scan  : " << divSieveFail << " failures\n";
    cout << "  mobius                         : " << mobFail << " failures\n";
    cout << "  factorize agrees with sieve    : " << factFail << " failures\n";
    cout << "  divisors agree, ascending      : " << divFail << " failures\n";
    cout << "  count_divisors                 : " << cntFail << " failures\n";
    cout << "  sum_divisors                   : " << sumFail << " failures\n";
    cout << "  total_divisor_count            : " << totCntFail << " failures\n";
    cout << "  total_divisor_sum              : " << totSumFail << " failures\n";
    cout << "  phi vs its definition          : " << phiFail << " failures\n";
    cout << "  phi_table                      : " << phiTabFail << " failures\n";
    cout << "  phi_from_factors               : " << phiFacFail << " failures\n";
    cout << "  phi_sum                        : " << phiSumFail << " failures\n";
    cout << "  fib fast doubling vs linear    : " << fibFail << " failures\n";
    cout << "  fib_matrix agrees              : " << fibMatFail << " failures\n";
    cout << "  linear_recurrence              : " << recFail << " failures\n";
    cout << "  fib at n = 1e18                : " << fibHugeFail << " failures\n";
    cout << "  to_base round trip             : " << roundFail << " failures\n";
    cout << "  to_base vs to_string / printf  : " << b2sFail << " failures\n";
    cout << "  from_base, lower case          : " << s2bFail << " failures\n";
    cout << "  valid_in_base                  : " << validFail << " failures\n";
}
