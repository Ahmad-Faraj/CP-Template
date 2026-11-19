// Permutation Cycles: splits a permutation into cycles, rebuilds it, finds its order and powers.
// Use when: "after how many shuffles is the deck back", apply a permutation k times, cycles.
// Usage: auto cyc = decompose(p); ll t = order(p); auto pk = restore(n, power(p, k));
// Handles: fixed points, any k including 0, negative (inverse powers) and huge (up to 1e18).
// Time: O(n) for each function (plus O(log) gcds)
// Indexing: 0-based, p is a permutation of 0..n-1; p^k means applying p k times: x -> p[p[..p[x]]]
// Note: order() is the lcm of the cycle lengths; it always fits in ll for n <= 347 and can overflow
//       beyond. For large n take the lcm mod M from each prime's max exponent over cycle lengths.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<vector<int>> decompose(const vector<int> &p) { // each cycle lists i, p[i], p[p[i]], ...
    int n = p.size();
    vector<vector<int>> cycles;
    vector<bool> vis(n, false);
    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;
        vector<int> v;
        for (int j = i; !vis[j]; j = p[j]) v.push_back(j), vis[j] = true;
        cycles.push_back(v);
    }
    return cycles;
}

vector<int> restore(int n, const vector<vector<int>> &cycles) { // inverse of decompose
    vector<int> p(n);
    for (auto &v : cycles) {
        int m = v.size();
        for (int i = 0; i < m; i++) p[v[i]] = v[(i + 1) % m];
    }
    return p;
}

ll order(const vector<int> &p) { // smallest t >= 1 with p^t = identity
    ll ans = 1;
    for (auto &v : decompose(p)) ans = ans / __gcd(ans, (ll)v.size()) * (ll)v.size();
    return ans;
}

vector<vector<int>> power(const vector<int> &p, ll k) { // cycle decomposition of p^k
    vector<vector<int>> ans;
    for (auto &v : decompose(p)) {
        int len = v.size();
        int s = (k % len + len) % len, g = __gcd(s, len);
        for (int i = 0; i < g; i++) {
            vector<int> w;
            for (int j = i, cnt = 0; cnt < len / g; cnt++, j = (j + s) % len) w.push_back(v[j]);
            ans.push_back(w);
        }
    }
    return ans;
}
