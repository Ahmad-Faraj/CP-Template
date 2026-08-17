// LDE N Variables: one integer solution of a[0]*x[0] + ... + a[n-1]*x[n-1] = c, with the x[i] unrestricted.
// Use when: more than two coefficients and any integer solution will do - "pick counts of these items summing to c".
// Handles: zero coefficients, all-zero coefficients, n = 1, negative values, and unsolvable equations (empty result).
// Time: O(n log(max a))
// Indexing: 0-based; the returned vector matches a element for element
// Note: solutions exist exactly when gcd of all a[i] divides c. For x[i] >= 0 see LDE_N_Variables_Non_Negative.cpp.

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

bool any_solution_2(ll a, ll b, ll c, ll &x, ll &y, ll &g) { // the two-variable case, used as the step
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

// one solution, or an empty vector when none exists
vector<ll> any_solution(const vector<ll> &a, ll c) {
    int n = (int)a.size();
    if (n == 0) return c == 0 ? vector<ll>{} : vector<ll>{};
    bool all_zero = true;
    for (ll v : a) all_zero &= (v == 0);
    if (all_zero) return c == 0 ? vector<ll>(n, 0) : vector<ll>{};

    ll g = 0;
    for (ll v : a) g = __gcd(g, v < 0 ? -v : v);
    if (c % g != 0) return {};
    if (n == 1) return {c / a[0]};

    vector<ll> suffix_gcd(n);
    suffix_gcd[n - 1] = a[n - 1] < 0 ? -a[n - 1] : a[n - 1];
    for (int i = n - 2; i >= 0; i--) suffix_gcd[i] = __gcd(suffix_gcd[i + 1], a[i] < 0 ? -a[i] : a[i]);

    vector<ll> x;
    ll rest = c;
    for (int i = 0; i + 1 < n; i++) {
        ll xi, yi, gi;
        // solve a[i] * xi + suffix_gcd[i+1] * yi = rest, then push the rest down the suffix
        if (!any_solution_2(a[i], suffix_gcd[i + 1], rest, xi, yi, gi)) return {};
        if (suffix_gcd[i + 1] != 0) { // keep xi small, or later terms blow up
            ll span = suffix_gcd[i + 1] / gi;
            if (span < 0) span = -span;
            if (span != 0) xi = ((xi % span) + span) % span;
        }
        x.push_back(xi);
        rest -= a[i] * xi;
    }
    x.push_back(a[n - 1] == 0 ? 0 : rest / a[n - 1]);
    return x;
}

// Standard problem: n coefficients and a target - print one solution, or IMPOSSIBLE
void solve() {
    int n;
    ll c;
    cin >> n >> c;
    vector<ll> a(n);
    for (ll &v : a) cin >> v;
    vector<ll> x = any_solution(a, c);
    if (x.empty() && !(n == 0)) {
        cout << "IMPOSSIBLE\n";
        return;
    }
    for (int i = 0; i < n; i++) cout << x[i] << " \n"[i + 1 == n];
}
