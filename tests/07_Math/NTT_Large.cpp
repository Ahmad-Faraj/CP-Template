// Test for 07_Math/NTT_Large.cpp against naive O(n*m) convolution mod 998244353.
// Every public function is checked: powrmod, mod_div, mod_poly (against naive reduction mod
// X^(n/2) -+ g^x), fast_ntt_poly_mul(a, b, x) (against a naive product mod X^n - g^x), and
// fast_ntt_poly_mul(a, b) (lengths 1..600, odd, powers of two, inputs restored), plus a product of
// length 1e7 > 2^23 checked at sampled indices.
#include "../../07_Math/NTT_Large.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(79);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
ll rnd(ll lo, ll hi) { return lo + (ll)(rng() % (unsigned long long)(hi - lo + 1)); }
const ll P = 998244353;
using T = NTT_Large;

ll slow_pow(ll b, ll e) {
    __int128 r = 1, x = b % P;
    for (; e; e >>= 1, x = x * x % P)
        if (e & 1) r = r * x % P;
    return (ll)r;
}
vector<ll> naive(const vector<int> &a, const vector<int> &b) {
    vector<ll> c(a.size() + b.size() - 1, 0);
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < b.size(); j++) c[i + j] = (c[i + j] + (ll)a[i] * b[j]) % P;
    return c;
}
vector<int> rvec(int n, int kind) {
    vector<int> v(n);
    for (auto &x : v) x = kind == 0 ? rnd(0, 3) : kind == 1 ? P - 1 - rnd(0, 3) : rnd(0, P - 1);
    return v;
}

int main() {
    for (int it = 0; it < 10000; it++) {
        ll b = rnd(0, P - 1), e = rnd(0, P);
        chk("powrmod", T::powrmod(b, e, P) == slow_pow(b, e));
        int x = rnd(0, 2 * P - 1);
        chk("mod_div", (ll)T::mod_div(x) * 2 % P == x % P && T::mod_div(x) >= 0 && T::mod_div(x) < P);
    }
    for (int it = 0; it < 2000; it++) {
        int n = 2 * rnd(1, 40), x = rnd(0, P - 2);
        auto a = rvec(n, rng() % 3);
        auto [lo, hi] = T::mod_poly(a, x);
        ll c = slow_pow(3, x);
        bool ok = (int)lo.size() == n / 2 && (int)hi.size() == n / 2;
        for (int i = 0; ok && i < n / 2; i++) // X^(n/2) = c and X^(n/2) = -c
            ok &= lo[i] == (a[i] + c * a[i + n / 2]) % P && hi[i] == ((a[i] - c * a[i + n / 2]) % P + P) % P;
        chk("mod_poly", ok);
    }
    // fast_ntt_poly_mul(a, b, x) = a * b mod (X^n - g^x), any n reachable from the wrapper's
    // recursion (n = y * 2^k) and any x whose halvings stay exact (x a multiple of (P-1) / 2^k)
    for (int it = 0; it < 1500; it++) {
        int k = rng() % 5, y = rnd(1, 70);
        int n = y << k;
        if (n > 1200) continue;
        int x = (ll)rnd(0, (1 << 10) - 1) * ((P - 1) >> 10) % (P - 1);
        auto a = rvec(n, rng() % 3), b = rvec(n, rng() % 3);
        auto full = naive(a, b);
        ll c = slow_pow(3, x);
        vector<ll> want(n, 0);
        for (int i = 0; i < (int)full.size(); i++)
            want[i % n] = (want[i % n] + (i >= n ? c : 1) * full[i]) % P;
        auto got = T::fast_ntt_poly_mul(a, b, x);
        chk("fast_ntt_poly_mul(a,b,x)", vector<ll>(got.begin(), got.end()) == want);
    }
    for (int it = 0; it < 3000; it++) {
        int n = rnd(1, 300), m = rnd(1, 300);
        if (it % 5 == 0) n = 1 << rnd(0, 9), m = 1 << rnd(0, 9);
        if (it % 7 == 0) n = rnd(1, 5);
        auto a = rvec(n, rng() % 3), b = rvec(m, rng() % 3);
        auto a0 = a, b0 = b;
        auto got = T::fast_ntt_poly_mul(a, b);
        chk("fast_ntt_poly_mul", vector<ll>(got.begin(), got.end()) == naive(a0, b0));
        chk("fast_ntt_poly_mul(restores a,b)", a == a0 && b == b0);
    }
    { // length past 2^23: 5e6 x 5e6 -> 1e7 - 1 terms, sampled indices against naive dot products
        int n = 5000000;
#ifdef __SANITIZE_ADDRESS__
        n = 300000;
#endif
        auto a = rvec(n, 2), b = rvec(n, 2);
        auto t0 = clock();
        auto c = T::fast_ntt_poly_mul(a, b);
        printf("fast_ntt_poly_mul %d x %d (length %d): %.3fs\n", n, n, 2 * n - 1,
               double(clock() - t0) / CLOCKS_PER_SEC);
        bool ok = (int)c.size() == 2 * n - 1;
        for (int s = 0; s < 30 && ok; s++) {
            int k = s == 0 ? 0 : s == 1 ? 2 * n - 2 : s == 2 ? n - 1 : rng() % (2 * n - 1);
            __int128 v = 0;
            for (int i = max<int>(0, k - n + 1); i <= min<int>(k, n - 1); i++) v += (ll)a[i] * b[k - i];
            ok &= (ll)(v % P) == c[k];
        }
        chk("fast_ntt_poly_mul(large)", ok);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
