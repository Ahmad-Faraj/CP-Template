// CHT (Line Container): maximum of a set of lines y = kx + m at a query x.
// Use when: dp[i] = max(dp[j] + a[j]*x[i]), "best over all lines", cost = rate * amount + fixed.
// Handles: any slope order, any query order, duplicate slopes, negative k/m/x, min or max. No deletion, not persistent.
// Time: add O(log n) | query O(log n)
// Indexing: no index; lines live in a multiset
// Note: slopes and queries may arrive in any order. For minimum pass is_min = 1 to BOTH add() and query().

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Line {
    mutable ll k, m, p;
    bool operator<(const Line &o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; }
};

struct LineContainer : multiset<Line, less<>> {
    static const ll inf = LLONG_MAX;

    ll floor_div(ll a, ll b) { return a / b - ((a ^ b) < 0 && a % b); }

    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k)
            x->p = x->m > y->m ? inf : -inf;
        else
            x->p = floor_div(y->m - x->m, x->k - y->k);
        return x->p >= y->p;
    }

    void add(ll k, ll m, bool is_min = false) { // insert y = kx + m
        if (is_min) k = -k, m = -m;
        auto z = insert({k, m, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }

    ll query(ll x, bool is_min = false) { // best value over all inserted lines at x
        assert(!empty());
        auto l = *lower_bound(x);
        return is_min ? -(l.k * x + l.m) : l.k * x + l.m;
    }
};

// Standard problem: add lines online, ask the minimum at x (Library Checker - Line Add Get Min)
void solve() {
    int n, q;
    cin >> n >> q;
    LineContainer lc;
    for (int i = 0; i < n; i++) {
        ll a, b;
        cin >> a >> b;
        lc.add(a, b, true);
    }
    while (q--) {
        int type;
        cin >> type;
        if (type == 0) {
            ll a, b;
            cin >> a >> b;
            lc.add(a, b, true);
        } else {
            ll x;
            cin >> x;
            cout << lc.query(x, true) << '\n';
        }
    }
}
