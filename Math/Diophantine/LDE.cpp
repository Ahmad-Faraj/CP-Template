// LDE: integer solutions of a*x + b*y = c - one solution, all of them, or how many lie inside a box.
// Use when: "buy exactly c using coins a and b", "how many (x, y) pairs in this range", combining two step sizes.
// Handles: negative and zero coefficients, unsolvable equations, and counting or listing solutions within given bounds.
// Time: O(log min(a, b))
// Indexing: not applicable
// Note: solutions exist exactly when gcd(a, b) divides c. All long long - the usual version of this uses int.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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

// one solution of a*x + b*y = c, with g set to gcd(|a|, |b|); false when none exists
bool any_solution(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
    if (a == 0 && b == 0) {
        x = y = g = 0;
        return c == 0;
    }
    g = extended_gcd(a < 0 ? -a : a, b < 0 ? -b : b, x, y);
    if (c % g) return false;
    x *= c / g;
    y *= c / g;
    if (a < 0) x = -x;
    if (b < 0) y = -y;
    return true;
}

void shift_solution(ll &x, ll &y, ll a, ll b, ll steps) { // move along the solution line
    x += steps * b;
    y -= steps * a;
}

// how many solutions have minx <= x <= maxx and miny <= y <= maxy
ll count_solutions(ll a, ll b, ll c, ll minx, ll maxx, ll miny, ll maxy) {
    if (a == 0 && b == 0) // degenerate: every (x, y) in the box works, or none does
        return c == 0 ? (maxx - minx + 1) * (maxy - miny + 1) : 0;
    if (a == 0) { // b*y = c, so y is fixed and x is free
        if (c % b || c / b < miny || c / b > maxy) return 0;
        return maxx - minx + 1;
    }
    if (b == 0) {
        if (c % a || c / a < minx || c / a > maxx) return 0;
        return maxy - miny + 1;
    }
    ll x, y, g;
    if (!any_solution(a, b, c, x, y, g)) return 0;
    a /= g, b /= g;
    ll sign_a = a > 0 ? 1 : -1, sign_b = b > 0 ? 1 : -1;

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
    ll lx = max(lx1, lx2), rx = min(rx1, rx2);
    if (lx > rx) return 0;
    return (rx - lx) / (b < 0 ? -b : b) + 1;
}

// the solutions themselves, inside the same box; cap limits how many are returned
vector<pair<ll, ll>> list_solutions(ll a, ll b, ll c, ll minx, ll maxx, ll miny, ll maxy, size_t cap = 1000000) {
    vector<pair<ll, ll>> out;
    if (a == 0 && b == 0) {
        if (c != 0) return out;
        for (ll xx = minx; xx <= maxx && out.size() < cap; xx++)
            for (ll yy = miny; yy <= maxy && out.size() < cap; yy++) out.push_back({xx, yy});
        return out;
    }
    if (b == 0) { // a*x = c fixes x; y ranges freely
        if (c % a) return out;
        ll xx = c / a;
        if (xx < minx || xx > maxx) return out;
        for (ll yy = miny; yy <= maxy && out.size() < cap; yy++) out.push_back({xx, yy});
        return out;
    }
    ll x0, y0, g;
    if (!any_solution(a, b, c, x0, y0, g)) return out;
    // x moves in steps of |b / g|; find the smallest such x that is >= minx, then walk up
    ll step = (b / g) < 0 ? -(b / g) : (b / g);
    ll k = (minx - x0) / step; // may be off by one in either direction, so nudge
    ll x = x0 + k * step;
    while (x < minx) x += step;
    while (x - step >= minx) x -= step;
    for (; x <= maxx && out.size() < cap; x += step) {
        ll rest = c - a * x;
        if (rest % b) continue; // cannot happen for a real solution, but keeps the division honest
        ll y = rest / b;
        if (y >= miny && y <= maxy) out.push_back({x, y});
    }
    return out;
}

// Standard problem: a, b, c and a box - report how many integer solutions lie inside it
void solve() {
    ll a, b, c, minx, maxx, miny, maxy;
    cin >> a >> b >> c >> minx >> maxx >> miny >> maxy;
    cout << count_solutions(a, b, c, minx, maxx, miny, maxy) << '\n';
}
