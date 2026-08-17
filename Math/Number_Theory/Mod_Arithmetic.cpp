// Mod Arithmetic: add, subtract, multiply, power and invert under a modulus, without overflowing on the way.
// Use when: any answer wanted "modulo 1e9+7", modular inverses, division under a modulus, a number too big to read.
// Handles: any modulus up to 9e18 via __int128, negative inputs, non-prime moduli (inverse falls back to extended gcd).
// Time: O(1) for add/sub/mul | O(log e) for power and inverse
// Indexing: not applicable
// Note: inverse exists only when gcd(a, m) == 1 - it returns -1 otherwise rather than a wrong number.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1000000007;

ll norm(ll a, ll m = MOD) { // into [0, m); adds m only when negative, so a large m cannot overflow
    a %= m;
    if (a < 0) a += m;
    return a;
}

ll add_mod(ll a, ll b, ll m = MOD) { // via __int128, since norm(a) + norm(b) can exceed 9e18
    return (ll)(((__int128)norm(a, m) + norm(b, m)) % m);
}

ll sub_mod(ll a, ll b, ll m = MOD) {
    ll d = norm(a, m) - norm(b, m);
    return d < 0 ? d + m : d;
}

ll mul_mod(ll a, ll b, ll m = MOD) { // __int128 so a modulus near 9e18 still works
    return (ll)((__int128)norm(a, m) * norm(b, m) % m);
}

ll pow_mod(ll base, ll exp, ll m = MOD) { // exp must be >= 0
    ll res = 1 % m;
    base = norm(base, m);
    while (exp > 0) {
        if (exp & 1) res = mul_mod(res, base, m);
        base = mul_mod(base, base, m);
        exp >>= 1;
    }
    return res;
}

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

ll inv_mod(ll a, ll m = MOD) { // -1 when a has no inverse, which happens iff gcd(a, m) != 1
    ll x, y;
    a = norm(a, m);
    if (extended_gcd(a, m, x, y) != 1) return -1;
    return norm(x, m);
}

ll inv_mod_prime(ll a, ll m = MOD) { return pow_mod(a, m - 2, m); } // only valid for prime m

ll div_mod(ll a, ll b, ll m = MOD) { // a / b, or -1 when b is not invertible
    ll ib = inv_mod(b, m);
    return ib < 0 ? -1 : mul_mod(a, ib, m);
}

ll mod_of_string(ll m, const string &s) { // for a number with more digits than any integer type holds
    ll res = 0;
    for (char c : s)
        if (isdigit((unsigned char)c)) res = (res * 10 + (c - '0')) % m;
    return res;
}

// Standard problem: read a huge number as text and a modulus, then report the number and its inverse mod m
void solve() {
    string s;
    ll m;
    cin >> s >> m;
    ll v = mod_of_string(m, s);
    ll iv = inv_mod(v, m);
    cout << v << ' ' << iv << '\n';
}
