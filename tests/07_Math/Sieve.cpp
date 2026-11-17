// Independent test for 07_Math/Sieve.cpp. Every public function is compared against an oracle
// written here from scratch: plain trial division for small values, deterministic Miller-Rabin
// (independent of trial division) for values near 2^31, 1e12 and 1e18.
#include "../../07_Math/Sieve.cpp"

static map<string, long long> cover;
static long long mismatches = 0;
#define CHECK(name, cond, ...)                                                   \
    do {                                                                         \
        cover[name]++;                                                           \
        if (!(cond)) {                                                           \
            if (++mismatches <= 20) cerr << "MISMATCH " << name << ": " << __VA_ARGS__ << '\n';   \
        }                                                                        \
    } while (0)

// ---- oracles ----
static long long o_spf(long long x) { // smallest d >= 2 dividing x, by testing every d
    for (long long d = 2; d * d <= x; d++)
        if (x % d == 0) return d;
    return x;
}
static unsigned long long mulmod(unsigned long long a, unsigned long long b, unsigned long long m) {
    return (unsigned long long)((__uint128_t)a * b % m);
}
static unsigned long long powmod(unsigned long long a, unsigned long long e, unsigned long long m) {
    unsigned long long r = 1 % m;
    a %= m;
    while (e) {
        if (e & 1) r = mulmod(r, a, m);
        a = mulmod(a, a, m);
        e >>= 1;
    }
    return r;
}
static bool o_mr(unsigned long long n) { // deterministic for 64-bit
    if (n < 2) return false;
    for (unsigned long long p : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL})
        if (n % p == 0) return n == p;
    unsigned long long d = n - 1;
    int s = 0;
    while (d % 2 == 0) d /= 2, s++;
    for (unsigned long long a : {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL}) {
        unsigned long long x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; r++) {
            x = mulmod(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}
static vector<pair<long long, long long>> o_factor(long long x) {
    vector<pair<long long, long long>> out;
    for (long long d = 2; d * d <= x; d++) {
        if (x % d) continue;
        long long e = 0;
        while (x % d == 0) x /= d, e++;
        out.push_back({d, e});
    }
    if (x > 1) out.push_back({x, 1});
    return out;
}
static vector<long long> o_divisors(long long x) {
    vector<long long> out;
    for (long long d = 1; d <= x; d++) {
        if (d * d > x) break;
        if (x % d == 0) {
            out.push_back(d);
            if (d != x / d) out.push_back(x / d);
        }
    }
    sort(out.begin(), out.end());
    return out;
}
template <class A, class B> static bool same(const vector<A> &a, const vector<B> &b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++)
        if ((long long)a[i] != (long long)b[i]) return false;
    return true;
}

signed main() {
    mt19937_64 rng(12345);
    const long long LIM = 1000000;

    // oracle prime table to 1e6 by trial division
    vector<char> isP(LIM + 1);
    vector<long long> spfO(LIM + 1, 0);
    for (long long x = 2; x <= LIM; x++) spfO[x] = o_spf(x), isP[x] = (spfO[x] == x);
    vector<long long> oPrimes;
    for (long long x = 2; x <= LIM; x++)
        if (isP[x]) oPrimes.push_back(x);
    // sanity: the two independent oracles agree
    for (long long x = 0; x <= 200000; x++) CHECK("oracle_self", o_mr(x) == (bool)isP[x], "oracle disagree " << x);

    // ---- is_prime ----
    for (long long x = -5; x <= LIM; x++) CHECK("is_prime", is_prime(x) == (x >= 2 && isP[x]), "x=" << x);
    const long long P31 = 1LL << 31;
    for (long long x = P31 - 3000; x <= P31 + 3000; x++) CHECK("is_prime", is_prime(x) == o_mr(x), "x=" << x);
    for (long long x = 1000000000000LL - 2000; x <= 1000000000000LL + 2000; x++)
        CHECK("is_prime", is_prime(x) == o_mr(x), "x=" << x);
    {
        long long big[] = {100000000000000003LL, 999999999999999989LL, 999999937LL * 999999929LL, 2147483647LL,
                           4611686014132420609LL /* (2^31-1)^2 */};
        for (long long x : big) CHECK("is_prime", is_prime(x) == o_mr(x), "big x=" << x);
        CHECK("oracle_self", o_mr(100000000000000003LL) && o_mr(999999999999999989LL) && !o_mr(999999937LL * 999999929LL),
              "known 18-digit primes");
    }

    // ---- primes_up_to ----
    for (int n = -3; n <= 400; n++) {
        vector<long long> exp;
        for (long long p : oPrimes)
            if (p <= n) exp.push_back(p);
        CHECK("primes_up_to", same(primes_up_to(n), exp), "n=" << n);
    }
    for (int n : vector<int>{46340, 46341, 46342, 99991, 99992, (int)LIM}) { // i*i passes 2^31 for i > 46340
        vector<long long> exp;
        for (long long p : oPrimes)
            if (p <= n) exp.push_back(p);
        CHECK("primes_up_to", same(primes_up_to(n), exp), "n=" << n);
    }

    // ---- segmented_sieve ----
    auto seg_check = [&](long long lo, long long hi) {
        vector<long long> exp;
        for (long long v = max(lo, 0LL); v <= hi; v++)
            if (o_mr(v)) exp.push_back(v);
        CHECK("segmented_sieve", same(segmented_sieve(lo, hi), exp), "[" << lo << "," << hi << "]");
    };
    for (long long lo = -3; lo <= 40; lo++)
        for (long long hi = -3; hi <= 60; hi++) seg_check(lo, hi);
    for (int t = 0; t < 300; t++) {
        long long lo = rng() % LIM, hi = lo + rng() % 3000;
        seg_check(lo, hi);
    }
    seg_check(P31 - 100000, P31 + 100000);
    seg_check(1000000000000LL - 50000, 1000000000000LL + 50000);
    seg_check(46340LL * 46340 - 500, 46341LL * 46341 + 500);

    // ---- Sieve struct ----
    for (int n = 0; n <= 60; n++) { // small tables, every member
        Sieve s(n);
        CHECK("Sieve.n", s.n == n, "n=" << n);
        CHECK("Sieve.spf", (int)s.spf.size() == n + 1, "size n=" << n);
        vector<long long> exp;
        for (long long p : oPrimes)
            if (p <= n) exp.push_back(p);
        CHECK("Sieve.primes", same(s.primes, exp), "n=" << n);
        for (int x = 0; x <= n; x++) CHECK("Sieve.spf", s.spf[x] == (x < 2 ? 0 : spfO[x]), "n=" << n << " x=" << x);
        for (int x = -2; x <= n + 5; x++)
            CHECK("Sieve.prime", s.prime(x) == (x >= 2 && x <= n && isP[x]), "n=" << n << " x=" << x);
    }
    {
        Sieve s((int)LIM);
        CHECK("Sieve.primes", same(s.primes, oPrimes), "n=1e6");
        for (long long x = 0; x <= LIM; x++) CHECK("Sieve.spf", s.spf[x] == (x < 2 ? 0 : spfO[x]), "x=" << x);
        for (long long x = -2; x <= LIM + 2; x++)
            CHECK("Sieve.prime", s.prime((int)x) == (x >= 2 && x <= LIM && isP[x]), "x=" << x);
        for (long long x = 0; x <= LIM; x++) {
            auto f = s.factorize((int)x);
            auto exp = x <= 1 ? vector<pair<long long, long long>>{} : o_factor(x);
            bool ok = f.size() == exp.size();
            for (size_t i = 0; ok && i < f.size(); i++) ok = f[i].first == exp[i].first && f[i].second == exp[i].second;
            CHECK("Sieve.factorize", ok, "x=" << x);
            vector<long long> pf;
            for (auto [p, e] : exp)
                for (long long k = 0; k < e; k++) pf.push_back(p);
            CHECK("Sieve.prime_factors", same(s.prime_factors((int)x), pf), "x=" << x);
            int mu = 1;
            for (auto [p, e] : exp) mu = (e > 1) ? 0 : -mu;
            if (x >= 1) CHECK("Sieve.mobius", s.mobius((int)x) == mu, "x=" << x);
        }
        for (long long x = 1; x <= 100000; x++) CHECK("Sieve.divisors", same(s.divisors((int)x), o_divisors(x)), "x=" << x);
        for (long long x : {720720LL, 997920LL, 1000000LL, 999983LL, 524288LL, 531441LL})
            CHECK("Sieve.divisors", same(s.divisors((int)x), o_divisors(x)), "x=" << x);
        for (int t = 0; t < 20000; t++) {
            long long x = 1 + rng() % LIM;
            CHECK("Sieve.divisors", same(s.divisors((int)x), o_divisors(x)), "x=" << x);
        }
    }

    // ---- timing (meaningful in the -O2 build) ----
    {
        auto t0 = chrono::steady_clock::now();
        Sieve s(10000000);
        auto t1 = chrono::steady_clock::now();
        auto pr = primes_up_to(10000000);
        auto t2 = chrono::steady_clock::now();
        CHECK("Sieve.primes", s.primes.size() == 664579 && pr.size() == 664579, "pi(1e7)");
        printf("time Sieve(1e7) = %.3f s, primes_up_to(1e7) = %.3f s\n", chrono::duration<double>(t1 - t0).count(),
               chrono::duration<double>(t2 - t1).count());
    }

    for (auto &[k, v] : cover) printf("COVER %-22s %lld checks\n", k.c_str(), v);
    printf("mismatches = %lld\n", mismatches);
    puts(mismatches == 0 ? "RESULT: PASS" : "RESULT: FAIL");
    return mismatches == 0 ? 0 : 1;
}
