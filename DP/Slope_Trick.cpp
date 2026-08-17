// Slope Trick: maintains a convex piecewise-linear f(x) under adds and shifts, always reporting min f.
// Use when: "make the array non-decreasing with minimum total |change|", costs that are sums of |x - a|.
// Handles: += a, += max(0,x-a), += max(0,a-x), += |x-a|, prefix/suffix min, shift, window slide. Convex f only.
// Time: O(log n) per operation
// Indexing: none; f is a function of integer x
// Note: get_min() returns min f, not the x attaining it. Read top_l() / top_r() for that.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct SlopeTrick {
    static constexpr ll INF = 1e18;
    ll min_f = 0, add_l = 0, add_r = 0;
    priority_queue<ll> L;                          // breakpoints of the decreasing part, max-heap
    priority_queue<ll, vector<ll>, greater<ll>> R; // breakpoints of the increasing part, min-heap

    ll top_l() { return L.empty() ? -INF : L.top() + add_l; } // right end of the argmin plateau
    ll top_r() { return R.empty() ? INF : R.top() + add_r; }  // left end of the argmin plateau
    void push_l(ll x) { L.push(x - add_l); }
    void push_r(ll x) { R.push(x - add_r); }

    ll get_min() { return min_f; } // min over x of f(x)

    void add_all(ll a) { min_f += a; } // f(x) += a

    void add_x_minus_a(ll a) { // f(x) += max(0, x - a)
        min_f += max(0LL, top_l() - a);
        push_l(a);
        ll t = top_l();
        L.pop();
        push_r(t);
    }

    void add_a_minus_x(ll a) { // f(x) += max(0, a - x)
        min_f += max(0LL, a - top_r());
        push_r(a);
        ll t = top_r();
        R.pop();
        push_l(t);
    }

    void add_abs(ll a) { // f(x) += |x - a|
        add_x_minus_a(a);
        add_a_minus_x(a);
    }

    void min_left() { R = {}; }  // f(x) = min over y <= x of f(y)
    void min_right() { L = {}; } // f(x) = min over y >= x of f(y)

    void shift(ll a) { add_l += a, add_r += a; }            // f(x) = f(x - a)
    void slide(ll a, ll b) { add_l += a, add_r += b; }      // f(x) = min over y in [x - b, x - a] of f(y)
};

// Standard problem: cheapest way to make a non-decreasing, cost = sum of |a[i] - b[i]| (CSES 2132)
void solve() {
    int n;
    cin >> n;
    SlopeTrick st;
    for (int i = 0; i < n; i++) {
        ll a;
        cin >> a;
        st.min_left();
        st.add_abs(a);
    }
    cout << st.get_min() << '\n';
}
