#include <bits/stdc++.h>
using namespace std;
#define ll long long

struct CountLib {
  private:
    const int MOD = 1e9 + 7;
    vector<ll> fact, inv_fact, inv;

  public:
    CountLib(int n, int mod = 1e9 + 7) : MOD(mod) {
        fact = inv_fact = inv = vector<ll>(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = (fact[i - 1] * i) % MOD;
        }
        inv[1] = 1;
        for (int i = 2; i <= n; i++) {
            inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
        }
        inv_fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            inv_fact[i] = (inv_fact[i - 1] * inv[i]) % MOD;
        }
    }

    // choosing r elements from n elements (order does not matter)
    ll nCr(int n, int r) {
        if (r < 0 || r > n) {
            return 0;
        }
        return (fact[n] * inv_fact[r] % MOD) * inv_fact[n - r] % MOD;
    }

    // choosing r elements from n elements (order matters)
    ll nPr(int n, int r) {
        if (r < 0 || r > n) {
            return 0;
        }
        return (fact[n] * inv_fact[n - r]) % MOD;
    }

    ll catalan(int n) { return nCr(2 * n, n) * inv[n + 1] % MOD; }

    // number of ways to put n indistinguishable balls into k distinguishable boxes
    // number of ways to but k indistinguishable dividers between n indistinguishable items
    ll stars_bars(int n, int k) { return nCr(n + k - 1, k - 1); }
};