// Euler Phi: how many integers in 1..n are coprime to n, for one n or for every n up to a limit.
// Use when: counting coprime pairs, shrinking an exponent by Euler's theorem, "how many i/n are already reduced".
// Handles: n = 1 (phi = 1), primes, prime powers, and a table for every value up to a limit.
// Time: phi O(sqrt(n)) | phi_table O(n log log n) | phi_sum O(n)
// Indexing: 0-based table of length limit + 1, so index i holds phi(i); phi_table[0] = 0
// Note: a^k = a^(k mod phi(m)) (mod m) needs gcd(a, m) == 1 - notes/Knowledge.md has the general form.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll phi(ll n) { // one value, by trial division
    if (n <= 0) return 0;
    ll result = n;
    for (ll p = 2; p <= n / p; p++)
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    if (n > 1) result -= result / n;
    return result;
}

vector<ll> phi_table(int limit) { // phi(i) for every i in 0..limit
    vector<ll> f(limit + 1);
    iota(f.begin(), f.end(), 0LL);
    for (int p = 2; p <= limit; p++)
        if (f[p] == p) // p is prime, so sieve its multiples
            for (int j = p; j <= limit; j += p) f[j] -= f[j] / p;
    return f;
}

ll phi_from_factors(const vector<pair<ll, int>> &factors) { // when the factorization is already known
    ll result = 1;
    for (auto [p, e] : factors) {
        ll power = 1;
        for (int i = 0; i < e - 1; i++) power *= p;
        result *= (p - 1) * power;
    }
    return result;
}

// sum of phi(1..limit); also the count of coprime pairs (a, b) with 1 <= a <= b <= limit
ll phi_sum(int limit) {
    vector<ll> f = phi_table(limit);
    ll total = 0;
    for (int i = 1; i <= limit; i++) total += f[i];
    return total;
}

// Standard problem: for each query n, report phi(n)
void solve() {
    int q;
    cin >> q;
    while (q--) {
        ll n;
        cin >> n;
        cout << phi(n) << '\n';
    }
}
