// Extended GCD: gcd(a, b) together with the x and y satisfying a*x + b*y = gcd(a, b).
// Use when: a modular inverse under a composite modulus, "is a*x + b*y = c solvable", exact common-factor cancelling.
// Handles: negative and zero inputs, a or b equal to zero, and inverses under a composite modulus.
// Time: O(log min(a, b))
// Indexing: not applicable
// Note: for the full solution SET of a*x + b*y = c see Diophantine/. Recursive, but only log deep.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// returns g = gcd(a, b) and sets x, y with a*x + b*y = g
ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = (a < 0 ? -1 : 1), y = 0;
        return a < 0 ? -a : a;
    }
    ll x1, y1;
    ll g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

ll gcd_of(ll a, ll b) { return b == 0 ? (a < 0 ? -a : a) : gcd_of(b, a % b); }

ll lcm_of(ll a, ll b) { // 0 when either is 0; divides first so it does not overflow needlessly
    if (a == 0 || b == 0) return 0;
    ll g = gcd_of(a, b);
    return (a / g) * b;
}

ll inv_mod(ll a, ll m) { // the inverse of a mod m, or -1 when gcd(a, m) != 1
    ll x, y;
    a %= m;
    if (a < 0) a += m;
    if (extended_gcd(a, m, x, y) != 1) return -1;
    x %= m;
    return x < 0 ? x + m : x;
}

// true when a*x + b*y = c has a solution, and then x, y is one of them
bool linear_solution(ll a, ll b, ll c, ll &x, ll &y) {
    ll g = extended_gcd(a, b, x, y);
    if (g == 0) { // a and b both zero
        x = y = 0;
        return c == 0;
    }
    if (c % g != 0) return false;
    ll k = c / g;
    x = (ll)((__int128)x * k);
    y = (ll)((__int128)y * k);
    return true;
}

// Standard problem: a, b and c - report whether a*x + b*y = c is solvable, and one solution if so
void solve() {
    ll a, b, c, x, y;
    cin >> a >> b >> c;
    if (!linear_solution(a, b, c, x, y))
        cout << "NO SOLUTION\n";
    else
        cout << x << ' ' << y << '\n';
}
