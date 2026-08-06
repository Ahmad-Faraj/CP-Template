# 2. Compound Games (Impartial Game Rules)

When playing multiple sub-games simultaneously, the rules of combination dictate how to evaluate the final state.

```cpp
#include "../../core.h"

// 1. DISJUNCTIVE COMPOUND
// Game Rules: You must make a valid move in EXACTLY ONE of the available sub-games.
// O ( N )
bool normal_disjunctive_long(const vector<int>& piles) { return nim_value(piles) != 0; }
bool normal_disjunctive_short(const vector<int>& grundies, int SW, int SL) {
    int x = 0;
    for (int g : grundies) x ^= (g == SW || g == SL ? 0 : g);
    return x != 0;
}
bool misere_disjunctive_long(const vector<int>& piles) {
    bool all_small = true;
    int total = 0, x = 0;
    for (int p : piles) {
        if (p > 1) all_small = false;
        total += p;
        x ^= p;
    }
    return all_small ? (total % 2 == 0) : (x != 0);
}

// 2. SELECTIVE COMPOUND
// Game Rules: You may make a valid move in ANY NUMBER of sub-games (at least one, up to all of them).
// O ( N )
bool normal_selective_long(const vector<int>& grundies) {
    for (int g : grundies) if (g != 0) return true;
    return false;
}
bool normal_selective_variant(const vector<int>& grundies) {
    if (grundies.empty()) return false;
    for (int g : grundies) if (g != grundies[0]) return true;
    return false;
}
bool misere_selective_short(const vector<int>& grundies) {
    int zeros = count(grundies.begin(), grundies.end(), 0);
    return zeros != (int)grundies.size();
}

// 3. CONJUNCTIVE COMPOUND
// Game Rules: You must make a valid move in ALL sub-games simultaneously. If you cannot move in all of them, you lose.
// Requires computing Remoteness and Suspense for the states.
// O ( N )
bool normal_conjunctive_long(const vector<int>& states) {
    int best = 0;
    for (int s : states) best = max(best, compute_suspense(s));
    return best > 0;
}
bool normal_conjunctive_short(const vector<int>& states) {
    if (states.empty()) return false;
    int worst = inf;
    for (int s : states) worst = min(worst, compute_remoteness(s));
    return worst > 0;
}
bool misere_conjunctive_short(const vector<int>& states) { return normal_conjunctive_short(states); }
bool misere_conjunctive_long(const vector<int>& states) { return normal_conjunctive_long(states); }
```
