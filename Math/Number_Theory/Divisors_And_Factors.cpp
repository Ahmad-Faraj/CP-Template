// Divisors And Factors: factorize one large number, list its divisors, and count or sum divisors over a whole range.
// Use when: "how many divisors has n", "sum of divisors of every i up to n", factorizing one number near 1e18.
// Handles: n = 1, primes, perfect squares, and range sums in O(sqrt(n)) by grouping equal quotients.
// Time: factorize O(sqrt(n)) | divisors O(sqrt(n)) | the range sums O(sqrt(n))
// Indexing: not applicable
// Note: for factorizing MANY numbers build the smallest-prime-factor table in Sieve.cpp instead - it is O(log n) each.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<pair<ll, int>> factorize(ll n) { // (prime, exponent), ascending; empty for n <= 1
    vector<pair<ll, int>> out;
    for (ll p = 2; p <= n / p; p++) { // p <= n / p, so p * p never overflows
        if (n % p) continue;
        int e = 0;
        while (n % p == 0) n /= p, e++;
        out.push_back({p, e});
    }
    if (n > 1) out.push_back({n, 1});
    return out;
}

vector<ll> prime_factors(ll n) { // with repetition, ascending
    vector<ll> out;
    for (auto [p, e] : factorize(n))
        for (int i = 0; i < e; i++) out.push_back(p);
    return out;
}

vector<ll> divisors(ll n) { // every divisor, ascending
    vector<ll> small, large;
    for (ll i = 1; i <= n / i; i++)
        if (n % i == 0) {
            small.push_back(i);
            if (i != n / i) large.push_back(n / i);
        }
    small.insert(small.end(), large.rbegin(), large.rend());
    return small;
}

ll count_divisors(ll n) { // from the exponents: product of (e + 1)
    ll total = 1;
    for (auto [p, e] : factorize(n)) {
        (void)p;
        total *= e + 1;
    }
    return total;
}

ll sum_divisors(ll n) { // from the exponents: product of (p^(e+1) - 1) / (p - 1)
    ll total = 1;
    for (auto [p, e] : factorize(n)) {
        ll term = 1, power = 1;
        for (int i = 0; i < e; i++) {
            power *= p;
            term += power;
        }
        total *= term;
    }
    return total;
}

// sum over i = 1..n of the number of divisors of i, by grouping equal values of n / i
ll total_divisor_count(ll n) {
    ll total = 0;
    for (ll left = 1, right; left <= n; left = right + 1) {
        right = n / (n / left);
        total += (n / left) * (right - left + 1);
    }
    return total;
}

// sum over i = 1..n of the sum of divisors of i, same grouping
ll total_divisor_sum(ll n) {
    ll total = 0;
    for (ll left = 1, right; left <= n; left = right + 1) {
        right = n / (n / left);
        ll count = right - left + 1;
        ll block = (left + right) * count / 2; // sum of the divisors in this block
        total += (n / left) * block;
    }
    return total;
}

// Standard problem: for each query n, report its factorization, divisor count and divisor sum
void solve() {
    int q;
    cin >> q;
    while (q--) {
        ll n;
        cin >> n;
        for (auto [p, e] : factorize(n)) cout << p << '^' << e << ' ';
        cout << "| " << count_divisors(n) << ' ' << sum_divisors(n) << '\n';
    }
}
