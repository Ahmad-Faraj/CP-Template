// Partitions: p(0..n) = number of ways to write i as an unordered sum of positive integers, mod m.
// Use when: "count multisets of positive integers summing to n", n up to ~5e5, generating
//           function prod 1 / (1 - x^k).
// Usage: vector<ll> p = partitions(n, 1000000007);
// Time: O(n sqrt n)
// Indexing: p[i] for i in 0..n, p[0] = 1
// Note: only + and - are used, so any modulus m <= 4e18 works (m = 0 is not allowed).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> partitions(int n, ll mod) {
    vector<ll> p(n + 1, 0);
    vector<pair<int, ll>> gp;
    for (ll k = 1; k * (3 * k - 1) / 2 <= n; k++) {
        gp.push_back({k % 2 ? 1 : -1, k * (3 * k - 1) / 2});
        gp.push_back({k % 2 ? 1 : -1, k * (3 * k + 1) / 2});
    }
    p[0] = 1 % mod;
    for (int i = 1; i <= n; i++) {
        ll s = 0;
        for (auto [sign, g] : gp) {
            if (g > i) break;
            s += sign > 0 ? p[i - g] : mod - p[i - g];
            if (s >= mod) s -= mod;
        }
        p[i] = s;
    }
    return p;
}
