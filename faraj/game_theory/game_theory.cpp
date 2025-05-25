#include <bits/stdc++.h>
using namespace std;

// ------------ Global ------------

int nim_value(const vector<int>& piles) {
    int x = 0;
    for (int p : piles) x ^= p;
    return x;
}

int mex(const unordered_set<int>& S) {
    int m = 0;
    while (S.count(m)) ++m;
    return m;
}

int compute_grundy(int h, const vector<int>& moves, vector<int>& dp) {
    if (dp[h] != -1) return dp[h];
    unordered_set<int> S;
    for (int mv : moves) {
        if (h >= mv) S.insert(compute_grundy(h - mv, moves, dp));
    }
    return dp[h] = mex(S);
}

int compute_remoteness(int /*state*/) {
    // minimal moves to win from this state
    return 1;
}
int compute_suspense(int /*state*/) {
    // maximal moves until opponent wins from this state
    return 1;
}

// ------------ 1. DISJUNCTIVE COMPOUND ------------

// 1.1 Long rule, Normal play
bool normal_disjunctive_long(const vector<int>& piles) { return nim_value(piles) != 0; }

// 1.2 Short rule (diminished), Normal play
bool normal_disjunctive_short(const vector<int>& grundies, int SW, int SL) {
    int x = 0;
    for (int g : grundies) {
        int w = (g == SW || g == SL ? 0 : g);
        x ^= w;
    }
    return x != 0;
}

// 1.3 Short rule (diminished), Misère play
bool misere_disjunctive_short(const vector<int>& grundies, int SW, int SL) {
    // same as normal diminished
    return normal_disjunctive_short(grundies, SW, SL);
}

// 1.4 Long rule, Misère play (misère Nim)
bool misere_disjunctive_long(const vector<int>& piles) {
    bool all_small = true;
    int total = 0, x = 0;
    for (int p : piles) {
        if (p > 1) all_small = false;
        total += p;
        x ^= p;
    }
    if (all_small) return (total % 2 == 0);
    return x != 0;
}

// ------------ 2. SELECTIVE COMPOUND ------------

// 2.1 Long rule, Normal play
bool normal_selective_long(const vector<int>& grundies) {
    for (int g : grundies)
        if (g != 0) return true;
    return false;
}

// specail: Selective variant: move in ≥1 but not all; losing iff all Grundies equal
bool normal_selective_variant(const vector<int>& grundies) {
    if (grundies.empty()) return false;
    int first = grundies[0];
    for (int g : grundies)
        if (g != first) return true;
    return false;
}

// 2.2 Short rule, Normal play (same as long)
bool normal_selective_short(const vector<int>& grundies) { return normal_selective_long(grundies); }

// 2.3 Short rule, Misère play
bool misere_selective_short(const vector<int>& grundies) {
    int zeros = count(grundies.begin(), grundies.end(), 0);
    if (zeros == (int)grundies.size()) return false;
    if (zeros == 1) return true;
    return true;
}

// 2.4 Long rule, Misère play (same as short)
bool misere_selective_long(const vector<int>& grundies) { return misere_selective_short(grundies); }

// ------------ 3. CONJUNCTIVE COMPOUND ------------

// 3.1 Long rule, Normal play (maximin)
bool normal_conjunctive_long(const vector<int>& states) {
    int best = 0;
    for (int s : states) best = max(best, compute_suspense(s));
    return best > 0;
}

// 3.2 Short rule, Normal play (minimax)
bool normal_conjunctive_short(const vector<int>& states) {
    if (states.empty()) return false;
    int worst = INT_MAX;
    for (int s : states) worst = min(worst, compute_remoteness(s));
    return worst > 0;
}

// 3.3 Short rule, Misère play (same as normal)
bool misere_conjunctive_short(const vector<int>& states) { return normal_conjunctive_short(states); }

// 3.4 Long rule, Misère play (same as normal)
bool misere_conjunctive_long(const vector<int>& states) { return normal_conjunctive_long(states); }