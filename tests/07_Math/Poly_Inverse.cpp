// Test for 07_Math/Poly_Inverse.cpp against naive O(n^2) series inversion mod 998244353.
// Every public function is checked: poly_inverse(a, n) for random a (any size vs n, negatives,
// values >= MOD), a[0] == 0 (no inverse -> {}), n = 0, empty a, and the Library Checker sample.
#include "../../07_Math/Poly_Inverse.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(97);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
const ll P = 998244353;

ll md(ll x) { return (x % P + P) % P; }
ll pw(ll b, ll e) { // independent modpow in __int128
    __int128 r = 1, x = md(b);
    for (; e; e >>= 1, x = x * x % P)
        if (e & 1) r = r * x % P;
    return (ll)r;
}
vector<ll> naive_inv(const vector<ll> &a, int n) { // b_k = -inv(a0) * sum_{i>=1} a_i b_{k-i}
    vector<ll> b(n);
    ll i0 = pw(a[0], P - 2);
    for (int k = 0; k < n; k++) {
        __int128 s = k == 0 ? 1 : 0;
        for (int i = 1; i <= k && i < (int)a.size(); i++) s -= (__int128)md(a[i]) * b[k - i];
        b[k] = (ll)(((s % P + P) % P) * i0 % P);
    }
    return b;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int len = rng() % 70 + 1, n = rng() % 90;
        if (it % 9 == 0) n = 1;
        if (it % 11 == 0) len = 1;
        vector<ll> a(len);
        int kind = rng() % 3;
        for (auto &x : a) x = kind == 0 ? rnd(0, 5) : kind == 1 ? rnd(0, P - 1) : rnd(-3*P, 3*P);
        bool zero = it % 13 == 0;
        if (zero) a[0] = P * rnd(-2, 2);
        else if (md(a[0]) == 0) a[0] = 1;
        auto got = poly_inverse(a, n);
        if (zero || n == 0) {
            chk(zero ? "poly_inverse(a0=0)" : "poly_inverse(n=0)", got.empty());
            continue;
        }
        chk(len < n ? "poly_inverse(|a|<n)" : "poly_inverse(|a|>=n)", got == naive_inv(a, n));
    }
    chk("poly_inverse(empty a)", poly_inverse({}, 5).empty());
    chk("samples", poly_inverse({5, 4, 3, 2, 1}, 5) ==
                       vector<ll>{598946612, 718735934, 862483121, 635682004, 163871793});
    { // large: Library Checker size; check a * b == 1 mod x^n at sampled coefficients
        int n = 500000;
#ifdef __SANITIZE_ADDRESS__
        n = 1 << 16;
#endif
        vector<ll> a(n);
        for (auto &x : a) x = rnd(0, P - 1);
        a[0] = rnd(1, P - 1);
        auto t0 = clock();
        auto b = poly_inverse(a, n);
        printf("poly_inverse n=%d: %.3fs\n", n, double(clock() - t0) / CLOCKS_PER_SEC);
        bool ok = (int)b.size() == n;
        for (int s = 0; s < 30 && ok; s++) {
            int k = s == 0 ? 0 : s == 1 ? n - 1 : rng() % n;
            __int128 v = 0;
            for (int i = 0; i <= k; i++) v = (v + (__int128)a[i] * b[k - i]) % P;
            ok &= (ll)v == (k == 0);
        }
        chk("poly_inverse(large)", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
