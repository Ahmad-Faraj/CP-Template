// Test for 07_Math/Euler_Phi.cpp against counting gcd(i, n) == 1 directly, and closed forms
// (p - 1, p^k - p^(k-1), (p - 1)(q - 1)) for large n. Every public function is checked: phi, phi_sieve.
#include "../../07_Math/Euler_Phi.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll brute(ll n) {
    ll c = 0;
    for (ll i = 1; i <= n; i++) c += __gcd(i, n) == 1;
    return c;
}

int main() {
    const int LIM = 3000;
    vector<ll> f = phi_sieve(LIM);
    chk("phi_sieve(size)", (int)f.size() == LIM + 1 && f[0] == 0);
    for (int n = 1; n <= LIM; n++) {
        ll b = brute(n);
        chk("phi", phi(n) == b);
        chk("phi_sieve", f[n] == b);
    }
    for (int M : {0, 1, 2, 3, 10}) { // small limits, compared with the big table
        vector<ll> g = phi_sieve(M);
        bool ok = (int)g.size() == M + 1;
        for (int i = 0; i <= M && ok; i++) ok &= g[i] == f[i];
        chk("phi_sieve(small MAX)", ok);
    }
    chk("phi(n<=0)", phi(0) == 0 && phi(-5) == -5);
    // closed forms for large n
    vector<ll> primes = {1000000007, 998244353, 1000003, 999983, 2, 3, 65537};
    for (ll p : primes) chk("phi(prime)", phi(p) == p - 1);
    for (ll p : {2LL, 3LL, 5LL, 7LL, 1000003LL}) {
        __int128 pk = p;
        for (int k = 1; pk <= LLONG_MAX; k++, pk *= p) chk("phi(p^k)", phi((ll)pk) == (ll)(pk - pk / p));
    }
    for (ll p : {1000003LL, 999983LL, 65537LL, 2LL})
        for (ll q : {1000000007LL, 998244353LL, 1000000009LL})
            chk("phi(p*q)", phi(p * q) == (p - 1) * (q - 1));
    // multiplicativity on random coprime pairs, both factors from the brute-checked table
    for (int it = 0; it < 3000; it++) {
        ll a = rng() % LIM + 1, b = rng() % LIM + 1;
        if (__gcd(a, b) == 1) chk("phi(multiplicative)", phi(a * b) == f[a] * f[b]);
    }
    {
        auto t0 = clock();
        vector<ll> big = phi_sieve(10000000);
        ll s = 0;
        for (int i = 0; i < 2000; i++) s += phi((ll)(rng() % 10000000000LL) + 1);
        printf("phi_sieve(1e7) + 2000 phi(n<=1e10): %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC,
               (s + big[9999991]) % 10);
        chk("phi_sieve(1e7 prime)", big[9999991] == 9999990);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
