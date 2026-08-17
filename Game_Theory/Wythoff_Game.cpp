// Wythoff Game: two piles, a move takes any amount from one pile or the SAME amount from both; last to move wins.
// Use when: exactly this game, or "two counters and a diagonal move" - the losing positions follow the golden ratio.
// Handles: either pile order, both piles empty, and huge pile sizes without floating-point drift.
// Time: O(1) per query with the closed form | O(n^2) to fill the table
// Indexing: pairs (a, b) of pile sizes, and losing pairs indexed by k from 0
// Note: the golden-ratio formula is computed in long double then CORRECTED exactly - never trust the raw multiply.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// the k-th losing pair is (floor(k*phi), floor(k*phi) + k); phi = (1 + sqrt 5) / 2
// A raw floor of k*phi in floating point is wrong for large k, so nudge the candidate
// and pick the value satisfying the defining property instead of trusting the multiply.
ll floor_k_phi(ll k) {
    static const long double PHI = (1.0L + sqrtl(5.0L)) / 2.0L;
    ll a = (ll)floorl((long double)k * PHI);
    // the true a is the unique value with a*a - a*k - k*k in the right window; correct by stepping
    auto ok = [&](ll cand) { // cand <= k*phi holds exactly when (2*cand - k)^2 < 5*k^2
        __int128 lhs = (__int128)(2 * cand - k) * (2 * cand - k);
        return lhs < (__int128)5 * k * k;
    };
    while (a > 0 && !ok(a)) a--;
    while (ok(a + 1)) a++;
    return a;
}

pair<ll, ll> losing_pair(ll k) { // the k-th losing position, k from 0
    ll a = floor_k_phi(k);
    return {a, a + k};
}

bool is_losing(ll a, ll b) { // true when the player to move loses
    if (a > b) swap(a, b);
    ll k = b - a;
    return a == floor_k_phi(k);
}

bool first_player_wins(ll a, ll b) { return !is_losing(a, b); }

vector<pair<ll, ll>> first_losing_pairs(int count) {
    vector<pair<ll, ll>> out;
    for (int k = 0; k < count; k++) out.push_back(losing_pair(k));
    return out;
}

// the losing positions found the slow way, for cross-checking the formula
vector<vector<char>> losing_table(int limit) {
    vector<vector<char>> lose(limit + 1, vector<char>(limit + 1, 0));
    for (int a = 0; a <= limit; a++)
        for (int b = 0; b <= limit; b++) {
            bool anyLosingMove = false;
            for (int i = 1; i <= a && !anyLosingMove; i++)
                if (lose[a - i][b]) anyLosingMove = true;
            for (int i = 1; i <= b && !anyLosingMove; i++)
                if (lose[a][b - i]) anyLosingMove = true;
            for (int i = 1; i <= min(a, b) && !anyLosingMove; i++)
                if (lose[a - i][b - i]) anyLosingMove = true;
            lose[a][b] = !anyLosingMove; // losing exactly when no move reaches a losing position
        }
    return lose;
}

// Standard problem: two pile sizes - report which player wins with perfect play
void solve() {
    ll a, b;
    cin >> a >> b;
    cout << (first_player_wins(a, b) ? "FIRST" : "SECOND") << '\n';
}
