// Josephus: n people in a circle, every k-th is removed; who dies m-th, and who survives.
// Use when: "every k-th person leaves the circle", elimination games, cyclic removal puzzles.
// Handles: any k including k = 1, the m-th victim, the final survivor, and the whole removal order.
// Time: kth_killed O(k log n) | survivor O(n) | removal_order O(n log n)
// Indexing: people are numbered 1..n, counting starts at person 1, and m runs 1..n
// Note: kth_killed is the fast one and is what you want for huge n. Build the full order only when n is small.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// the m-th person to be removed, counting every k-th
ll kth_killed(ll n, ll k, ll m) {
    m = n - m;
    if (k <= 1) return n - m;
    ll i = m;
    while (i < n) {
        ll r = (i - m + k - 2) / (k - 1);
        if (i + r > n) r = n - i;
        else if (!r) r = 1;
        i += r;
        m = (m + r * k) % i;
    }
    return m + 1;
}

ll survivor(ll n, ll k) { // the one left at the end, grown one person at a time
    ll pos = 0;
    for (ll people = 2; people <= n; people++) pos = (pos + k) % people;
    return pos + 1;
}

ll survivor_k2(ll n) { // k = 2 has a closed form: strip the leading bit and shift it in
    ll highest = 1;
    while (highest * 2 <= n) highest *= 2;
    return 2 * (n - highest) + 1;
}

vector<ll> removal_order(ll n, ll k) { // every victim in order; only for n small enough to list
    vector<ll> alive(n), out;
    iota(alive.begin(), alive.end(), 1);
    ll idx = 0;
    while (!alive.empty()) {
        idx = (idx + k - 1) % (ll)alive.size();
        out.push_back(alive[idx]);
        alive.erase(alive.begin() + idx);
    }
    return out;
}

// Standard problem: n and k - print the removal order, then the survivor
void solve() {
    ll n, k;
    cin >> n >> k;
    vector<ll> order = removal_order(n, k);
    for (size_t i = 0; i < order.size(); i++) cout << order[i] << " \n"[i + 1 == order.size()];
    cout << survivor(n, k) << '\n';
}
