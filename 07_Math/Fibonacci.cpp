// Fibonacci: F(n) mod m - linear loop, 2x2 matrix power, or fast doubling giving (F(n), F(n+1)).
// Use when: "n-th Fibonacci number modulo m" with n up to 1e18; any 2-term recurrence via matpow.
// Usage: fib(n, m);  fib_pair(n, m).first;  matpow(matrix{{{1, 1}, {1, 0}}}, n, m).mat[0][1];
// Handles: n = 0, m = 1, n up to 9e18, m up to 9e18.
// Time: fibonacci O(n) | fib, fib_pair O(log n)
// Indexing: F(0) = 0, F(1) = 1, F(2) = 1
// Note: every value is taken mod m (F(93) already overflows a long long). fib_pair recurses
//       log2(n) deep. m defaults to 1e9 + 7.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll fibonacci(ll n, ll m = 1e9 + 7) {
    ll a = 0, b = 1 % m;
    for (ll i = 0; i < n; i++) {
        ll tmp = ((__int128)a + b) % m;
        a = b;
        b = tmp;
    }
    return a;
}

struct matrix {
    ll mat[2][2];
};

matrix mul(const matrix &a, const matrix &b, ll m) {
    matrix c;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            __int128 s = 0;
            for (int k = 0; k < 2; k++) s += (__int128)a.mat[i][k] * b.mat[k][j];
            c.mat[i][j] = s % m;
        }
    return c;
}

matrix matpow(matrix base, ll n, ll m) { // entries of base must be in [0, m)
    matrix ans{{{1 % m, 0}, {0, 1 % m}}};
    while (n) {
        if (n & 1) ans = mul(ans, base, m);
        base = mul(base, base, m);
        n >>= 1;
    }
    return ans;
}

ll fib(ll n, ll m = 1e9 + 7) {
    matrix base{{{1 % m, 1 % m}, {1 % m, 0}}};
    return matpow(base, n, m).mat[0][1];
}

// returns {F(n), F(n+1)} mod m
pair<ll, ll> fib_pair(ll n, ll m = 1e9 + 7) {
    if (n == 0) return {0, 1 % m};
    auto p = fib_pair(n >> 1, m);
    ll c = (__int128)p.first * ((2 * (__int128)p.second - p.first + m) % m) % m;
    ll d = ((__int128)p.first * p.first + (__int128)p.second * p.second) % m;
    if (n & 1) return {d, (ll)(((__int128)c + d) % m)};
    return {c, d};
}
