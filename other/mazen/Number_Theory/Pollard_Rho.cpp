#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// Pollard's Rho algorithm for integer factorization
// Complexity: O(n^(1/4))
namespace PollardRho {
    mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
    const int MAX_PRIME = 1e6 + 9;
    ll sequence[MAX_PRIME];
    int primes[MAX_PRIME], smallest_prime_factor[MAX_PRIME];

    // Function to add two numbers modulo m
    inline ll add_mod(ll x, ll y, ll m) {
        return (x += y) < m ? x : x - m;
    }

    // Function to multiply two numbers modulo m
    inline ll mul_mod(ll x, ll y, ll m) {
        ll res = __int128(x) * y % m;
        return res;
    }

    // Function to compute (x^n) % m
    inline ll pow_mod(ll x, ll n, ll m) {
        ll res = 1 % m;
        for (; n; n >>= 1) {
            if (n & 1) res = mul_mod(res, x, m);
            x = mul_mod(x, x, m);
        }
        return res;
    }

    // Miller-Rabin primality test
    inline bool miller_rabin(ll n) {
        if (n <= 2 || (n % 2 == 0)) return (n == 2);
        if (n < MAX_PRIME) return smallest_prime_factor[n] == n;

        ll s = 0, r = n - 1;
        while (!(r & 1)) {
            r >>= 1;
            s++;
        }

        for (int i = 0; primes[i] < n && primes[i] < 32; i++) {
            ll c = pow_mod(primes[i], r, n);
            for (int j = 0; j < s; j++) {
                ll d = mul_mod(c, c, n);
                if (d == 1 && c != 1 && c != (n - 1)) return false;
                c = d;
            }
            if (c != 1) return false;
        }
        return true;
    }

    // Initialize smallest prime factors and prime numbers
    void init() {
        int cnt = 0;
        for (int i = 2; i < MAX_PRIME; i++) {
            if (!smallest_prime_factor[i]) {
                primes[cnt++] = smallest_prime_factor[i] = i;
            }
            for (int j = 0, k; (k = i * primes[j]) < MAX_PRIME; j++) {
                smallest_prime_factor[k] = primes[j];
                if (smallest_prime_factor[i] == smallest_prime_factor[k]) break;
            }
        }
    }

    // Pollard's Rho algorithm for integer factorization
    ll pollard_rho(ll n) {
        while (true) {
            ll x = rnd() % n, y = x, c = rnd() % n, u = 1, v, t = 0;
            ll *px = sequence, *py = sequence;
            while (true) {
                *py++ = y = add_mod(mul_mod(y, y, n), c, n);
                *py++ = y = add_mod(mul_mod(y, y, n), c, n);
                if ((x = *px++) == y) break;
                v = u;
                u = mul_mod(u, abs(y - x), n);
                if (!u) return __gcd(v, n);
                if (++t == 32) {
                    t = 0;
                    if ((u = __gcd(u, n)) > 1 && u < n) return u;
                }
            }
            if (t && (u = __gcd(u, n)) > 1 && u < n) return u;
        }
    }

    // Function to factorize a number
    vector<ll> factorize(ll n) {
        if (n == 1) return vector<ll>();
        if (miller_rabin(n)) return vector<ll>{n};

        vector<ll> factors, sub_factors;
        while (n > 1 && n < MAX_PRIME) {
            factors.push_back(smallest_prime_factor[n]);
            n /= smallest_prime_factor[n];
        }
        if (n >= MAX_PRIME) {
            ll factor = pollard_rho(n);
            factors = factorize(factor);
            sub_factors = factorize(n / factor);
            factors.insert(factors.end(), sub_factors.begin(), sub_factors.end());
        }
        return factors;
    }
    static const int initialize = []() {
        init();
        return 0;
    }();
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);


    int t;
    cin >> t;
    while (t--) {
        ll n;
        cin >> n;
        auto factors = PollardRho::factorize(n);
        sort(factors.begin(), factors.end());
        cout << factors.size() << ' ';
        for (auto factor : factors) cout << factor << ' ';
        cout << '\n';
    }

    return 0;
}