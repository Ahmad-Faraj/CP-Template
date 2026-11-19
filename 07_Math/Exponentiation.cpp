// Exponentiation: binary power of a number (plain or mod m) and of a permutation, O(log n) steps.
// Use when: "a^n mod m" with huge n, inverse mod a prime, "where is each item after k shuffles".
// Usage: pow_mod(a, n, m);  mod_inv(a, p);  bin_pow(a, n);  permute_k_times(a, p, k);
// Handles: n = 0, negative a, a >= m, m = 1 (gives 0), any m up to 9e18.
// Time: O(log n) | permute_k_times O(size * log k)
// Indexing: permutations 0-based; apply_permutation(a, p)[i] = a[p[i]]
// Note: bin_pow has no modulus and silently overflows past 2^63.
//       mod_inv needs a PRIME p and a % p != 0.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll bin_pow(ll x, ll n) {
    ll res = 1;
    while (n) {
        if (n & 1) res = res * x;
        n >>= 1;
        if (n) x = x * x;
    }
    return res;
}

ll pow_mod(ll x, ll n, ll mod) {
    ll res = 1 % mod;
    x %= mod;
    if (x < 0) x += mod;
    while (n) {
        if (n & 1) res = (__int128)res * x % mod;
        x = (__int128)x * x % mod;
        n >>= 1;
    }
    return res;
}

ll mod_inv(ll x, ll mod) { return pow_mod(x, mod - 2, mod); }

vector<int> apply_permutation(vector<int> a, vector<int> p) {
    int n = (int)a.size();
    vector<int> res(n);
    for (int i = 0; i < n; i++) res[i] = a[p[i]];
    return res;
}

vector<int> permute_k_times(vector<int> a, vector<int> p, ll k) {
    while (k > 0) {
        if (k & 1) a = apply_permutation(a, p);
        p = apply_permutation(p, p);
        k >>= 1;
    }
    return a;
}
