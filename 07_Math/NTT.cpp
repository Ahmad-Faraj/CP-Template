// NTT: exact polynomial product modulo 998244353 (number-theoretic transform, integers only).
// Use when: convolution / polynomial product "modulo 998244353", counting ways combined by sums.
// Usage: vector<ll> c = NTT::multiply(a, b); | NTT::poly_pow_mod(a, e) | generator(p)
// Handles: empty input (returns {}), coefficients outside [0, MOD) incl. negatives (reduced first).
// Time: multiply O(n log n) | poly_pow_mod O(L log L log e), L = result length | generator O(sqrt)
// Indexing: 0-based; c[k] = sum a[i] * b[j] over i + j == k (mod MOD), |c| = |a| + |b| - 1
// Note: padded length <= 2^23 (|a| + |b| <= ~8.3e6); longer: NTT_Large; other modulus: NTT_Any_Mod.
//       poly_pow_mod keeps every term (length (|a| - 1) * e + 1). generator(p) = primitive root.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct NTT {
    static constexpr ll MOD = 998244353; // 119 * 2^23 + 1
    static constexpr ll ROOT = 3;        // primitive root of MOD

    static ll mod_pow(ll base, ll exp, ll mod) {
        ll result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = result * base % mod;
            base = base * base % mod;
            exp >>= 1;
        }
        return result;
    }

    static void ntt(vector<ll> &a, bool invert) { // a.size() is a power of two, values in [0, MOD)
        int n = a.size();
        for (int i = 1, j = 0; i < n; i++) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }
        for (int len = 2; len <= n; len <<= 1) {
            ll wlen = mod_pow(ROOT, (MOD - 1) / len, MOD);
            if (invert) wlen = mod_pow(wlen, MOD - 2, MOD);
            for (int i = 0; i < n; i += len) {
                ll w = 1;
                for (int j = 0; j < len / 2; j++) {
                    ll u = a[i + j], v = a[i + j + len / 2] * w % MOD;
                    a[i + j] = u + v < MOD ? u + v : u + v - MOD;
                    a[i + j + len / 2] = u - v >= 0 ? u - v : u - v + MOD;
                    w = w * wlen % MOD;
                }
            }
        }
        if (invert) {
            ll n_inv = mod_pow(n, MOD - 2, MOD);
            for (ll &x : a) x = x * n_inv % MOD;
        }
    }

    static vector<ll> multiply(const vector<ll> &a, const vector<ll> &b) {
        if (a.empty() || b.empty()) return {};
        int need = a.size() + b.size() - 1, n = 1;
        while (n < need) n <<= 1;
        vector<ll> fa(n), fb(n);
        for (int i = 0; i < (int)a.size(); i++) fa[i] = (a[i] % MOD + MOD) % MOD;
        for (int i = 0; i < (int)b.size(); i++) fb[i] = (b[i] % MOD + MOD) % MOD;
        ntt(fa, false), ntt(fb, false);
        for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] % MOD;
        ntt(fa, true);
        fa.resize(need);
        return fa;
    }

    static vector<ll> poly_pow_mod(const vector<ll> &a, ll exp) { // a^exp mod MOD, every term kept
        vector<ll> result{1}, base = a;
        while (exp > 0) {
            if (exp & 1) result = multiply(result, base);
            exp >>= 1;
            if (exp) base = multiply(base, base);
        }
        return result;
    }
};

ll generator(ll p) { // smallest primitive root of a prime p < ~3e9
    vector<ll> fact;
    ll phi = p - 1, n = phi;
    for (ll i = 2; i * i <= n; ++i)
        if (n % i == 0) {
            fact.push_back(i);
            while (n % i == 0) n /= i;
        }
    if (n > 1) fact.push_back(n);
    for (ll res = 1; res <= p; ++res) {
        bool ok = true;
        for (size_t i = 0; i < fact.size() && ok; ++i)
            ok &= NTT::mod_pow(res, phi / fact[i], p) != 1;
        if (ok) return res;
    }
    return -1;
}
