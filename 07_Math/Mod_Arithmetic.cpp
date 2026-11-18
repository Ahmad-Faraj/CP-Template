// Mod Arithmetic: add / sub / mul / pow / inverse under a modulus, and a huge decimal string mod m.
// Use when: "print the answer modulo 1e9+7", division mod a prime, a number too long to store.
// Usage: add_mod(a, b, m);  sub_mod(a, b, m);  mul_mod(a, b, m);  // m defaults to MOD
//        mod_pow(a, e, m);  mod_inv(a, m);  Big_Mod(s, m);
// Handles: negative inputs (results in [0, m)), m up to 9e18 (__int128 products), e = 0, m = 1.
// Time: O(1) add/sub/mul | O(log e) pow and inverse | O(|s|) Big_Mod
// Indexing: not applicable
// Note: mod_inv is Fermat: m must be PRIME and a % m != 0 (else use Extended_GCD).
//       Big_Mod takes digits only (no sign, no spaces).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1e9 + 7;

ll add_mod(ll a, ll b, ll m = MOD) { return ((__int128)(a % m) + b % m + 2 * (__int128)m) % m; }
ll sub_mod(ll a, ll b, ll m = MOD) { return ((__int128)(a % m) - b % m + 2 * (__int128)m) % m; }
ll mul_mod(ll a, ll b, ll m = MOD) {
    __int128 r = (__int128)(a % m) * (b % m) % m;
    return r < 0 ? r + m : r;
}

// get a mod for big int
ll Big_Mod(string s, ll mod) {
    ll res = 0;
    for (auto &c : s) res = ((__int128)res * 10 + (c - '0')) % mod;
    return res;
}

ll mod_pow(ll a, ll b, ll m = MOD) {
    ll res = 1 % m;
    a %= m;
    if (a < 0) a += m;
    while (b) {
        if (b & 1) res = (__int128)res * a % m;
        a = (__int128)a * a % m;
        b >>= 1;
    }
    return res;
}

ll mod_inv(ll a, ll m = MOD) {
    return mod_pow(a, m - 2, m); // Fermat
}
