// Sieve: primes up to n, plus a smallest-prime-factor table that factorizes any number below n in O(log n).
// Use when: "all primes up to n", "is this prime", factorizing many numbers, divisor or Mobius tables.
// Handles: n = 0 and 1, a single primality test past the sieve limit via trial division, and repeated factorizations.

#include <bits/stdc++.h>
using namespace std;

const int MAXV = 1e7 + 5;
vector<int> spf(MAXV); // spf[i] = the smallest prime dividing i
vector<int> primes;    // every prime < MAXV, ascending

void sieve() {
    spf[0] = spf[1] = 1;
    for (int i = 2; i < MAXV; i++) spf[i] = i;
    for (int i = 4; i < MAXV; i += 2) spf[i] = 2;
    for (int i = 3; i * i < MAXV; i += 2) {
        if (spf[i] == i) {
            for (int j = i * i; j < MAXV; j += 2 * i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    if (MAXV > 2) primes.push_back(2);
    for (int i = 3; i < MAXV; i += 2) {
        if (spf[i] == i) primes.push_back(i);
    }
}

bool is_prime(long long x) { // Trial division, safe for any x up to about 1e18
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (long long i = 5; i <= x / i; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

bool prime(int x) { // O(1) query for x < MAXV
    return x >= 2 && x < MAXV && spf[x] == x;
}

vector<pair<int, int>> factorize(int x) { // (prime, exponent), ascending; empty for x <= 1
    vector<pair<int, int>> out;
    while (x > 1) {
        int p = spf[x], e = 0;
        while (x % p == 0) {
            x /= p;
            e++;
        }
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
        long long power = 1;
        for (int k = 1; k <= e; k++) {
            power *= p;
            for (int i = 0; i < was; i++) {
                divs.push_back((int)(divs[i] * power));
            }
        }
    }
    sort(divs.begin(), divs.end());
    return divs;
}

int mobius(int x) { // 0 when x has a squared prime factor, else +/-1 by the count of primes
    if (x == 1) return 1;
    int mu = 1;
    for (auto [p, e] : factorize(x)) {
        if (e > 1) return 0;
        mu = -mu;
    }
    return mu;
}