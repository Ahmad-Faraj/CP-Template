// Super Win Lose: Sprague-Grundy for a sum where finishing ANY one component ends the whole game.
// Use when: several piles/boards, a move changes one of them, and "whoever empties a pile wins"
//   (normal) or "whoever empties a pile loses" (misere) - plain Grundy XOR is wrong there.
// Usage: Super_Win_Lose g(get_moves); g.first_player_wins(states, misere); g.grundy_normal(s).
// Time: O(sum of moves over reachable states), memoised
// Indexing: a state is any int; get_moves(s) lists the states reachable from s (empty = terminal)
// Note: grundy_* returns SL (mover has already lost), SW (mover wins at once) or a Grundy value.
//       Normal: terminal = SL, a move to SL = SW. Misere: terminal = SW, moves into SW are suicide.
//       The moves must be acyclic. Recursive: depth up to the longest chain of states.

#include <bits/stdc++.h>
using namespace std;

struct Super_Win_Lose {
    static constexpr int SL = 100000000; // Super Losing
    static constexpr int SW = SL + 1;    // Super Winning

    function<vector<int>(int)> get_moves;
    unordered_map<int, int> memo_normal, memo_misere;

    Super_Win_Lose(function<vector<int>(int)> f) : get_moves(f) {}

    static int mex(const unordered_set<int> &s) {
        for (int i = 0;; ++i)
            if (!s.count(i)) return i;
    }

    int grundy_normal(int state) {
        if (memo_normal.count(state)) return memo_normal[state];
        auto moves = get_moves(state);
        if (moves.empty()) return memo_normal[state] = SL;
        unordered_set<int> next;
        for (int to : moves) {
            int val = grundy_normal(to);
            if (val == SL) return memo_normal[state] = SW;
            if (val != SW) next.insert(val); // moving to SW hands the opponent the win
        }
        return memo_normal[state] = mex(next);
    }

    int grundy_misere(int state) {
        if (memo_misere.count(state)) return memo_misere[state];
        auto moves = get_moves(state);
        if (moves.empty()) return memo_misere[state] = SW;
        unordered_set<int> next;
        for (int to : moves) {
            int val = grundy_misere(to);
            if (val != SW) next.insert(val);
        }
        return memo_misere[state] = mex(next);
    }

    bool first_player_wins(const vector<int> &states, bool misere = false) {
        bool sl = false, sw = false;
        int xor_sum = 0;
        for (int s : states) {
            int g = misere ? grundy_misere(s) : grundy_normal(s);
            if (g == SL) sl = true;
            else if (g == SW) sw = true;
            else xor_sum ^= g;
        }
        if (sl) return false; // a component is already finished: the previous player won
        if (sw) return true;
        return xor_sum != 0;
    }
};
