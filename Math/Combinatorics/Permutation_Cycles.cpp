// Permutations: enumerate them, apply one k times, decompose into cycles, and read off order and parity.
// Use when: "where does each element land after k shuffles", "how many shuffles restore the order", the sign.
// Handles: any permutation of 0..n-1, fixed points as one-element cycles, k = 0, and the identity.
// Time: apply O(n) | power O(n log k) | decompose O(n) | order O(n log n)
// Indexing: 0-based, and the values are also 0..n-1; p[i] is where position i reads from
// Note: order() can exceed a long long for large n, so it is taken modulo nothing - use order_mod when n is past ~40.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> apply_permutation(const vector<int> &a, const vector<int> &p) { // result[i] = a[p[i]]
    int n = (int)a.size();
    vector<int> res(n);
    for (int i = 0; i < n; i++) res[i] = a[p[i]];
    return res;
}

vector<int> permute_times(vector<int> a, vector<int> p, ll k) { // a after applying p exactly k times
    while (k > 0) {
        if (k & 1) a = apply_permutation(a, p);
        p = apply_permutation(p, p);
        k >>= 1;
    }
    return a;
}

vector<vector<int>> decompose(const vector<int> &p) { // the cycles, each starting at its smallest index
    int n = (int)p.size();
    vector<vector<int>> cycles;
    vector<char> seen(n, 0);
    for (int start = 0; start < n; start++) {
        if (seen[start]) continue;
        vector<int> cycle;
        for (int x = start; !seen[x]; x = p[x]) { // a separate variable, so the outer loop is untouched
            seen[x] = 1;
            cycle.push_back(x);
        }
        cycles.push_back(cycle);
    }
    return cycles;
}

vector<int> restore(int n, const vector<vector<int>> &cycles) { // rebuild p from its cycles
    vector<int> p(n);
    for (const vector<int> &c : cycles) {
        int m = (int)c.size();
        for (int i = 0; i < m; i++) p[c[i]] = c[(i + 1) % m];
    }
    return p;
}

vector<vector<int>> power_cycles(const vector<int> &p, ll k) { // the cycles of p^k, without building p^k
    vector<vector<int>> out;
    for (const vector<int> &c : decompose(p)) {
        ll len = (ll)c.size(), g = __gcd(k % len == 0 ? len : k, len);
        for (ll i = 0; i < g; i++) {
            vector<int> w;
            for (ll j = i, cnt = 0; cnt < len / g; cnt++, j = (j + k) % len) w.push_back(c[(size_t)j]);
            out.push_back(w);
        }
    }
    return out;
}

ll order(const vector<int> &p) { // smallest k > 0 with p^k the identity: the lcm of the cycle lengths
    ll result = 1;
    for (const vector<int> &c : decompose(p)) {
        ll len = (ll)c.size();
        result = result / __gcd(result, len) * len; // divide first, so it overflows as late as possible
    }
    return result;
}

ll order_mod(const vector<int> &p, ll mod) { // the same, reduced, for when the true order does not fit
    map<ll, int> best; // the highest power of each prime across all cycle lengths
    for (const vector<int> &c : decompose(p)) {
        ll len = (ll)c.size();
        for (ll f = 2; f * f <= len; f++) {
            int e = 0;
            while (len % f == 0) len /= f, e++;
            if (e) best[f] = max(best[f], e);
        }
        if (len > 1) best[len] = max(best[len], 1);
    }
    ll result = 1 % mod;
    for (auto [prime, e] : best)
        for (int i = 0; i < e; i++) result = (ll)((__int128)result * prime % mod);
    return result;
}

int sign(const vector<int> &p) { // +1 for an even permutation, -1 for an odd one
    int parity = 0;
    for (const vector<int> &c : decompose(p)) parity ^= (int)((c.size() - 1) & 1);
    return parity ? -1 : 1;
}

void for_each_permutation(vector<int> items, const function<void(const vector<int> &)> &visit) {
    sort(items.begin(), items.end());
    do {
        visit(items);
    } while (next_permutation(items.begin(), items.end()));
}

// Standard problem: Timus 1024 - the smallest k for which applying the permutation k times restores the order
void solve() {
    int n;
    cin >> n;
    vector<int> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
        --p[i];
    }
    cout << order(p) << '\n';
}
