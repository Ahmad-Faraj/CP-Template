// Test for 07_Math/Combinatorics.cpp against Pascal's triangle, a product loop for nPr, the Catalan
// convolution recurrence and a stars-and-bars DP. Every public method is checked: constructor
// (incl. N = 0 and small prime moduli), nCr, nPr, catalan, stars_bars.
#include "../../07_Math/Combinatorics.cpp"

map<string, pair<long long, long long>> cov;
mt19937_64 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (ll p : {1000000007LL, 998244353LL, 1000003LL, 131LL, 13LL, 2LL}) {
        int N = (int)min<ll>(p - 1, 120); // the table is only valid below p
        CountLib C(N, p);
        // Pascal's triangle mod p
        vector<vector<ll>> P(N + 1, vector<ll>(N + 1, 0));
        for (int n = 0; n <= N; n++) {
            P[n][0] = 1 % p;
            for (int r = 1; r <= n; r++) P[n][r] = (P[n - 1][r - 1] + (r <= n - 1 ? P[n - 1][r] : 0)) % p;
        }
        for (int n = 0; n <= N; n++)
            for (int r = -2; r <= n + 2; r++) {
                ll want = (r >= 0 && r <= n) ? P[n][r] : 0;
                chk("nCr", C.nCr(n, r) == want);
                ll pr = (r >= 0 && r <= n) ? 1 : 0;
                for (int i = 0; i < r && r <= n; i++) pr = pr * ((n - i) % p) % p;
                chk("nPr", C.nPr(n, r) == pr);
            }
        // Catalan: C0 = 1, C(k+1) = sum C(i) C(k-i)
        vector<ll> cat(N / 2 + 1);
        cat[0] = 1 % p;
        for (int k = 1; k <= N / 2; k++) {
            cat[k] = 0;
            for (int i = 0; i < k; i++) cat[k] = (cat[k] + cat[i] * cat[k - 1 - i]) % p;
        }
        for (int k = 0; 2 * k <= N && k + 1 <= N; k++) chk("catalan", C.catalan(k) == cat[k]);
        // stars and bars: ways[n][k] = sum_t ways[n - t][k - 1]
        int S = min(N, 30);
        vector<vector<ll>> W(S + 1, vector<ll>(S + 1, 0));
        for (int k = 0; k <= S; k++)
            for (int n = 0; n <= S; n++) {
                if (k == 0) W[n][k] = (n == 0) % p;
                else
                    for (int t = 0; t <= n; t++) W[n][k] = (W[n][k] + W[n - t][k - 1]) % p;
            }
        for (int n = 0; n <= S; n++)
            for (int k = 0; k <= S; k++)
                if (n + k - 1 <= N) chk("stars_bars", C.stars_bars(n, k) == W[n][k]);
    }
    { // N = 0 and N = 1 constructors (the original wrote inv[1] out of bounds for N = 0)
        CountLib z(0);
        chk("ctor(N=0)", z.nCr(0, 0) == 1 && z.nPr(0, 0) == 1 && z.nCr(0, 1) == 0 && z.stars_bars(0, 0) == 1);
        CountLib o(1);
        chk("ctor(N=1)", o.nCr(1, 0) == 1 && o.nCr(1, 1) == 1 && o.catalan(0) == 1);
    }
    { // large n against Lucas-free direct product with modular inverse by brute Fermat
        CountLib C(2000000);
        const ll p = 1e9 + 7;
        auto pw = [&](ll a, ll e) { ll r = 1; a %= p; while (e) { if (e & 1) r = r * a % p; a = a * a % p; e >>= 1; } return r; };
        for (int it = 0; it < 300; it++) {
            int n = rng() % 2000001, r = rng() % (n + 1);
            int k = min(r, n - r);
            ll num = 1, den = 1;
            for (int i = 0; i < k; i++) num = num * (n - i) % p, den = den * (i + 1) % p;
            chk("nCr(large)", C.nCr(n, r) == num * pw(den, p - 2) % p);
        }
        auto t0 = clock();
        CountLib T(10000000);
        ll s = 0;
        for (int i = 0; i < 10000000; i++) {
            int n = rng() % 10000001;
            s += T.nCr(n, rng() % (n + 1));
        }
        printf("build N=1e7 + 1e7 nCr: %.3fs (%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, s % 10);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-32s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
