// NTT Any Mod: exact polynomial multiplication under any modulus, by running three NTTs and recombining with CRT.
// Use when: the modulus is not NTT-friendly - 1e9+7 is the usual case - and FFT's doubles would lose precision.
// Handles: any modulus up to about 1e18, non-prime moduli, any lengths, negative coefficients.
// Time: O(n log n) with a constant factor of three transforms
// Indexing: 0-based; result[k] is the sum of a[i] * b[j] over i + j == k, of length a.size() + b.size() - 1
// Note: three times the work of NTT.cpp - use that one whenever the modulus really is 998244353.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll pow_mod(ll b, ll e, ll m) {
    ll r = 1 % m;
    b %= m;
    if (b < 0) b += m;
    while (e > 0) {
        if (e & 1) r = (ll)((__int128)r * b % m);
        b = (ll)((__int128)b * b % m);
        e >>= 1;
    }
    return r;
}

void ntt(vector<ll> &a, bool invert, ll mod, ll root) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = pow_mod(root, (mod - 1) / len, mod);
        if (invert) w = pow_mod(w, mod - 2, mod);
        for (int i = 0; i < n; i += len) {
            ll cur = 1;
            for (int j = 0; j < len / 2; j++) {
                ll u = a[i + j], v = a[i + j + len / 2] * cur % mod;
                a[i + j] = (u + v) % mod;
                a[i + j + len / 2] = (u - v + mod) % mod;
                cur = cur * w % mod;
            }
        }
    }
    if (invert) {
        ll inv_n = pow_mod(n, mod - 2, mod);
        for (ll &x : a) x = x * inv_n % mod;
    }
}

vector<ll> multiply_under(vector<ll> a, vector<ll> b, ll mod, ll root) { // one NTT-friendly prime
    if (a.empty() || b.empty()) return {};
    int need = (int)(a.size() + b.size() - 1), n = 1;
    while (n < need) n <<= 1;
    for (ll &x : a) x = ((x % mod) + mod) % mod;
    for (ll &x : b) x = ((x % mod) + mod) % mod;
    a.resize(n), b.resize(n);
    ntt(a, false, mod, root), ntt(b, false, mod, root);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % mod;
    ntt(a, true, mod, root);
    a.resize(need);
    return a;
}

// three NTT-friendly primes whose product exceeds 1e27, so any true coefficient is recovered exactly
const ll M1 = 167772161, R1 = 3;  // 5 * 2^25 + 1
const ll M2 = 469762049, R2 = 3;  // 7 * 2^26 + 1
const ll M3 = 1224736769, R3 = 3; // 73 * 2^24 + 1

vector<ll> multiply(const vector<ll> &a, const vector<ll> &b, ll mod) {
    vector<ll> x = multiply_under(a, b, M1, R1);
    vector<ll> y = multiply_under(a, b, M2, R2);
    vector<ll> z = multiply_under(a, b, M3, R3);
    vector<ll> out(x.size());
    ll inv_m1_m2 = pow_mod(M1 % M2, M2 - 2, M2);
    __int128 m1m2 = (__int128)M1 * M2;
    ll inv_m1m2_m3 = pow_mod((ll)(m1m2 % M3), M3 - 2, M3);
    __int128 P = m1m2 * M3; // about 9.6e25, so any true coefficient below P/2 in size is exact
    for (size_t i = 0; i < x.size(); i++) {
        // lift x[i] to a value congruent mod M1 and M2, then again mod M3
        ll t1 = (ll)((__int128)(y[i] - x[i] % M2 + M2) % M2 * inv_m1_m2 % M2);
        __int128 v12 = x[i] + (__int128)M1 * t1;
        ll t2 = (ll)((__int128)((z[i] - (ll)(v12 % M3)) % M3 + M3) % M3 * inv_m1m2_m3 % M3);
        __int128 v = v12 + m1m2 * t2;
        // CRT returns a value in [0, P); a genuinely NEGATIVE coefficient comes back as v + P,
        // so centre it before reducing, or negative inputs give the wrong residue
        if (v > P / 2) v -= P;
        ll r = (ll)(v % mod);
        out[i] = r < 0 ? r + mod : r;
    }
    return out;
}

// Standard problem: two polynomials and a modulus - print the coefficients of their product mod m
void solve() {
    int n, m;
    ll mod;
    cin >> n >> m >> mod;
    vector<ll> a(n), b(m);
    for (ll &x : a) cin >> x;
    for (ll &x : b) cin >> x;
    vector<ll> c = multiply(a, b, mod);
    for (size_t i = 0; i < c.size(); i++) cout << c[i] << " \n"[i + 1 == c.size()];
}
