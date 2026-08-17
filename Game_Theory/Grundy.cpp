// Grundy: Sprague-Grundy values for impartial games, so a sum of games reduces to a single XOR.
// Use when: "who wins if both play perfectly", Nim and its variants, several independent games played at once.
// Handles: normal play (last move wins) and misere play, arbitrary move sets, memoised state graphs, and Nim itself.
// Time: O(states * moves) to fill the table, O(1) per lookup afterwards
// Indexing: 0-based states; a state with no moves has Grundy value 0 and loses under normal play
// Note: the misere rule is NOT "invert the answer" in general - it is only that simple for plain Nim. See misere_nim.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int mex(const vector<int> &values) { // smallest non-negative integer not present
    vector<char> seen(values.size() + 1, 0);
    for (int v : values)
        if (v >= 0 && v <= (int)values.size()) seen[v] = 1;
    for (int i = 0;; i++)
        if (!seen[i]) return i;
}

// Grundy values for states 0..limit, where moves_from(s) lists the states reachable from s.
// Every listed state must be strictly smaller than s, or the recursion has nothing to stand on.
template <typename F> vector<int> grundy_table(int limit, F moves_from) {
    vector<int> g(limit + 1, 0);
    for (int s = 0; s <= limit; s++) {
        vector<int> reachable;
        for (int t : moves_from(s)) reachable.push_back(g[t]);
        g[s] = mex(reachable);
    }
    return g;
}

// the Grundy value of a sum of independent games is the XOR of their values
int game_sum(const vector<int> &grundy_values) {
    int total = 0;
    for (int g : grundy_values) total ^= g;
    return total;
}

bool first_player_wins(const vector<int> &grundy_values) { return game_sum(grundy_values) != 0; }

// plain Nim: a position loses under normal play exactly when the XOR of the pile sizes is 0
bool nim_first_player_wins(const vector<ll> &piles) {
    ll x = 0;
    for (ll p : piles) x ^= p;
    return x != 0;
}

// Misere Nim: the loser takes the last object. The rule changes only when no pile exceeds 1.
bool misere_nim_first_player_wins(const vector<ll> &piles) {
    bool any_big = false;
    ll x = 0, ones = 0;
    for (ll p : piles) {
        if (p > 1) any_big = true;
        if (p == 1) ones++; // count the ONES, not the piles - empty piles are not part of the game
        x ^= p;
    }
    if (!any_big) return (ones % 2) == 0; // an even number of single-object piles wins
    return x != 0;
}

// a winning move on a Nim position, as (pile index, how many to take); {-1, 0} when the position is lost
pair<int, ll> nim_winning_move(const vector<ll> &piles) {
    ll x = 0;
    for (ll p : piles) x ^= p;
    if (x == 0) return {-1, 0};
    for (size_t i = 0; i < piles.size(); i++) {
        ll target = piles[i] ^ x;
        if (target < piles[i]) return {(int)i, piles[i] - target};
    }
    return {-1, 0};
}

// Grundy values over an explicit acyclic move graph, memoised
struct Grundy_Graph {
    vector<vector<int>> moves;
    vector<int> value;
    vector<char> done;

    Grundy_Graph(int n) : moves(n), value(n, 0), done(n, 0) {}

    void add_move(int from, int to) { moves[from].push_back(to); }

    int grundy(int state) { // the move graph must be acyclic
        if (done[state]) return value[state];
        done[state] = 1; // set before recursing, so a stray cycle terminates instead of hanging
        vector<int> reachable;
        for (int t : moves[state]) reachable.push_back(grundy(t));
        return value[state] = mex(reachable);
    }
};

// Standard problem: n Nim piles - report whether the first player wins, and a winning move if so
void solve() {
    int n;
    cin >> n;
    vector<ll> piles(n);
    for (ll &p : piles) cin >> p;
    if (!nim_first_player_wins(piles)) {
        cout << "SECOND\n";
        return;
    }
    auto [pile, take] = nim_winning_move(piles);
    cout << "FIRST " << pile + 1 << ' ' << take << '\n';
}
