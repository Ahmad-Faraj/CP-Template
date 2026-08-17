// FWHT: convolution where indices combine by XOR, AND or OR instead of by addition.
// Use when: "count pairs whose XOR is k", subset-sum or superset-sum sums, "how many subsets AND to zero".
// Handles: XOR, AND and OR convolution, a modulus, taking a power of one sequence, and any padded power-of-two length.
// Time: O(n log n) where n is the padded power of two
// Indexing: 0-based; the length must be a power of two, so pad to one with pad_to_power_of_two
// Note: XOR's inverse divides by n, so the modulus must be odd for 2 to be invertible. Sized by vectors, not arrays.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

enum Combine { WHT_OR = 0, WHT_AND = 1, WHT_XOR = 2 };

size_t pad_to_power_of_two(vector<ll> &a) { // grows a to the next power of two, zero filled
    size_t n = 1;
    while (n < a.size()) n <<= 1;
    a.resize(n, 0);
    return n;
}

void wht(vector<ll> &a, Combine how, ll mod) { // the forward transform, in place
    size_t n = a.size();
    for (size_t len = 1; len < n; len <<= 1)
        for (size_t i = 0; i < n; i += len << 1)
            for (size_t j = 0; j < len; j++) {
                ll x = a[i + j], y = a[i + j + len];
                if (how == WHT_OR)
                    a[i + j] = x, a[i + j + len] = (x + y) % mod;
                else if (how == WHT_AND)
                    a[i + j] = (x + y) % mod, a[i + j + len] = y;
                else
                    a[i + j] = (x + y) % mod, a[i + j + len] = (x - y + mod) % mod;
            }
}

void inverse_wht(vector<ll> &a, Combine how, ll mod) {
    size_t n = a.size();
    ll inv2 = (mod + 1) / 2; // valid because the modulus is required to be odd
    for (size_t len = 1; len < n; len <<= 1)
        for (size_t i = 0; i < n; i += len << 1)
            for (size_t j = 0; j < len; j++) {
                ll x = a[i + j], y = a[i + j + len];
                if (how == WHT_OR)
                    a[i + j] = x, a[i + j + len] = (y - x + mod) % mod;
                else if (how == WHT_AND)
                    a[i + j] = (x - y + mod) % mod, a[i + j + len] = y;
                else {
                    a[i + j] = (ll)((__int128)(x + y) % mod * inv2 % mod);
                    a[i + j + len] = (ll)((__int128)(x - y + mod) % mod * inv2 % mod);
                }
            }
}

// result[k] = sum of a[i] * b[j] over all i, j with (i op j) == k
vector<ll> convolve(vector<ll> a, vector<ll> b, Combine how, ll mod) {
    size_t n = max(a.size(), b.size()), p = 1;
    while (p < n) p <<= 1;
    a.resize(p, 0), b.resize(p, 0);
    for (ll &x : a) x = ((x % mod) + mod) % mod;
    for (ll &x : b) x = ((x % mod) + mod) % mod;
    wht(a, how, mod), wht(b, how, mod);
    for (size_t i = 0; i < p; i++) a[i] = (ll)((__int128)a[i] * b[i] % mod);
    inverse_wht(a, how, mod);
    return a;
}

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

vector<ll> convolve_power(vector<ll> a, ll times, Combine how, ll mod) { // a convolved with itself `times` times
    pad_to_power_of_two(a);
    for (ll &x : a) x = ((x % mod) + mod) % mod;
    wht(a, how, mod);
    for (ll &x : a) x = pow_mod(x, times, mod);
    inverse_wht(a, how, mod);
    return a;
}

// Standard problem: two sequences and a modulus - print their XOR convolution
void solve() {
    int n;
    ll mod;
    cin >> n >> mod;
    vector<ll> a(n), b(n);
    for (ll &x : a) cin >> x;
    for (ll &x : b) cin >> x;
    vector<ll> c = convolve(a, b, WHT_XOR, mod);
    for (size_t i = 0; i < c.size(); i++) cout << c[i] << " \n"[i + 1 == c.size()];
}
