// NTT Any Mod: exact polynomial product modulo ANY m (e.g. 1e9+7), no doubles.
// Use when: convolution "modulo 1e9+7" (any non-NTT modulus) where FFT would lose precision.
// Usage: vector<ll> c = NTT_Any_Mod::multiply(a, b);  // mod 1e9+7 | NTT_Any_Mod::multiply(a, b, m)
// Handles: empty input (returns {}), coefficients outside [0, m) incl. negatives (reduced first).
// Time: O(n log n), about 3x NTT.cpp
// Indexing: 0-based; c[k] = sum a[i] * b[j] over i + j == k (mod m), |c| = |a| + |b| - 1
// Note: exact while min(|a|, |b|) * (m - 1)^2 < M1 * M2 * M3 ~ 5.9e25 (m = 1e9+7: any length) and
//       |a| + |b| - 1 <= 2^24 (~1.6e7). For m = 998244353 use NTT.cpp: it is 3x faster.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct NTT_Any_Mod {
    static constexpr ll mod1 = 167772161, root1 = 3;
    static constexpr ll mod2 = 469762049, root2 = 3;
    static constexpr ll mod3 = 754974721, root3 = 11;

    static ll power(ll a, ll b, ll mod) {
        ll res = 1;
        a %= mod;
        while (b) {
            if (b & 1) res = res * a % mod;
            a = a * a % mod;
            b >>= 1;
        }
        return res;
    }

    static void ntt(vector<ll> &a, ll mod, ll root, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = power(root, (mod - 1) / len, mod);
            if (invert) wlen = power(wlen, mod - 2, mod);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j], v = a[i + j + len / 2] * w % mod;
                    a[i + j] = (u + v) % mod;
                    a[i + j + len / 2] = (u - v + mod) % mod;
                    w = w * wlen % mod;
                }
            }
        }
        if (invert) {
            ll inv_n = power(n, mod - 2, mod);
            for (ll &x : a) x = x * inv_n % mod;
        }
    }

    static vector<ll> multiply_mod(vector<ll> a, vector<ll> b, ll mod, ll root) {
        int need = a.size() + b.size() - 1, n = 1;
        while (n < need) n <<= 1;
        a.resize(n), b.resize(n);
        for (ll &x : a) x %= mod;
        for (ll &x : b) x %= mod;
        ntt(a, mod, root, false), ntt(b, mod, root, false);
        for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
        ntt(a, mod, root, true);
        a.resize(need);
        return a;
    }

    static ll crt(ll r1, ll r2, ll r3, ll m) {
        static const ll m1_inv_m2 = power(mod1, mod2 - 2, mod2);
        static const ll m12_inv_m3 = power(mod1 * mod2 % mod3, mod3 - 2, mod3);
        ll x2 = (r2 - r1 % mod2 + mod2) % mod2 * m1_inv_m2 % mod2;
        ll x3 = ((r3 - (r1 + mod1 * x2) % mod3) % mod3 + mod3) % mod3 * m12_inv_m3 % mod3;
        __int128 result = r1 + (__int128)mod1 * x2 + (__int128)mod1 * mod2 * x3;
        return (ll)(result % m);
    }

    static vector<ll> multiply(vector<ll> a, vector<ll> b, ll m = 1'000'000'007) {
        if (a.empty() || b.empty()) return {};
        for (ll &x : a) x = (x % m + m) % m;
        for (ll &x : b) x = (x % m + m) % m;
        vector<ll> x = multiply_mod(a, b, mod1, root1);
        vector<ll> y = multiply_mod(a, b, mod2, root2);
        vector<ll> z = multiply_mod(a, b, mod3, root3);
        vector<ll> res(x.size());
        for (int i = 0; i < (int)x.size(); i++) res[i] = crt(x[i], y[i], z[i], m);
        return res;
    }
};
