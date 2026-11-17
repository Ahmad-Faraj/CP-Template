// Test for 07_Math/NTT_Any_Mod.cpp against naive O(n*m) convolution in __int128 reduced mod m.
// Every public function is checked: power, ntt (round trip), multiply_mod (per NTT prime), crt (against
// known x), multiply (many moduli, negatives, empty), plus a 2^18 x 2^18 precision run mod 1e9+7 with
// coefficients up to m - 1, checked in full against a closed form and at sampled indices naively.
#include "../../07_Math/NTT_Any_Mod.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(67);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
using T = NTT_Any_Mod;

vector<ll> naive(const vector<ll> &a, const vector<ll> &b, ll m) {
    if (a.empty() || b.empty()) return {};
    vector<__int128> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] += (__int128)a[i] * b[j]; // exact
    vector<ll> r(c.size());
    for (size_t i = 0; i < c.size(); i++) r[i] = (ll)((c[i] % m + m) % m);
    return r;
}
ll slow_pow(ll b, ll e, ll m) {
    __int128 r = 1 % m, x = b % m;
    for (; e; e >>= 1, x = x * x % m)
        if (e & 1) r = r * x % m;
    return (ll)r;
}

int main() {
    const ll MODS[] = {1000000007, 998244353, 1000000009, 2, 1, 7, 1 << 30, 999999999, 1000000};
    for (int it = 0; it < 10000; it++) {
        ll b = rnd(0, 2e9), e = rnd(0, 1e18), m = rnd(1, 2e9);
        chk("power", T::power(b, e, m) == slow_pow(b, e, m));
    }
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 40, m = rng() % 40;
        ll mod = MODS[rng() % 9];
        vector<ll> a(n), b(m);
        int kind = rng() % 3;
        for (auto *v : {&a, &b})
            for (auto &x : *v) x = kind == 0 ? rnd(0, 9) : kind == 1 ? rnd(0, mod - 1) : rnd(-3 * mod, 3 * mod);
        auto got = mod == 1000000007 && rng() % 2 ? T::multiply(a, b) : T::multiply(a, b, mod);
        chk(n && m ? "multiply" : "multiply(empty)", got == naive(a, b, mod));
    }
    const ll PR[3][2] = {{T::mod1, T::root1}, {T::mod2, T::root2}, {T::mod3, T::root3}};
    for (int it = 0; it < 3000; it++) {
        auto [p, g] = PR[it % 3];
        int n = rng() % 40 + 1, m = rng() % 40 + 1;
        vector<ll> a(n), b(m);
        for (auto &x : a) x = rnd(0, p - 1);
        for (auto &x : b) x = rnd(0, p - 1);
        chk("multiply_mod", T::multiply_mod(a, b, p, g) == naive(a, b, p));
        int len = 1 << (rng() % 10);
        vector<ll> v(len);
        for (auto &x : v) x = rnd(0, p - 1);
        auto f = v;
        T::ntt(f, p, g, false), T::ntt(f, p, g, true);
        chk("ntt(round trip)", f == v);
    }
    // roots really are primitive (order p - 1), so every power-of-two length up to 2^24 works
    for (auto [p, g] : PR) {
        ll phi = p - 1, x = phi;
        bool ok = true;
        for (ll q = 2; q * q <= x; q++)
            if (x % q == 0) {
                ok &= slow_pow(g, phi / q, p) != 1;
                while (x % q == 0) x /= q;
            }
        if (x > 1) ok &= slow_pow(g, phi / x, p) != 1;
        chk("roots primitive", ok && phi % (1 << 24) == 0);
    }
    __int128 M = (__int128)T::mod1 * T::mod2 * T::mod3;
    for (int it = 0; it < 20000; it++) {
        __int128 x = (__int128)(((unsigned __int128)rng() << 64 | rng()) % (unsigned __int128)M);
        if (it < 3) x = it == 0 ? 0 : it == 1 ? M - 1 : (__int128)1000000000000000000LL * 10000000; // edges, 1e25
        ll m = MODS[rng() % 9];
        chk("crt", T::crt(x % T::mod1, x % T::mod2, x % T::mod3, m) == (ll)(x % m));
    }
    { // precision: 2^18 x 2^18, coefficients up to m - 1 = 1e9 + 6
        int n = 1 << 18;
#ifdef __SANITIZE_ADDRESS__
        n = 1 << 15;
#endif
        const ll m = 1000000007;
        vector<ll> a(n, m - 1), b(n, m - 1); // c[k] = cnt(k) * (m-1)^2: true values up to 2.6e23
        auto c = T::multiply(a, b);
        bool ok = (int)c.size() == 2 * n - 1;
        ll sq = (ll)((__int128)(m - 1) * (m - 1) % m);
        for (int k = 0; ok && k < 2 * n - 1; k++) ok &= c[k] == (ll)((__int128)min(k + 1, 2 * n - 1 - k) * sq % m);
        chk("multiply(all m-1, 2^18)", ok);
        for (auto &x : a) x = rnd(m - 1000, m - 1);
        for (auto &x : b) x = rnd(0, m - 1);
        auto t0 = clock();
        c = T::multiply(a, b);
        printf("multiply %d x %d mod 1e9+7: %.3fs\n", n, n, double(clock() - t0) / CLOCKS_PER_SEC);
        ok = (int)c.size() == 2 * n - 1;
        for (int s = 0; s < 60 && ok; s++) {
            int k = s == 0 ? n - 1 : rng() % (2 * n - 1);
            __int128 v = 0;
            for (int i = max<int>(0, k - n + 1); i <= min<int>(k, n - 1); i++) v += (__int128)a[i] * b[k - i];
            ok &= (ll)(v % m) == c[k];
        }
        chk("multiply(random 2^18)", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
