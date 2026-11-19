// LDE N Vars Non Negative: which totals k = sum a[i] * x[i] with all x[i] >= 0 are reachable.
// Use when: "pay exactly k with unlimited coins a[i]", "how many k in [l, r] are reachable".
// Usage: LDE t(a); t.can(k); t.count(l, r);
// Handles: duplicates, gcd(a) > 1 (unreachable residues), k = 0 (always reachable), negative k / l.
// Time: build O(min(a) * n * log(min(a))) | can O(1) | count O(min(a))
// Indexing: 0-based a
// Note: every a[i] > 0; min(a) is the table size, so keep min(a) <= ~1e6. k up to ~9e18.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LDE {
    static constexpr ll inf = LLONG_MAX;
    int n, x;
    vector<ll> d;
    LDE() {}
    template <class T> LDE(const vector<T> &a) {
        n = a.size();
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
        x = *min_element(a.begin(), a.end());
        assert(x > 0);
        d.assign(x, inf);
        d[0] = 0;
        q.push({0, 0});
        while (!q.empty()) {
            auto [val, u] = q.top();
            q.pop();
            if (d[u] != val) continue;
            for (int i = 0; i < n; ++i) {
                ll tmp = val + a[i];
                int v = tmp % x;
                if (d[v] > tmp) {
                    d[v] = tmp;
                    q.push({tmp, v});
                }
            }
        }
    }
    bool can(ll k) { return k >= 0 && d[k % x] <= k; }
    ll count(ll l, ll r) { // number of reachable k with l <= k <= r
        ll ans = 0;
        for (int i = 0; i < x; i++) {
            ans += d[i] <= r ? (r - d[i]) / x + 1 : 0;
            ans -= d[i] <= l - 1 ? (l - 1 - d[i]) / x + 1 : 0;
        }
        return ans;
    }
};
