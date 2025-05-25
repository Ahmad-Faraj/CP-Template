#include <bits/stdc++.h>
using namespace std;

const int SL = 1e8;     // Super Losing
const int SW = SL + 1;  // Super Winning

unordered_map<int, int> memo_normal;
unordered_map<int, int> memo_misere;

// Define your state transitions for the game
vector<int> get_moves(int state) {
    // Example: remove 1, 3, or 4 tokens (like a variant of Nim)
    vector<int> moves;
    if (state >= 1) moves.push_back(state - 1);
    if (state >= 3) moves.push_back(state - 3);
    if (state >= 4) moves.push_back(state - 4);
    return moves;
}

int mex(const unordered_set<int>& s) {
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
        if (val != SW) next.insert(val);
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

int main() {
    int n;
    cin >> n;             // number of piles or states
    bool misere = false;  // toggle for misère play
    int xor_sum = 0;
    bool sl = false, sw = false;

    for (int i = 0; i < n; ++i) {
        int state;
        cin >> state;
        int g = misere ? grundy_misere(state) : grundy_normal(state);
        if (g == SL)
            sl = true;
        else if (g == SW)
            sw = true;
        else
            xor_sum ^= g;
    }

    if (sl)
        cout << "Second win*\n";
    else if (sw)
        cout << "First win*\n";
    else if (xor_sum)
        cout << "First win\n";
    else
        cout << "Second win\n";

    return 0;
}
