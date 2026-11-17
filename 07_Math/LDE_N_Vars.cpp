// LDE N Vars: one integer solution of a[0]*x[0] + ... + a[n-1]*x[n-1] = c (x[i] any sign), or none.
// Use when: "reach exactly c with steps a[i] forwards or backwards", Diophantine with > 2 terms.
// Usage: vector<ll> x = find_any_solution(a, c);  // empty = no solution
//        find_any_solution(a, b, c, x0, y0, g)     // a*x0 + b*y0 = c, g = gcd(|a|, |b|)
// Handles: zero and negative coefficients, all-zero a (solvable iff c == 0), n = 1.
// Time: O(n log(max |a|))
// Indexing: 0-based; x[i] pairs with a[i]
// Note: a solution exists iff gcd(a) divides c. x[0..n-2] are kept small (in [0, suffix gcd / g));
//       the last one absorbs the rest. Safe in ll for |a[i]|, |c| <= 1e9. n = 0 returns {} always.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll extended_euclid(ll a, ll b, ll &x, ll &y) {
    ll xx = y = 0;
    ll yy = x = 1;
    while (b) {
        ll q = a / b;
        ll t = b;
        b = a % b;
        a = t;
        t = xx;
        xx = x - q * xx;
        x = t;
        t = yy;
        yy = y - q * yy;
        y = t;
    }
    return a;
}

// a * x0 + b * y0 = c; false if impossible
bool find_any_solution(ll a, ll b, ll c, ll &x0, ll &y0, ll &g) {
    if (a == 0 && b == 0) {
        if (c) return false;
        x0 = y0 = g = 0;
        return true;
    }
    g = extended_euclid(abs(a), abs(b), x0, y0);
    if (c % g != 0) return false;
    x0 *= c / g;
    y0 *= c / g;
    if (a < 0) x0 *= -1;
    if (b < 0) y0 *= -1;
    return true;
}

vector<ll> find_any_solution(vector<ll> a, ll c) {
    int n = a.size();
    vector<ll> x;
    bool all_zero = true;
    for (int i = 0; i < n; i++) all_zero &= a[i] == 0;
    if (all_zero) {
        if (c) return {};
        x.assign(n, 0);
        return x;
    }
    ll g = 0;
    for (int i = 0; i < n; i++) g = __gcd(g, a[i]);
    if (c % g != 0) return {};
    if (n == 1) return {c / a[0]};
    vector<ll> suf_gcd(n);
    suf_gcd[n - 1] = a[n - 1];
    for (int i = n - 2; i >= 0; i--) suf_gcd[i] = __gcd(suf_gcd[i + 1], a[i]);
    ll cur = c;
    for (int i = 0; i + 1 < n; i++) {
        ll x0, y0, g;
        // solve a[i] * x + suf_gcd[i + 1] * y = cur, the rest of the suffix makes up y * suf_gcd
        bool ok = find_any_solution(a[i], suf_gcd[i + 1], cur, x0, y0, g);
        assert(ok);
        ll shift = g ? abs(suf_gcd[i + 1] / g) : 0; // 0 when the whole suffix is zero
        if (shift) x0 = (x0 % shift + shift) % shift;
        x.push_back(x0);
        cur -= a[i] * x0;
    }
    x.push_back(a[n - 1] == 0 ? 0 : cur / a[n - 1]);
    return x;
}
