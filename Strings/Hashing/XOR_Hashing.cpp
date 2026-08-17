// XOR Hashing: tags each distinct value with a random 64-bit number so a set of values collapses to one XOR.
// Use when: "do these two ranges hold the same set of distinct values", comparing sets where multiplicity is ignored.
// Handles: any value usable as a map key, prefix or range comparison, order-independent. Multiplicity is ignored.
// Time: build O(n log n) | comparing two prefixes O(1)
// Indexing: 0-based
// Note: all arrays being compared must come from the SAME XorHashing object, so they share the tag table.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

struct XorHashing {
    map<ll, ll> tag;
    mt19937_64 gen{(unsigned long long)chrono::steady_clock::now().time_since_epoch().count()};

    ll tag_of(ll v) { // the random tag for value v, created on first sight
        auto it = tag.find(v);
        if (it != tag.end()) return it->second;
        return tag[v] = (ll)(gen() >> 1);
    }

    vector<ll> prefix(const vector<ll> &a) { // pref[i] = XOR of the tags of the distinct values in a[0 .. i]
        set<ll> seen;
        vector<ll> pref(sz(a), 0);
        for (int i = 0; i < sz(a); i++) {
            if (!seen.count(a[i])) pref[i] = tag_of(a[i]), seen.insert(a[i]);
            if (i) pref[i] ^= pref[i - 1];
        }
        return pref;
    }
};

// Standard problem: two arrays and q queries asking whether a[0..x] and b[0..y] hold the same set of values
void solve() {
    int n;
    cin >> n;
    vector<ll> a(n), b(n);
    for (auto &x : a) cin >> x;
    for (auto &x : b) cin >> x;
    XorHashing h;
    vector<ll> pa = h.prefix(a), pb = h.prefix(b);
    int q;
    cin >> q;
    while (q--) {
        int x, y;
        cin >> x >> y;
        cout << (pa[x - 1] == pb[y - 1] ? "Yes" : "No") << '\n';
    }
}
