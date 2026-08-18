// Ternary Search: the minimum or maximum of a unimodal function, by discarding a third of the range each step.
// Use when: the value falls then rises (or rises then falls) - "minimise this cost", geometry distances, tuning a parameter.
// Handles: integer and real domains, minimising or maximising, and flat stretches in the integer version.
// Time: O(log range) evaluations for integers, O(iterations) for reals
// Indexing: the integer search is inclusive [lo, hi]
// Note: the function MUST be unimodal. Given two separate dips this returns one of them silently - it cannot detect that.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// the x in [lo, hi] minimising f
template <typename F> ll ternary_min(ll lo, ll hi, F f) {
    while (hi - lo > 2) {
        ll m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) <= f(m2)) hi = m2 - 1;
        else lo = m1 + 1;
    }
    ll best = lo;
    for (ll x = lo; x <= hi; x++)
        if (f(x) < f(best)) best = x;
    return best;
}

template <typename F> ll ternary_max(ll lo, ll hi, F f) {
    return ternary_min(lo, hi, [&](ll x) { return -f(x); });
}

template <typename F> double ternary_min_real(double lo, double hi, F f, int iterations = 200) {
    for (int i = 0; i < iterations; i++) {
        double m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        if (f(m1) <= f(m2)) hi = m2;
        else lo = m1;
    }
    return (lo + hi) / 2;
}

template <typename F> double ternary_max_real(double lo, double hi, F f, int iterations = 200) {
    return ternary_min_real(lo, hi, [&](double x) { return -f(x); }, iterations);
}

// Standard problem: minimise a*x*x + b*x + c over the integers in [lo, hi]
void solve() {
    ll a, b, c, lo, hi;
    cin >> a >> b >> c >> lo >> hi;
    ll x = ternary_min(lo, hi, [&](ll t) { return a * t * t + b * t + c; });
    cout << x << ' ' << a * x * x + b * x + c << '\n';
}
