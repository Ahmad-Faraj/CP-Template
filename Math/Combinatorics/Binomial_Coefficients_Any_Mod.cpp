// Binomial Coefficients Any Mod: nCr modulo a modulus that is NOT prime, by splitting it into prime powers.
// Use when: the problem hands you a composite modulus, so factorials are not invertible and the usual table fails.
// Handles: any modulus, huge n and r, r out of range, and prime powers where the answer is 0 for divisibility reasons.
// Time: O(m log n log m), dominated by building a table of size p^k per prime power of m
// Indexing: not applicable
// Note: the cost is driven by the LARGEST prime power dividing m, so this is practical while m stays modest.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll pow_mod_small(ll b, ll e, ll m) {
    ll r = 1 % m;
    b %= m;
    if (b < 0) b += m;
    while (e) {
        if (e & 1) r = (ll)((__int128)r * b % m);
        b = (ll)((__int128)b * b % m);
        e >>= 1;
    }
    return r;
}

ll extended_gcd_any(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extended_gcd_any(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}

ll inverse_mod(ll a, ll m) { // -1 when a is not invertible
    ll x, y;
    if (extended_gcd_any(((a % m) + m) % m, m, x, y) != 1) return -1;
    return ((x % m) + m) % m;
}

// n! modulo p^k with every factor of p stripped out, which is what makes it invertible
ll factorial_without_p(ll n, ll p, ll mod, const vector<ll> &table) {
    ll result = 1 % mod;
    while (n > 1) {
        result = (ll)((__int128)result * table[n % mod] % mod);
        result = (ll)((__int128)result * pow_mod_small(table[mod], n / mod, mod) % mod);
        n /= p;
    }
    return result;
}

ll multiplicity(ll n, ll p) { // the exponent of p in n!, by Legendre
    ll total = 0;
    while (n) total += (n /= p);
    return total;
}

ll ncr_prime_power(ll n, ll r, ll p, int k) { // nCr modulo p^k
    if (r < 0 || r > n) return 0;
    ll mod = 1;
    for (int i = 0; i < k; i++) mod *= p;
    ll t = multiplicity(n, p) - multiplicity(r, p) - multiplicity(n - r, p);
    if (t >= k) return 0; // p^k divides the answer outright

    vector<ll> table(mod + 1); // built ONCE and shared by the three factorials
    table[0] = 1 % mod;
    for (ll i = 1; i <= mod; i++) table[i] = (i % p) ? (ll)((__int128)table[i - 1] * i % mod) : table[i - 1];

    ll a = factorial_without_p(n, p, mod, table);
    ll b = factorial_without_p(r, p, mod, table);
    ll c = factorial_without_p(n - r, p, mod, table);
    ll result = (ll)((__int128)a * inverse_mod(b, mod) % mod);
    result = (ll)((__int128)result * inverse_mod(c, mod) % mod);
    return (ll)((__int128)result * pow_mod_small(p, t, mod) % mod);
}

// merge x = a1 (mod m1) with x = a2 (mod m2); m is -1 when they contradict
pair<ll, ll> crt_merge(ll a1, ll m1, ll a2, ll m2) {
    ll p, q;
    ll g = extended_gcd_any(m1, m2, p, q);
    if ((a2 - a1) % g != 0) return {0, -1};
    ll lcm = m1 / g * m2;
    ll step = (ll)((__int128)((a2 - a1) / g % (m2 / g)) * (p % (m2 / g)) % (m2 / g));
    ll x = (ll)(((__int128)m1 * step + a1) % lcm);
    return {(x % lcm + lcm) % lcm, lcm};
}

ll ncr_any_mod(ll n, ll r, ll m) { // nCr mod m for ANY m
    if (r < 0 || r > n) return 0;
    pair<ll, ll> answer{0, 1};
    for (ll p = 2; p * p <= m; p++) { // factor m by trial division; no global sieve needed
        if (m % p) continue;
        int k = 0;
        ll power = 1;
        while (m % p == 0) m /= p, power *= p, k++;
        answer = crt_merge(answer.first, answer.second, ncr_prime_power(n, r, p, k), power);
    }
    if (m > 1) answer = crt_merge(answer.first, answer.second, ncr_prime_power(n, r, m, 1), m);
    return answer.first;
}

// Standard problem: q queries of n, r and m - report nCr mod m, where m need not be prime
void solve() {
    int q;
    cin >> q;
    while (q--) {
        ll n, r, m;
        cin >> n >> r >> m;
        cout << ncr_any_mod(n, r, m) << '\n';
    }
}
