// Monotonic CHT: minimum of a set of lines y = mx + b at a query x, in O(1) amortized.
// Use when: same shape as CHT, but slopes already arrive sorted and queries move one way — drops the log.
// Handles: decreasing slopes only; O(1) at non-decreasing x, O(log n) at arbitrary x. No deletion, not persistent.
// Time: add O(1) amortized | query O(1) amortized | query_bs O(log n)
// Indexing: 0-based (lines kept in insertion order)
// Note: add slopes decreasing; query() needs non-decreasing x, else query_bs(). For max add {-m,-b} and negate.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
#define sz(x) (int)(x).size()

struct CHT {
    vector<ll> m, b;
    int ptr = 0;

    bool bad(int l1, int l2, int l3) {
        return 1.0 * (b[l3] - b[l1]) * (m[l1] - m[l2]) <= 1.0 * (b[l2] - b[l1]) * (m[l1] - m[l3]);
    }

    ll f(int i, ll x) { return m[i] * x + b[i]; }

    void add(ll _m, ll _b) { // insert y = _m x + _b, slopes decreasing
        m.push_back(_m);
        b.push_back(_b);
        int s = sz(m);
        while (s >= 3 && bad(s - 3, s - 2, s - 1)) {
            s--;
            m.erase(m.end() - 2);
            b.erase(b.end() - 2);
        }
    }

    ll query(ll x) { // minimum at x, x non-decreasing across calls
        if (ptr >= sz(m)) ptr = sz(m) - 1;
        while (ptr + 1 < sz(m) && f(ptr + 1, x) < f(ptr, x)) ptr++;
        return f(ptr, x);
    }

    ll bs(int l, int r, ll x) {
        int mid = (l + r) / 2;
        if (mid + 1 < sz(m) && f(mid + 1, x) < f(mid, x)) return bs(mid + 1, r, x);
        if (mid - 1 >= 0 && f(mid - 1, x) < f(mid, x)) return bs(l, mid - 1, x);
        return f(mid, x);
    }

    ll query_bs(ll x) { return bs(0, sz(m) - 1, x); } // minimum at any x
};

// Standard problem: dp[i] = min over j < i of dp[j] + b[j] * a[i], with b decreasing and a increasing (CF 319C)
void solve() {
    int n;
    cin >> n;
    vector<ll> a(n), b(n);
    for (int i = 0; i < n; i++) cin >> a[i];
    for (int i = 0; i < n; i++) cin >> b[i];
    CHT cht;
    cht.add(b[0], 0);
    ll ans = 0;
    for (int i = 1; i < n; i++) {
        ans = cht.query(a[i]);
        cht.add(b[i], ans);
    }
    cout << ans << '\n';
}
