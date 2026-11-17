// Wythoff: two piles; a move takes any amount from one pile or the same amount from both.
// Use when: "two piles, remove from one or equally from both", queen moving toward the corner.
// Usage: is_wythoff_losing(a, b) -> the player to move loses; get_kth_wythoff_pair(k).
// Handles: either pile order, empty piles, piles up to 1e18 with no floating-point drift.
// Time: O(1) per query | wythoff_pairs O(N) | generate_wythoff_moves O(a + b)
// Indexing: losing pairs (floor(k*phi), floor(k*phi) + k) indexed by k from 0
// Note: a plain int(k * phi) in double is wrong from k = 102334155 on; floor_k_phi corrects it.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// floor(k * phi), phi = (1 + sqrt 5) / 2, exact: cand <= k*phi iff (2*cand - k)^2 < 5*k^2
ll floor_k_phi(ll k) {
    static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;
    ll a = (ll)floorl((long double)k * PHI);
    auto ok = [&](ll c) { return (__int128)(2 * c - k) * (2 * c - k) < (__int128)5 * k * k; };
    while (a > 0 && !ok(a)) a--;
    while (ok(a + 1)) a++;
    return a;
}

pair<ll, ll> get_kth_wythoff_pair(ll k) {
    ll a = floor_k_phi(k);
    return {a, a + k};
}

bool is_wythoff_losing(ll a, ll b) { // true when the player to move loses
    if (a > b) swap(a, b);
    return a == floor_k_phi(b - a);
}

vector<pair<ll, ll>> wythoff_pairs(int N) { // the first N losing pairs, k = 0..N-1
    vector<pair<ll, ll>> result;
    for (int k = 0; k < N; ++k) result.push_back(get_kth_wythoff_pair(k));
    return result;
}

// Check if a move from (a,b) to (x,y) is valid
bool is_valid_wythoff_move(ll a, ll b, ll x, ll y) {
    if (x < 0 || y < 0 || x > a || y > b) return false;
    ll da = a - x, db = b - y;
    return (da == 0 && db > 0) || (db == 0 && da > 0) || (da == db && da > 0);
}

// Generate all legal moves from a position (a, b)
vector<pair<ll, ll>> generate_wythoff_moves(ll a, ll b) {
    vector<pair<ll, ll>> moves;
    for (ll i = 1; i <= a; ++i) moves.emplace_back(a - i, b);
    for (ll i = 1; i <= b; ++i) moves.emplace_back(a, b - i);
    for (ll i = 1; i <= min(a, b); ++i) moves.emplace_back(a - i, b - i);
    return moves;
}
