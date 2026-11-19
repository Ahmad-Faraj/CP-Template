// Impartial Games: Sprague-Grundy values, Nim (normal and misere) and compound-game win tests.
// Use when: "both play optimally, who wins", Nim, subtraction games, several games at once.
// Usage: compute_grundy(h, moves, dp); normal_disjunctive_long(piles); nim_winning_move(piles)
//        Grundy_Graph G(n); G.add_move(u, v); G.grundy(u);
// Handles: normal play (last move wins), misere Nim, sums where you move in one game (disjunctive)
//          or in any non-empty set of games (selective).
// Not: sums where finishing ANY one game ends everything (Super_Win_Lose.cpp), moving in EVERY game
//      (conjunctive: Remoteness_Suspense.cpp), cyclic move graphs (Games_On_Graph.cpp).
// Time: compute_grundy / Grundy_Graph O(states * moves) total; the Nim tests O(number of piles)
// Indexing: states 0..; a state with no moves has Grundy 0 (lost for the mover in normal play)
// Note: misere is NOT "invert the answer" except for plain Nim (misere_disjunctive_long).
//       normal_selective_variant is only proven for Nim piles; on Grundy values of other games
//       it is unproven (matched brute force on small cases).
//       compute_grundy / Grundy_Graph recurse: depth up to the longest chain of moves.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

int mex(const vector<int> &values) { // smallest non-negative integer not in values
    vector<char> seen(values.size() + 1, 0);
    for (int v : values)
        if (v >= 0 && v <= (int)values.size()) seen[v] = 1;
    int m = 0;
    while (seen[m]) m++;
    return m;
}

// subtraction game: from h you may remove any mv in moves (mv >= 1, h >= mv). dp sized > h, all -1.
int compute_grundy(int h, const vector<int> &moves, vector<int> &dp) {
    if (dp[h] != -1) return dp[h];
    vector<int> S;
    for (int mv : moves)
        if (h >= mv) S.push_back(compute_grundy(h - mv, moves, dp));
    return dp[h] = mex(S);
}

// Grundy values on an explicit acyclic move graph, memoised
struct Grundy_Graph {
    vector<vector<int>> moves;
    vector<int> value;

    Grundy_Graph(int n) : moves(n), value(n, -1) {}

    void add_move(int from, int to) { moves[from].push_back(to); }

    int grundy(int s) {
        if (value[s] != -1) return value[s];
        vector<int> reach;
        for (int t : moves[s]) reach.push_back(grundy(t));
        return value[s] = mex(reach);
    }
};

ll nim_value(const vector<ll> &piles) { // XOR of the Grundy values = Grundy value of the sum
    ll x = 0;
    for (ll p : piles) x ^= p;
    return x;
}

// Disjunctive, normal play: move in exactly one game. Pass pile sizes or Grundy values.
bool normal_disjunctive_long(const vector<ll> &piles) { return nim_value(piles) != 0; }

// Misere Nim (taking the last object loses): XOR rule unless every pile is <= 1.
bool misere_disjunctive_long(const vector<ll> &piles) {
    bool all_small = true;
    ll ones = 0;
    for (ll p : piles) {
        if (p > 1) all_small = false;
        ones += p == 1;
    }
    if (all_small) return ones % 2 == 0;
    return nim_value(piles) != 0;
}

// a winning Nim move as (pile index, how many to take); {-1, 0} when the position is lost
pair<int, ll> nim_winning_move(const vector<ll> &piles) {
    ll x = nim_value(piles);
    if (x == 0) return {-1, 0};
    for (int i = 0; i < (int)piles.size(); i++)
        if ((piles[i] ^ x) < piles[i]) return {i, piles[i] - (piles[i] ^ x)};
    return {-1, 0};
}

// Selective, normal play: move in any non-empty set of games. Lost iff every Grundy value is 0.
bool normal_selective_long(const vector<int> &grundies) {
    for (int g : grundies)
        if (g != 0) return true;
    return false;
}

// Nim where you take from at least one but not all piles: lost iff all piles are equal.
bool normal_selective_variant(const vector<ll> &piles) {
    for (ll p : piles)
        if (p != piles[0]) return true;
    return false;
}
