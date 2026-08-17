// Catalan: the n-th Catalan number, by the convolution recurrence or by the closed form.
// Use when: balanced bracket sequences, binary trees with n nodes, polygon triangulations, paths under the diagonal.
// Handles: n = 0 and 1, any modulus for the recurrence, and a prime modulus for the closed form.
// Time: table O(n^2) by recurrence, O(n) by closed form | one value O(n)
// Indexing: 0-based; C(0) = C(1) = 1, C(2) = 2, C(3) = 5, C(4) = 14
// Note: the recurrence works under ANY modulus. The closed form needs a prime one, because it inverts (n + 1).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// C(0..n) by the convolution recurrence: C(i) = sum over j of C(j) * C(i - 1 - j)
vector<ll> catalan_recurrence(int n, ll mod) {
    vector<ll> c(max(n + 1, 2));
    c[0] = 1 % mod;
    c[1] = 1 % mod;
    for (int i = 2; i <= n; i++) {
        c[i] = 0;
        for (int j = 0; j < i; j++) c[i] = (c[i] + c[j] * c[i - 1 - j]) % mod; // ll, not int
    }
    c.resize(n + 1);
    return c;
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

// C(0..n) in O(n) from C(n) = binom(2n, n) / (n + 1); the modulus must be prime
vector<ll> catalan_table(int n, ll mod) {
    vector<ll> fact(2 * n + 2), inv_fact(2 * n + 2);
    fact[0] = 1 % mod;
    for (int i = 1; i <= 2 * n + 1; i++) fact[i] = fact[i - 1] * i % mod;
    inv_fact[2 * n + 1] = pow_mod(fact[2 * n + 1], mod - 2, mod);
    for (int i = 2 * n + 1; i > 0; i--) inv_fact[i - 1] = inv_fact[i] * i % mod;
    vector<ll> c(n + 1);
    for (int i = 0; i <= n; i++) {
        ll binom = fact[2 * i] * inv_fact[i] % mod * inv_fact[i] % mod;
        c[i] = binom * pow_mod(i + 1, mod - 2, mod) % mod;
    }
    return c;
}

// one value, by the product form C(n) = prod over k of (n + k) / k for k = 2..n
ll catalan_single(ll n, ll mod) {
    if (n <= 1) return 1 % mod;
    ll num = 1, den = 1;
    for (ll k = 2; k <= n; k++) {
        num = (ll)((__int128)num * (n + k) % mod);
        den = (ll)((__int128)den * k % mod);
    }
    return (ll)((__int128)num * pow_mod(den, mod - 2, mod) % mod);
}

// Standard problem: n and a modulus - report the n-th Catalan number
void solve() {
    int n;
    ll mod;
    cin >> n >> mod;
    cout << catalan_recurrence(n, mod)[n] << '\n';
}
