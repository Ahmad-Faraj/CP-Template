// LDE All Solutions: counts integer (x, y) with a*x + b*y = c inside a box of x and y bounds.
// Use when: "how many pairs (x, y) in these ranges satisfy a*x + b*y = c", coin pairs with bounds.
// Usage: ll cnt = find_all_solutions(a, b, c, minx, maxx, miny, maxy);
//        find_any_solution(a, b, c, x0, y0, g);  shift_solution(x, y, a / g, b / g, k);
// Handles: any signs of a, b, c; a = 0 or b = 0 or both; unsolvable equations; empty boxes.
// Time: O(log min(|a|, |b|))
// Indexing: not applicable (bounds are inclusive)
// Note: all solutions are x0 + k*(b/g), y0 - k*(a/g). Safe in ll while |a|, |b|, |c| and bounds
//       <= 1e9 (b*c / g^2 must fit). a = b = c = 0 returns the box area.

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

void shift_solution(ll &x, ll &y, ll a, ll b, ll cnt) {
    x += cnt * b;
    y -= cnt * a;
}

ll find_all_solutions(ll a, ll b, ll c, ll minx, ll maxx, ll miny, ll maxy) {
    if (minx > maxx || miny > maxy) return 0;
    if (a == 0 && b == 0) return c == 0 ? (maxx - minx + 1) * (maxy - miny + 1) : 0;
    if (a == 0) return (c % b == 0 && miny <= c / b && c / b <= maxy) ? maxx - minx + 1 : 0;
    if (b == 0) return (c % a == 0 && minx <= c / a && c / a <= maxx) ? maxy - miny + 1 : 0;
    ll x, y, g;
    if (!find_any_solution(a, b, c, x, y, g)) return 0;
    a /= g;
    b /= g;

    ll sign_a = a > 0 ? +1 : -1;
    ll sign_b = b > 0 ? +1 : -1;

    shift_solution(x, y, a, b, (minx - x) / b);
    if (x < minx) shift_solution(x, y, a, b, sign_b);
    if (x > maxx) return 0;
    ll lx1 = x;

    shift_solution(x, y, a, b, (maxx - x) / b);
    if (x > maxx) shift_solution(x, y, a, b, -sign_b);
    ll rx1 = x;

    shift_solution(x, y, a, b, -(miny - y) / a);
    if (y < miny) shift_solution(x, y, a, b, -sign_a);
    if (y > maxy) return 0;
    ll lx2 = x;

    shift_solution(x, y, a, b, -(maxy - y) / a);
    if (y > maxy) shift_solution(x, y, a, b, sign_a);
    ll rx2 = x;

    if (lx2 > rx2) swap(lx2, rx2);
    ll lx = max(lx1, lx2);
    ll rx = min(rx1, rx2);

    if (lx > rx) return 0;
    return (rx - lx) / abs(b) + 1;
}
