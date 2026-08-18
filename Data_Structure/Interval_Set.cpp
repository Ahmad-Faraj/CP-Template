// Interval Set: keeps a sequence as runs of equal values, so assigning a whole range is one operation.
// Use when: "set a[l..r] all to v" happens often - the runs collapse, and everything else is a walk over few pieces.
// Handles: range assign, point read, walking the runs of a range, range sum, and counting a value over a range.
// Time: amortized O(log n) per assign when ranges are varied; O(n) worst case if they are adversarial
// Indexing: 1-based inclusive [l, r]; the structure spans [1, n] fixed at construction
// Note: the speed comes from ASSIGN merging runs. Without assigns nothing collapses and this degrades to a slow list.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Interval_Set {
    ll n;
    map<ll, ll> runs; // key = the run's first index, value = the value held across that run

    Interval_Set(ll n, ll initial = 0) : n(n) {
        runs[1] = initial;
        runs[n + 1] = initial; // sentinel, so every lookup has a successor
    }

    // make sure a run starts exactly at pos, splitting whatever covers it
    map<ll, ll>::iterator split(ll pos) {
        if (pos > n + 1) return runs.end(); // n + 1 is the sentinel and is a legal split point
        auto it = runs.upper_bound(pos);
        --it;
        if (it->first == pos) return it;
        ll value = it->second;
        return runs.emplace(pos, value).first;
    }

    void assign(ll l, ll r, ll value) { // a[l..r] = value
        if (l > r) return;
        auto right = split(r + 1); // split the far end FIRST; splitting l can invalidate nothing after
        auto left = split(l);
        runs.erase(left, right);
        runs[l] = value;
    }

    ll get(ll pos) { // a[pos]
        auto it = runs.upper_bound(pos);
        --it;
        return it->second;
    }

    // visit every run overlapping [l, r] as (from, to, value), clipped to the range
    template <typename F> void for_each_run(ll l, ll r, F visit) {
        if (l > r) return;
        auto it = runs.upper_bound(l);
        --it;
        while (it->first <= r && it->first <= n) {
            auto next = std::next(it);
            ll from = max(it->first, l), to = min(next->first - 1, r);
            if (from <= to) visit(from, to, it->second);
            if (next->first > r) break;
            it = next;
        }
    }

    ll sum(ll l, ll r) { // sum over a[l..r]
        ll total = 0;
        for_each_run(l, r, [&](ll from, ll to, ll v) { total += (to - from + 1) * v; });
        return total;
    }

    ll count(ll l, ll r, ll value) { // how many positions in [l, r] hold this value
        ll total = 0;
        for_each_run(l, r, [&](ll from, ll to, ll v) { if (v == value) total += to - from + 1; });
        return total;
    }

    ll run_count() { return (ll)runs.size() - 1; } // how many runs the sequence has collapsed to
};

// Standard problem: Codeforces 915E - n days all available, each query marks [l, r]
// unavailable (k = 1) or available (k = 2); report the available count after each.
void solve() {
    ll n;
    int q;
    cin >> n >> q;
    Interval_Set days(n, 1); // 1 = available
    while (q--) {
        ll l, r;
        int k;
        cin >> l >> r >> k;
        days.assign(l, r, k == 1 ? 0 : 1);
        cout << days.sum(1, n) << '\n';
    }
}
