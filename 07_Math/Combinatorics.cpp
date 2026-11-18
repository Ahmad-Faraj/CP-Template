// Combinatorics: factorial / inverse-factorial tables mod a prime; nCr, nPr, Catalan, stars-bars.
// Use when: "how many ways to choose / arrange", "modulo 1e9+7", balls into boxes, Catalan numbers.
// Usage: CountLib C(2e6);  C.nCr(n, r);  C.nPr(n, r);  C.catalan(n);  C.stars_bars(n, k);
// Handles: r < 0 or r > n (returns 0), n = 0, any prime modulus below 2^31.
// Time: build O(N) | every query O(1)
// Indexing: build with N = largest factorial index used (catalan(n) needs 2n, stars_bars n + k - 1)
// Note: mod must be PRIME and > N (inverses use the inv[i] recurrence). Asking past N reads out of
//       bounds.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct CountLib {
  private:
    ll MOD;
    vector<ll> fact, inv_fact, inv;

  public:
    CountLib(int n, ll mod = 1e9 + 7) : MOD(mod) {
        if (n < 1) n = 1; // inv[1] must exist
        fact = inv_fact = inv = vector<ll>(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = (fact[i - 1] * i) % MOD;
        inv[1] = 1;
        for (int i = 2; i <= n; i++) inv[i] = MOD - (MOD / i) * inv[MOD % i] % MOD;
        inv_fact[0] = 1;
        for (int i = 1; i <= n; i++) inv_fact[i] = (inv_fact[i - 1] * inv[i]) % MOD;
    }

    // choosing r elements from n elements (order does not matter)
    ll nCr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return (fact[n] * inv_fact[r] % MOD) * inv_fact[n - r] % MOD;
    }

    // choosing r elements from n elements (order matters)
    ll nPr(int n, int r) {
        if (r < 0 || r > n) return 0;
        return (fact[n] * inv_fact[n - r]) % MOD;
    }

    ll catalan(int n) { return nCr(2 * n, n) * inv[n + 1] % MOD; }

    // n indistinguishable balls into k distinguishable boxes (empty boxes allowed)
    // = ways to place k - 1 dividers among n items
    ll stars_bars(int n, int k) { return k == 0 ? (n == 0) : nCr(n + k - 1, k - 1); }
};
