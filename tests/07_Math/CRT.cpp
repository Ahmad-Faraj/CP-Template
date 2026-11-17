// Independent test for 07_Math/CRT.cpp. Oracles: brute-force search for the smallest x over small moduli,
// and for large moduli a planted answer x0 (remainders are derived from x0, so the unique answer mod lcm is x0).
#include "../../07_Math/CRT.cpp"

static map<string, long long> cover;
static long long mismatches = 0;
#define CHECK(name, cond, ...)                                                   \
    do {                                                                         \
        cover[name]++;                                                           \
        if (!(cond)) {                                                           \
            if (++mismatches <= 20) cerr << "MISMATCH " << name << ": " << __VA_ARGS__ << '\n'; \
        }                                                                        \
    } while (0)

typedef __int128 i128;
static long long o_gcd(long long a, long long b) { // plain Euclid, written here
    while (b) {
        long long t = a % b;
        a = b, b = t;
    }
    return a;
}
static long long o_mod(long long r, long long m) { return ((r % m) + m) % m; }
static string show(const vector<long long> &r, const vector<long long> &m) {
    string s;
    for (size_t i = 0; i < r.size(); i++) s += "x=" + to_string(r[i]) + " mod " + to_string(m[i]) + "; ";
    return s;
}

signed main() {
    mt19937_64 rng(777);
    auto rnd = [&](long long lo, long long hi) { return lo + (long long)(rng() % (unsigned long long)(hi - lo + 1)); };

    // ---- extended_gcd ----
    for (long long a = 0; a <= 60; a++)
        for (long long b = 0; b <= 60; b++) {
            long long x, y, g = extended_gcd(a, b, x, y);
            CHECK("extended_gcd", g == o_gcd(a, b) && (i128)a * x + (i128)b * y == g, "a=" << a << " b=" << b);
        }
    for (int t = 0; t < 200000; t++) {
        long long a = rng() >> rnd(1, 63), b = rng() >> rnd(1, 63);
        long long x, y, g = extended_gcd(a, b, x, y);
        CHECK("extended_gcd", g == o_gcd(a, b) && (i128)a * x + (i128)b * y == g, "a=" << a << " b=" << b);
    }

    // ---- crt_pair, small: compare with brute force over [0, lcm) ----
    for (long long m1 = 1; m1 <= 24; m1++)
        for (long long m2 = 1; m2 <= 24; m2++)
            for (long long r1 = 0; r1 < m1; r1++)
                for (long long r2 = 0; r2 < m2; r2++) {
                    long long L = m1 / o_gcd(m1, m2) * m2, want = -1;
                    for (long long x = 0; x < L; x++)
                        if (x % m1 == r1 && x % m2 == r2) { want = x; break; }
                    bool ok = true;
                    auto [r, m] = crt_pair(r1, m1, r2, m2, ok);
                    if (want < 0) CHECK("crt_pair", !ok, r1 << " " << m1 << " " << r2 << " " << m2 << " expected no solution");
                    else CHECK("crt_pair", ok && r == want && m == L, r1 << " " << m1 << " " << r2 << " " << m2);
                }

    // ---- crt, small systems of 0..5 congruences, remainders may be negative or >= modulus ----
    long long noSol = 0, withSol = 0;
    for (int t = 0; t < 60000; t++) {
        int k = t < 10 ? 0 : (int)rnd(1, 5);
        vector<long long> r(k), m(k);
        for (int i = 0; i < k; i++) m[i] = rnd(1, 12), r[i] = rnd(-40, 40);
        long long L = 1;
        for (int i = 0; i < k; i++) L = L / o_gcd(L, m[i]) * m[i];
        if (L > 3000) continue; // keep the brute force cheap
        long long want = -1;
        for (long long x = 0; x < L; x++) {
            bool good = true;
            for (int i = 0; i < k && good; i++) good = o_mod(x - r[i], m[i]) == 0;
            if (good) { want = x; break; }
        }
        bool ok = false;
        auto [rem, mod] = crt(r, m, ok);
        if (want < 0) {
            CHECK("crt", !ok, show(r, m) << "expected no solution");
            noSol++;
        } else {
            CHECK("crt", ok && rem == want && mod == L, show(r, m) << " got " << rem << " mod " << mod);
            withSol++;
        }
    }
    cover["crt(no solution cases)"] = noSol, cover["crt(solvable cases)"] = withSol;

    // ---- crt and crt_pair, large: planted answer, lcm up to just under 2^63 ----
    const long long PR[] = {1000000007LL, 998244353LL, 2147483647LL, 3037000493LL /* ~sqrt(2^63) */, 1000000009LL};
    for (int t = 0; t < 100000; t++) {
        long long m1, m2;
        int kind = t % 4;
        if (kind == 0) { m1 = PR[rnd(0, 4)], m2 = PR[rnd(0, 4)]; }            // coprime primes (or equal)
        else if (kind == 1) { long long g = rnd(1, 1000000); m1 = g * rnd(1, 2000000000); m2 = g * rnd(1, 2000000); } // non-coprime
        else if (kind == 2) { m1 = 3037000493LL, m2 = 3037000453LL; }        // lcm ~ 9.22e18, close to 2^63
        else { m1 = rnd(1, 4000000000LL), m2 = rnd(1, 2000000000LL); }
        i128 L = (i128)m1 / o_gcd(m1, m2) * m2;
        if (L > (i128)LLONG_MAX) { t--; continue; }
        long long x0 = (long long)((((unsigned __int128)rng() << 64) | rng()) % (unsigned __int128)L);
        long long r1 = x0 % m1, r2 = x0 % m2;
        bool ok = false;
        auto [r, m] = crt_pair(r1, m1, r2, m2, ok);
        CHECK("crt_pair", ok && r == x0 && m == (long long)L, "big " << r1 << " " << m1 << " " << r2 << " " << m2);
        // the same system through crt, with negative and shifted remainders
        vector<long long> rr = {r1 - m1 * rnd(0, 3), r2 + m2 * rnd(0, 3)}, mm = {m1, m2};
        auto [r3, m3] = crt(rr, mm, ok);
        CHECK("crt", ok && r3 == x0 && m3 == (long long)L, "big " << show(rr, mm));
        // break it: shift r2 by a non-multiple of gcd
        long long g = o_gcd(m1, m2);
        if (g > 1) {
            bool ok2 = true;
            crt_pair(r1, m1, (r2 + 1) % m2, m2, ok2);
            CHECK("crt_pair", !ok2, "big inconsistent");
            vector<long long> rb = {r1, r2 + 1};
            crt(rb, mm, ok2);
            CHECK("crt", !ok2, "big inconsistent " << show(rb, mm));
        }
    }
    // many coprime moduli whose lcm ends just under 2^63
    {
        vector<long long> m = {4, 9, 25, 49, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43}; // product ~ 2.75e18 < 2^63
        i128 L = 1;
        for (long long v : m) L *= v;
        if (L <= (i128)LLONG_MAX) {
            for (int t = 0; t < 2000; t++) {
                long long x0 = (long long)((((unsigned __int128)rng() << 64) | rng()) % (unsigned __int128)L);
                vector<long long> r;
                for (long long v : m) r.push_back(x0 % v - (long long)rnd(0, 2) * v);
                bool ok = false;
                auto [rem, mod] = crt(r, m, ok);
                CHECK("crt", ok && rem == x0 && mod == (long long)L, "16 moduli x0=" << x0);
            }
        } else
            CHECK("crt", false, "test setup: product overflows");
    }

    for (auto &[k, v] : cover) printf("COVER %-26s %lld checks\n", k.c_str(), v);
    printf("mismatches = %lld\n", mismatches);
    puts(mismatches == 0 ? "RESULT: PASS" : "RESULT: FAIL");
    return mismatches == 0 ? 0 : 1;
}
