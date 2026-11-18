// Test for 07_Math/Fibonacci.cpp against an iterative __int128 table for small n, the Pisano
// period (found by brute force) for huge n with small m, and naive 2x2 matrix products for matpow.
// Every public function is checked: fibonacci, mul, matpow, fib, fib_pair (with and without m).
#include "../../07_Math/Fibonacci.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(29);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    vector<ll> mods = {1, 2, 10, 1000, 1000000007, 998244353, (1LL << 61) - 1, LLONG_MAX, 4611686018427387847LL};
    for (ll m : mods) {
        vector<ll> F(3001);
        F[0] = 0, F[1] = 1 % m;
        for (int i = 2; i <= 3000; i++) F[i] = (ll)(((__int128)F[i - 1] + F[i - 2]) % m);
        for (int n = 0; n < 3000; n++) {
            chk("fibonacci", fibonacci(n, m) == F[n]);
            chk("fib", fib(n, m) == F[n]);
            auto p = fib_pair(n, m);
            chk("fib_pair", p.first == F[n] && p.second == F[n + 1]);
        }
    }
    { // exact values that fit a long long (m larger than F(92))
        ll a = 0, b = 1;
        for (int n = 0; n <= 91; n++) {
            chk("fib(exact)", fib(n, LLONG_MAX) == a && fib_pair(n, LLONG_MAX).first == a);
            if (n == 91) break; // F(93) does not fit
            ll t = a + b;
            a = b, b = t;
        }
    }
    // default modulus 1e9 + 7
    for (int n = 0; n < 2000; n++)
        chk("default m", fib(n) == fibonacci(n, 1000000007) && fib_pair(n).first == fibonacci(n) &&
                             fibonacci(n) == fibonacci(n, 1000000007));
    // huge n through the Pisano period
    for (ll m = 1; m <= 300; m++) {
        ll a = 0, b = 1 % m, per = 0;
        do {
            ll t = (a + b) % m;
            a = b, b = t, per++;
        } while (!(a == 0 && b == 1 % m));
        for (int it = 0; it < 30; it++) {
            ll n = (ll)(rng() >> 1);
            ll want = fibonacci(n % per, m);
            chk("fib(huge n)", fib(n, m) == want);
            chk("fib_pair(huge n)", fib_pair(n, m).first == want && fib_pair(n, m).second == fibonacci(n % per + 1, m));
        }
    }
    // huge n, big m: matrix and doubling must agree, and F(2n) = F(n) * (2F(n+1) - F(n))
    for (int it = 0; it < 20000; it++) {
        ll m = mods[4 + rng() % 5], n = (ll)(rng() >> 2);
        auto p = fib_pair(n, m), q = fib_pair(2 * n, m);
        __int128 d = ((__int128)2 * p.second - p.first) % m;
        if (d < 0) d += m;
        chk("fib vs fib_pair(huge)", fib(n, m) == p.first && q.first == (ll)((__int128)p.first * d % m));
    }
    // matpow on random matrices vs naive repeated products
    for (int it = 0; it < 20000; it++) {
        ll m = it % 2 ? mods[rng() % mods.size()] : (ll)(rng() >> 1) + 1;
        matrix A;
        for (auto &r : A.mat)
            for (auto &v : r) v = (ll)(rng() % (unsigned long long)m);
        ll e = rng() % 30;
        __int128 R[2][2] = {{1 % m, 0}, {0, 1 % m}};
        for (ll t = 0; t < e; t++) {
            __int128 N[2][2];
            for (int i = 0; i < 2; i++)
                for (int j = 0; j < 2; j++)
                    N[i][j] = ((R[i][0] * A.mat[0][j]) % m + (R[i][1] * A.mat[1][j]) % m) % m;
            memcpy(R, N, sizeof R);
        }
        matrix P = matpow(A, e, m), M = mul(A, A, m);
        bool ok = true, ok2 = true;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++) {
                ok &= P.mat[i][j] == (ll)R[i][j];
                ok2 &= M.mat[i][j] == (ll)(((__int128)A.mat[i][0] * A.mat[0][j] % m + (__int128)A.mat[i][1] * A.mat[1][j] % m) % m);
            }
        chk("matpow", ok);
        chk("mul", ok2);
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 1000000; i++) s ^= fib_pair((ll)(rng() >> 1)).first;
        for (int i = 0; i < 300000; i++) s ^= fib((ll)(rng() >> 1));
        printf("1e6 fib_pair + 3e5 fib (n~4e18): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
