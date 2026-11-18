// Extended GCD: g = gcd(a, b) with a*x + b*y = g; then one solution of a*x + b*y = c.
// Use when: inverse mod a non-prime, "is a*x + b*y = c solvable", linear Diophantine equation.
// Usage: ll x, y; ll g = extended_gcd(a, b, x, y);   // a, b >= 0
//        ll x0, y0, g; if (find_any_solution(a, b, c, x0, y0, g)) ...  // any signs
// Handles: negative a / b in find_any_solution, a or b zero, a = b = 0 (solvable iff c = 0, g = 0).
// Time: O(log min(a, b))
// Indexing: not applicable
// Note: all solutions: x = x0 + k*(b/g), y = y0 - k*(a/g). x0 *= c/g can overflow once
//       |b*c| / g^2 > 9e18 (fine for |a|, |b|, |c| <= 1e9). Box counting: LDE_All_Solutions.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll d = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

bool find_any_solution(ll a, ll b, ll c, ll &x0, ll &y0, ll &g) {
    g = extended_gcd(abs(a), abs(b), x0, y0);
    if (g == 0) return x0 = y0 = 0, c == 0; // a = b = 0
    if (c % g) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    return true;
}
