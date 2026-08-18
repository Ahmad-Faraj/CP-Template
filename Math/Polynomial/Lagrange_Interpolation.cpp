// Lagrange Interpolation: recovers the unique degree-n polynomial through n+1 points and evaluates it anywhere.
// Use when: "sum of i^k for i = 1..n" with n huge, or a DP you can only compute for small n but know is polynomial.
// Handles: arbitrary points in O(n^2), equally spaced points in O(n), and x landing exactly on a sample point.
// Time: interpolate O(n^2) | interpolate_equal O(n + log MOD) | sum_of_powers O(k log k)
// Indexing: 0-based; interpolate_equal takes y[i] at x = a + d*i for i = 0..n
// Note: MOD must be prime and larger than the number of points, or the factorial inverses do not exist.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

const ll MOD = 1000000007;

ll norm(ll a) { return (a % MOD + MOD) % MOD; }

ll pw(ll b, ll e) {
    b = norm(b);
    ll r = 1;
    for (; e; e >>= 1, b = b * b % MOD)
        if (e & 1) r = r * b % MOD;
    return r;
}

ll inv(ll a) { return pw(a, MOD - 2); } // MOD must be prime

// f(x) through arbitrary points (x_i, y_i); the x_i must be distinct modulo MOD
ll interpolate(vector<pair<ll, ll>> pts, ll x) {
    int n = (int)pts.size();
    x = norm(x);
    for (auto &p : pts) p.first = norm(p.first), p.second = norm(p.second);
    for (auto &[xi, yi] : pts)
        if (xi == x) return yi; // sitting on a sample point, so no division is needed
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        ll num = 1, den = 1;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            num = num * norm(x - pts[j].first) % MOD;
            den = den * norm(pts[i].first - pts[j].first) % MOD;
        }
        ans = (ans + pts[i].second * num % MOD * inv(den)) % MOD;
    }
    return ans;
}

// f(x) where y[i] is f(a + d*i) for i = 0..n; d must not be 0 modulo MOD
ll interpolate_equal(const vector<ll> &y, ll x, ll a = 0, ll d = 1) {
    int n = (int)y.size() - 1;
    if (n < 0) return 0;
    x = norm(x), a = norm(a), d = norm(d);
    vector<ll> node(n + 1);
    for (int i = 0; i <= n; i++) {
        node[i] = (a + d * i) % MOD;
        if (node[i] == x) return norm(y[i]);
    }
    vector<ll> pref(n + 2, 1), suf(n + 2, 1); // products of (x - node[j]) on either side of i
    for (int i = 0; i <= n; i++) pref[i + 1] = pref[i] * norm(x - node[i]) % MOD;
    for (int i = n; i >= 0; i--) suf[i] = suf[i + 1] * norm(x - node[i]) % MOD;
    vector<ll> fact(n + 1, 1), finv(n + 1, 1); // one inversion total, not one per term
    for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % MOD;
    finv[n] = inv(fact[n]);
    for (int i = n; i >= 1; i--) finv[i - 1] = finv[i] * i % MOD;
    ll ans = 0;
    for (int i = 0; i <= n; i++) {
        ll term = norm(y[i]) * pref[i] % MOD * suf[i + 1] % MOD * finv[i] % MOD * finv[n - i] % MOD;
        ans = ((n - i) & 1) ? norm(ans - term) : (ans + term) % MOD;
    }
    return ans * inv(pw(d, n)) % MOD; // every denominator carries a factor d^n
}

// 1^k + 2^k + ... + n^k, for n up to 9e18 and small k
ll sum_of_powers(ll n, int k) {
    vector<ll> y(k + 2, 0); // the partial sums are a polynomial in n of degree k+1
    for (int i = 1; i <= k + 1; i++) y[i] = (y[i - 1] + pw(i, k)) % MOD;
    return interpolate_equal(y, n);
}

// Standard problem: n and k - report (1^k + 2^k + ... + n^k) mod 1e9+7
void solve() {
    ll n;
    int k;
    cin >> n >> k;
    cout << sum_of_powers(n, k) << '\n';
}
