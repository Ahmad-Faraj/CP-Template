// Binomial Coefficients: factorial and inverse-factorial tables, then nCr, nPr and the standard counting formulas.
// Use when: "how many ways to choose", "how many arrangements", stars and bars, derangements, Catalan by formula.
// Handles: r out of range (returns 0), r > n, n = 0, any prime modulus, and huge n via lucas_nCr.
// Time: build O(n) | every query O(1)
// Indexing: build with the largest n you will ask about; queries are 0-based counts
// Note: the modulus must be PRIME and bigger than n - at n = mod the factorials vanish. Small modulus? use lucas_nCr.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Binomial {
    ll mod;
    vector<ll> fact, inv_fact, inv;

    Binomial(int n, ll mod = 1000000007) : mod(mod) {
        n = max(n, 2); // so inv[1] and inv[2] always exist
        fact.resize(n + 1);
        inv_fact.resize(n + 1);
        inv.resize(n + 1);
        fact[0] = 1 % mod;
        for (int i = 1; i <= n; i++) fact[i] = fact[i - 1] * i % mod;
        inv[1] = 1 % mod;
        for (int i = 2; i <= n; i++) inv[i] = (mod - (mod / i) * inv[mod % i] % mod) % mod;
        inv_fact[0] = 1 % mod;
        for (int i = 1; i <= n; i++) inv_fact[i] = inv_fact[i - 1] * inv[i] % mod;
    }

    ll nCr(ll n, ll r) { // choose r of n, order irrelevant
        if (r < 0 || r > n || n < 0) return 0;
        return fact[n] * inv_fact[r] % mod * inv_fact[n - r] % mod;
    }

    ll nPr(ll n, ll r) { // arrange r of n, order matters
        if (r < 0 || r > n || n < 0) return 0;
        return fact[n] * inv_fact[n - r] % mod;
    }

    ll catalan(ll n) { return n < 0 ? 0 : nCr(2 * n, n) * inv[n + 1] % mod; }

    // n indistinguishable balls into k distinguishable boxes, empty boxes allowed
    ll stars_and_bars(ll n, ll k) { return k <= 0 ? (n == 0) : nCr(n + k - 1, k - 1); }

    // the same, but every box must hold at least one
    ll stars_and_bars_positive(ll n, ll k) { return (k <= 0 || n < k) ? 0 : nCr(n - 1, k - 1); }

    ll multinomial(const vector<int> &parts) { // n! / (p1! p2! ...), where n is the sum of parts
        ll total = 0;
        for (int p : parts) total += p;
        ll res = fact[total];
        for (int p : parts) res = res * inv_fact[p] % mod;
        return res;
    }

    ll derangements(int n) { // permutations leaving nothing in place: D(n) = (n-1)(D(n-1) + D(n-2))
        if (n == 0) return 1 % mod;
        if (n == 1) return 0;
        ll prev = 1 % mod, cur = 0;
        for (int i = 2; i <= n; i++) {
            ll next = (ll)(i - 1) % mod * ((prev + cur) % mod) % mod;
            prev = cur, cur = next;
        }
        return cur;
    }
};

// nCr for a huge n against a SMALL prime modulus, by Lucas' theorem.
// Needed exactly because Binomial's tables are only valid while n < mod.
ll lucas_nCr(ll n, ll r, ll p) {
    if (r < 0 || r > n) return 0;
    Binomial small((int)p - 1, p); // indices stay below p, where the factorials are still invertible
    ll res = 1;
    while (n > 0 || r > 0) {
        ll ni = n % p, ri = r % p;
        if (ri > ni) return 0;
        res = res * small.nCr(ni, ri) % p;
        n /= p, r /= p;
    }
    return res;
}

// Standard problem: q queries of n and r - report nCr mod 1e9+7
void solve() {
    int q;
    cin >> q;
    Binomial b(1000000);
    while (q--) {
        ll n, r;
        cin >> n >> r;
        cout << b.nCr(n, r) << '\n';
    }
}
