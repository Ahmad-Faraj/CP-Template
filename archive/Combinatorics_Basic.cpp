#include <bits/stdc++.h>
using namespace std;
#define ll long long

ll factorial(ll x) {
    if (x <= 1) return 1;
    return x * factorial(x - 1);
}

const int FACT_N = 2 + 3;
const int MOD = 1e9 + 7;
ll fact[FACT_N], inv_fact[FACT_N];
void compute_fact() {
    fact[0] = inv_fact[0] = 1;
    for (int i = 1; i < FACT_N; i++) {
        fact[i] = fact[i - 1] * i % MOD;
        inv_fact[i] = mod_inv(fact[i]);
    }
}

ll nPr(ll x, ll y) { return fact(x) / (fact(x - y)); }

ll nCr(ll n, ll r) {
    if (r > n) return 0;
    ll p = 1, k = 1;
    if (n - r < r) r = n - r;
    // condition for minimum choose
    if (n < 1) return 0;
    while (r > 0) {
        p *= n, k *= r;
        ll m = __gcd(p, k);
        p /= m, k /= m, n--, r--;
    }
    return p;
}

ll nCr_mod(int n, int r) {
    if (r > n || r < 0) return 0;
    return fact[n] * inv_fact[r] % MOD * inv_fact[n - r] % MOD;
}
