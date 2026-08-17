// Fibonacci: F(n) mod m for n up to 9e18, by fast doubling, plus the matrix power the same trick generalises to.
// Use when: "the n-th Fibonacci number mod m" at huge n, or any two-term linear recurrence.
// Handles: n = 0, huge n, any modulus, and returning the pair (F(n), F(n+1)) when the next term is also needed.
// Time: O(log n)
// Indexing: F(0) = 0, F(1) = 1
// Note: everything is taken mod m, because F(93) already exceeds a long long. Pass m = 0 for no modulus only if n < 93.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll fib_linear(ll n, ll m) { // O(n), for cross-checking or tiny n
    ll a = 0, b = 1 % m;
    for (ll i = 0; i < n; i++) {
        ll t = (a + b) % m;
        a = b;
        b = t;
    }
    return a;
}

// returns (F(n) mod m, F(n+1) mod m) by fast doubling
pair<ll, ll> fib_pair(ll n, ll m) {
    if (n == 0) return {0, 1 % m};
    auto [a, b] = fib_pair(n >> 1, m); // a = F(k), b = F(k+1) where k = n / 2
    ll c = (ll)((__int128)a * (((__int128)2 * b - a + 2 * m) % m) % m); // F(2k)
    ll d = (ll)(((__int128)a * a + (__int128)b * b) % m);               // F(2k+1)
    if (n & 1) return {d, (c + d) % m};
    return {c, d};
}

ll fib(ll n, ll m) { return fib_pair(n, m).first; }

struct Matrix2 { // the 2x2 matrix power that fast doubling is a specialisation of
    ll a, b, c, d, m;
    Matrix2(ll a, ll b, ll c, ll d, ll m) : a(a), b(b), c(c), d(d), m(m) {}
    Matrix2 operator*(const Matrix2 &o) const {
        auto mix = [&](ll p, ll q, ll r, ll s) { return (ll)(((__int128)p * r + (__int128)q * s) % m); };
        return Matrix2(mix(a, b, o.a, o.c), mix(a, b, o.b, o.d), mix(c, d, o.a, o.c), mix(c, d, o.b, o.d), m);
    }
};

Matrix2 matrix_power(Matrix2 base, ll e) {
    Matrix2 res(1 % base.m, 0, 0, 1 % base.m, base.m);
    while (e > 0) {
        if (e & 1) res = res * base;
        base = base * base;
        e >>= 1;
    }
    return res;
}

ll fib_matrix(ll n, ll m) { return matrix_power(Matrix2(1, 1, 1, 0, m), n).b; } // the same answer, slower

// any two-term recurrence: g(n) = p*g(n-1) + q*g(n-2), with g(0) and g(1) given
ll linear_recurrence(ll p, ll q, ll g0, ll g1, ll n, ll m) {
    if (n == 0) return g0 % m;
    Matrix2 t = matrix_power(Matrix2(p % m, q % m, 1, 0, m), n - 1);
    return (ll)(((__int128)t.a * (g1 % m) + (__int128)t.b * (g0 % m)) % m);
}

// Standard problem: n and m - report F(n) mod m
void solve() {
    ll n, m;
    cin >> n >> m;
    cout << fib(n, m) << '\n';
}
