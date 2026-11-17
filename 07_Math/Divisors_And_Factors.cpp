// Divisors And Factors: trial-division factorization, divisor list and divisor count of one number,
// plus sum of sigma(i) for i = 1..n.
// Use when: "divisors of n", "how many divisors", "prime factors of n" for a few n up to ~1e13.
// Usage: prime_factorization(n) -> {2,2,3} | get_factors(n) -> {{2,2},{3,1}} | get_divisors(n)
//        count_divisors(n) | get_sum_div(n) = sum_{i<=n} sigma(i) = sum_{d<=n} d * (n / d)
// Handles: n = 1 (empty factor lists, divisors {1}), primes and prime squares up to 1e18.
// Time: factor / divisors / count O(sqrt(n)) | get_sum_div O(sqrt(n)) by grouping equal n / d
// Indexing: not applicable
// Note: n >= 1. sqrt(1e18) = 1e9 steps (~1 s), so a prime near 1e18 is slow: use Pollard rho there.
//       get_sum_div overflows ll past n ~ 3e9. For many n <= 1e7 use the spf table in Sieve.cpp.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> prime_factorization(ll n) { // with repetition, ascending
    vector<ll> factors;
    while (n % 2 == 0) factors.push_back(2), n /= 2;
    for (ll i = 3; i <= n / i; i += 2)
        while (n % i == 0) factors.push_back(i), n /= i;
    if (n > 2) factors.push_back(n);
    return factors;
}

vector<pair<ll, int>> get_factors(ll n) { // (prime, exponent), ascending
    vector<pair<ll, int>> factors;
    for (ll i = 2, d = 1; i <= n / i; i += d, d = 2)
        if (n % i == 0) {
            factors.push_back({i, 0});
            while (n % i == 0) n /= i, factors.back().second++;
        }
    if (n != 1) factors.push_back({n, 1});
    return factors;
}

vector<ll> get_divisors(ll n) { // ascending
    vector<ll> divs;
    for (ll i = 1; i <= n / i; i++)
        if (n % i == 0) {
            divs.push_back(i);
            if (i != n / i) divs.push_back(n / i);
        }
    sort(divs.begin(), divs.end());
    return divs;
}

ll count_divisors(ll n) {
    ll cnt = 0;
    for (ll i = 1; i <= n / i; i++)
        if (n % i == 0) cnt += (n / i == i) ? 1 : 2;
    return cnt;
}

ll get_sum_div(ll x) { // sum_{i=1..x} sigma(i)
    ll ans = 0;
    for (ll left = 1, right; left <= x; left = right + 1) {
        right = x / (x / left);
        ans += (x / left) * ((left + right) * (right - left + 1) / 2);
    }
    return ans;
}
