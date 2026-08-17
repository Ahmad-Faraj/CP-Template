// Sieve: primes up to n, plus a smallest-prime-factor table that factorizes any number below n in O(log n).
// Use when: "all primes up to n", "is this prime", factorizing many numbers, divisor or Mobius tables.
// Handles: n = 0 and 1, a single primality test past the sieve limit via trial division, and repeated factorizations.
// Time: sieve O(n log log n) | smallest_prime_factor build O(n) | factorize O(log n) | is_prime O(sqrt(n))
// Indexing: 0-based tables of length n + 1, so index i describes the number i
// Note: build the table once and reuse it. factorize only works for values <= the limit; is_prime works for any value.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

bool is_prime(ll x) { // trial division, safe for any x up to about 1e18
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (ll i = 5; i <= x / i; i += 6) // i <= x / i avoids i * i overflowing
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

vector<int> primes_up_to(int n) {
    if (n < 2) return {};
    vector<char> composite(n + 1, 0);
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
        if (composite[i]) continue;
        primes.push_back(i);
        for (ll j = (ll)i * i; j <= n; j += i) composite[j] = 1; // ll, or i * i overflows past 46341
    }
    return primes;
}

struct Sieve {
    int n;
    vector<int> spf;    // spf[i] = the smallest prime dividing i; spf[0] = spf[1] = 0
    vector<int> primes; // every prime <= n, ascending

    Sieve(int n) : n(n), spf(n + 1, 0) {
        for (int i = 2; i <= n; i++) {
            if (!spf[i]) {
                spf[i] = i;
                primes.push_back(i);
            }
            for (int p : primes) {
                if (p > spf[i] || (ll)i * p > n) break;
                spf[(ll)i * p] = p;
            }
        }
    }

    bool prime(int x) { return x >= 2 && x <= n && spf[x] == x; }

    vector<pair<int, int>> factorize(int x) { // (prime, exponent), ascending; empty for x <= 1
        vector<pair<int, int>> out;
        while (x > 1) {
            int p = spf[x], e = 0;
            while (x % p == 0) x /= p, e++;
            out.push_back({p, e});
        }
        return out;
    }

    vector<int> prime_factors(int x) { // with repetition, ascending
        vector<int> out;
        while (x > 1) {
            out.push_back(spf[x]);
            x /= spf[x];
        }
        return out;
    }

    vector<int> divisors(int x) { // every divisor, ascending
        vector<int> divs{1};
        for (auto [p, e] : factorize(x)) {
            int was = (int)divs.size();
            ll power = 1;
            for (int k = 1; k <= e; k++) {
                power *= p;
                for (int i = 0; i < was; i++) divs.push_back((int)(divs[i] * power));
            }
        }
        sort(divs.begin(), divs.end());
        return divs;
    }

    int mobius(int x) { // 0 when x has a squared prime factor, else +/-1 by the count of primes
        if (x == 1) return 1;
        int mu = 1;
        for (auto [p, e] : factorize(x)) {
            (void)p;
            if (e > 1) return 0;
            mu = -mu;
        }
        return mu;
    }
};

// Standard problem: report the primes up to n, then factorize q queries using the same table
void solve() {
    int n, q;
    cin >> n >> q;
    Sieve s(n);
    cout << s.primes.size() << '\n';
    while (q--) {
        int x;
        cin >> x;
        for (auto [p, e] : s.factorize(x)) cout << p << '^' << e << ' ';
        cout << '\n';
    }
}
