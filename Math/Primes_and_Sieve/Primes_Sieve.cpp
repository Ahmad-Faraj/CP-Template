#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define endl "\n"
#define sz(x) (int)x.size()
#define all(x) x.begin(), x.end()

// O(sqrt(n))
bool isprime(ll x) {
    if (x <= 1) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (int i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

// O(nloglog(n))
vector<int> primes;
vector<int> sieve(int n) {
    vector<bool> is_prime(n + 1, true);
    vector<int> primes;

    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i <= n; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * i; j <= n; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return primes;
}