// Test for 07_Math/NTT.cpp against naive O(n*m) convolution mod 998244353 and brute-force orders.
// Every public function is checked: NTT::mod_pow, NTT::ntt (against a naive DFT over Z_p, and the
// inverse), NTT::multiply, NTT::poly_pow_mod, generator.
#include "../../07_Math/NTT.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(53);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
const ll P = 998244353;

ll md(ll x) { return (x % P + P) % P; }
ll slow_pow(ll b, ll e, ll m) { // square-and-multiply in __int128, independent of mod_pow
    __int128 r = 1 % m, x = ((b % m) + m) % m;
    for (; e; e >>= 1, x = x * x % m)
        if (e & 1) r = r * x % m;
    return (ll)r;
}
vector<ll> naive(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    vector<ll> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] = (c[i + j] + (__int128)md(a[i]) * md(b[j])) % P;
    return c;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        ll b = rnd(-1e18, 1e18), e = rnd(0, 1e18), m = rnd(1, 2e9);
        if (it % 3 == 0) m = P;
        if (b < 0) b = -b;
        chk("mod_pow", NTT::mod_pow(b, e, m) == slow_pow(b, e, m));
    }
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 50, m = rng() % 50;
        if (it % 7 == 0) n = 1;
        vector<ll> a(n), b(m);
        int kind = rng() % 3; // small, full range, outside [0, P) incl. negatives
        for (auto *v : {&a, &b})
            for (auto &x : *v) x = kind == 0 ? rnd(0, 9) : kind == 1 ? rnd(0, P - 1) : rnd(-4 * P, 4 * P);
        chk(n && m ? "multiply" : "multiply(empty)", NTT::multiply(a, b) == naive(a, b));
    }
    for (int it = 0; it < 300; it++) { // ntt is the DFT at w = 3^((P-1)/n), natural order
        int n = 1 << (rng() % 8);
        vector<ll> a(n);
        for (auto &x : a) x = rnd(0, P - 1);
        vector<ll> f = a;
        NTT::ntt(f, false);
        ll w = slow_pow(3, (P - 1) / n, P);
        bool ok = true;
        for (int k = 0; k < n; k++) {
            __int128 s = 0;
            for (int j = 0; j < n; j++) s += (__int128)a[j] * slow_pow(w, (ll)j * k, P);
            ok &= (ll)(s % P) == f[k];
        }
        chk("ntt(forward)", ok);
        NTT::ntt(f, true);
        chk("ntt(inverse)", f == a);
    }
    for (int it = 0; it < 2000; it++) {
        int n = rng() % 6;
        ll e = rng() % 9;
        vector<ll> a(n);
        for (auto &x : a) x = rnd(-P, 2 * P);
        vector<ll> want{1};
        for (int k = 0; k < e; k++) want = naive(want, a);
        chk("poly_pow_mod", NTT::poly_pow_mod(a, e) == want);
    }
    { // a long power: (1 + x)^e has binomial coefficients
        ll e = 100000;
        auto c = NTT::poly_pow_mod({1, 1}, e);
        bool ok = (ll)c.size() == e + 1;
        ll cur = 1; // C(e, k) mod P by the multiplicative recurrence
        for (ll k = 0; ok && k <= e; k++) {
            ok &= c[k] == cur;
            cur = (__int128)cur * md(e - k) % P * slow_pow(k + 1, P - 2, P) % P;
        }
        chk("poly_pow_mod(binomial)", ok);
    }
    // generator: smallest g whose multiplicative order is p - 1, found by brute force
    auto brute_gen = [](ll p) {
        for (ll g = 1; g < p; g++) {
            ll x = 1, ord = 0;
            do x = x * g % p, ord++;
            while (x != 1);
            if (ord == p - 1) return g;
        }
        return -1LL;
    };
    for (ll p = 2; p < 3000; p++) {
        bool pr = p > 1;
        for (ll d = 2; d * d <= p; d++) pr &= p % d != 0;
        if (pr) chk("generator", generator(p) == brute_gen(p));
    }
    chk("generator(known)", generator(998244353) == 3 && generator(167772161) == 3 &&
                                generator(469762049) == 3 && generator(754974721) == 11 &&
                                generator(1000000007) == 5);
    { // large: padded length 2^23 (the maximum), sampled indices against naive dot products
        int n = 1 << 22;
#ifdef __SANITIZE_ADDRESS__
        n = 1 << 18;
#endif
        vector<ll> a(n), b(n);
        for (auto &x : a) x = rnd(0, P - 1);
        for (auto &x : b) x = rnd(0, P - 1);
        auto t0 = clock();
        auto c = NTT::multiply(a, b);
        printf("multiply %d x %d (padded %d): %.3fs\n", n, n, 2 * n, double(clock() - t0) / CLOCKS_PER_SEC);
        bool ok = (int)c.size() == 2 * n - 1;
        for (int s = 0; s < 40 && ok; s++) {
            int k = s == 0 ? 0 : s == 1 ? 2 * n - 2 : rng() % (2 * n - 1);
            __int128 v = 0;
            for (int i = max<int>(0, k - n + 1); i <= min<int>(k, n - 1); i++) v += (__int128)a[i] * b[k - i];
            ok &= (ll)(v % P) == c[k];
        }
        chk("multiply(large)", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
