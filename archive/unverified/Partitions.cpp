// Partitions: p(0..n) mod a prime, p(i) = number of ways to write i as an unordered sum of parts.
// Use when: "count partitions of n", generating functions 1 / prod(1 - x^k), Euler's pentagonal
// theorem, "parts not divisible by d" = sum over pentagonal g of +-p(n - d*g) (ACPC 2025 H).
// Usage: vector<ll> p = partitions(n, 998244353); p[i];
// Time: O(n sqrt n) (n = 5e5 in ~1 s)
// Indexing: p[0..n], p[0] = 1
// Note: p(i) = sum over k >= 1 of (-1)^(k+1) (p(i - k(3k-1)/2) + p(i - k(3k+1)/2)).
// Source: Shohag's code library "Partition Function" (modint replaced by plain ll mod).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<ll> partitions(int n, ll mod) {
    vector<ll> p(n + 1, 0);
    vector<pair<int, ll>> gp; // (generalized pentagonal number, sign as +1 / mod - 1)
    for (ll k = 1; k * (3 * k - 1) / 2 <= n; k++) {
        ll s = k % 2 ? 1 : mod - 1;
        gp.push_back({(int)(k * (3 * k - 1) / 2), s});
        if (k * (3 * k + 1) / 2 <= n) gp.push_back({(int)(k * (3 * k + 1) / 2), s});
    }
    p[0] = 1 % mod;
    for (int i = 1; i <= n; i++) {
        ll s = 0;
        for (auto [g, sg] : gp) {
            if (g > i) break;
            s += sg == 1 ? p[i - g] : mod - p[i - g];
            if (s >= mod) s -= mod;
        }
        p[i] = s;
    }
    return p;
}
