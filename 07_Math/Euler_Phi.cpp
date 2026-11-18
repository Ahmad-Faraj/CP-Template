// Euler Phi: phi(n) = how many of 1..n are coprime to n, for one n or for every n up to a limit.
// Use when: coprime pairs / reduced fractions, reducing an exponent mod phi(m), gcd(i, n) = 1.
// Usage: phi(n);  vector<ll> f = phi_sieve(MAX);  // f[i] = phi(i)
// Handles: n = 1 (phi = 1), primes, prime powers, n up to 9e18 (no i*i overflow).
// Time: phi O(sqrt(n)) | phi_sieve O(MAX log log MAX)
// Indexing: phi_sieve returns MAX + 1 values, index i holds phi(i), f[0] = 0
// Note: a^k = a^(k mod phi(m)) (mod m) only when gcd(a, m) = 1. phi(n) for n <= 0 returns n.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll phi(ll n) {
    ll result = n;
    for (ll i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            while (n % i == 0) n /= i;
            result -= result / i;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

vector<ll> phi_sieve(int MAX) {
    vector<ll> phi(MAX + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int i = 2; i <= MAX; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= MAX; j += i) phi[j] -= phi[j] / i;
        }
    }
    return phi;
}
