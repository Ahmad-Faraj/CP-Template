#include <bits/stdc++.h>
using namespace std;
#define ll long long

ll phi(ll n) {
    ll result = n;
    for (ll i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            while (n % i == 0) {
                n /= i;
            }
            result -= result / i;
        }
    }
    if (n > 1) {
        result -= result / n;
    }
    return result;
}

vector<ll> phi_sieve(int MAX) {
    vector<ll> phi(MAX + 1);
    iota(phi.begin(), phi.end(), 0);
    for (int i = 2; i <= MAX; i++) {
        if (phi[i] == i) {
            for (int j = i; j <= MAX; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
    return phi;
}