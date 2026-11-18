// Test for 07_Math/Exponentiation.cpp against repeated multiplication (__int128, small exponents),
// a separate recursive power for huge exponents, Fermat's little theorem, and naive k-fold
// permutation application. Every public function is checked: bin_pow, pow_mod, mod_inv,
// apply_permutation, permute_k_times.
#include "../../07_Math/Exponentiation.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(5);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

ll md(__int128 v, ll m) { v %= m; return (ll)(v < 0 ? v + m : v); }
ll naive_pow(ll a, ll n, ll m) {
    ll r = md(1, m);
    for (ll i = 0; i < n; i++) r = md((__int128)r * md(a, m), m);
    return r;
}
ll rec_pow(ll a, ll n, ll m) { // different shape: recursive, top-down
    if (n == 0) return md(1, m);
    ll h = rec_pow(a, n / 2, m);
    ll r = md((__int128)h * h, m);
    return n % 2 ? md((__int128)r * md(a, m), m) : r;
}
ll rnd(ll lo, ll hi) { // uniform in [lo, hi], unsigned so the span cannot overflow
    unsigned long long span = (unsigned long long)hi - (unsigned long long)lo + 1;
    return (ll)((unsigned long long)lo + (span ? rng() % span : rng()));
}

int main() {
    vector<ll> mods = {1, 2, 3, 7, 1000000007, 998244353, 1000000000000000003LL, (1LL << 61) - 1,
                       LLONG_MAX, 4611686018427387847LL, 1000000000000LL};
    for (int it = 0; it < 20000; it++) {
        ll m = it % 3 ? mods[rng() % mods.size()] : rnd(1, LLONG_MAX);
        ll a = it % 4 == 0 ? rnd(LLONG_MIN + 1, LLONG_MAX) : rnd(-50, 50);
        ll n = rng() % 60;
        chk("pow_mod(small n)", pow_mod(a, n, m) == naive_pow(a, n, m));
        ll N = rnd(0, LLONG_MAX);
        chk("pow_mod(huge n)", pow_mod(a, N, m) == rec_pow(a, N, m));
    }
    chk("pow_mod(m=1)", pow_mod(5, 0, 1) == 0 && pow_mod(0, 0, 7) == 1 && pow_mod(0, 5, 7) == 0);
    for (ll p : {2LL, 3LL, 13LL, 1000000007LL, 998244353LL, (1LL << 61) - 1, 1000000000000000003LL})
        for (int it = 0; it < 500; it++) {
            ll a = rnd(-(ll)4e18, (ll)4e18);
            if (md(a, p) == 0) continue;
            ll iv = mod_inv(a, p);
            chk("mod_inv", iv >= 0 && iv < p && md((__int128)md(a, p) * iv, p) == 1);
            chk("pow_mod(fermat)", pow_mod(a, p - 1, p) == 1);
        }
    // bin_pow: only where the true value fits in a long long
    for (ll a = -40; a <= 40; a++)
        for (ll n = 0; n < 64; n++) {
            __int128 v = 1;
            bool fits = true;
            for (ll i = 0; i < n && fits; i++) {
                v *= a;
                if (v > LLONG_MAX || v < LLONG_MIN) fits = false;
            }
            if (fits) chk("bin_pow", bin_pow(a, n) == (ll)v);
        }
    chk("bin_pow(edges)", bin_pow(2, 62) == (1LL << 62) && bin_pow(-2, 63) == LLONG_MIN && bin_pow(0, 0) == 1);
    // permutations
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 9;
        vector<int> a(n), p(n);
        for (auto &x : a) x = rng() % 100 - 50;
        iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        vector<int> r(n);
        for (int i = 0; i < n; i++) r[i] = a[p[i]];
        chk("apply_permutation", apply_permutation(a, p) == r);
        ll k = rng() % 40;
        vector<int> cur = a;
        for (ll t = 0; t < k; t++) {
            vector<int> nx(n);
            for (int i = 0; i < n; i++) nx[i] = cur[p[i]];
            cur = nx;
        }
        chk("permute_k_times", permute_k_times(a, p, k) == cur);
        // huge k: the permutation's order divides lcm of cycle lengths <= 840 for n <= 8
        ll K = rnd(0, LLONG_MAX);
        chk("permute_k_times(huge k)", permute_k_times(a, p, K) == permute_k_times(a, p, K % 840));
    }
    {
        auto t0 = clock();
        ll s = 0;
        for (int i = 0; i < 1000000; i++) s ^= pow_mod(rng(), rnd(0, LLONG_MAX), 1000000007);
        for (int i = 0; i < 1000000; i++) s ^= pow_mod(rng(), rnd(0, LLONG_MAX), (1LL << 61) - 1);
        int n = 200000;
        vector<int> a(n), p(n);
        iota(a.begin(), a.end(), 0), iota(p.begin(), p.end(), 0);
        shuffle(p.begin(), p.end(), rng);
        auto r = permute_k_times(a, p, (ll)1e18);
        printf("2e6 pow_mod (n~1e18) + permute n=2e5 k=1e18: %.3fs (%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, (s ^ r[0]) % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
