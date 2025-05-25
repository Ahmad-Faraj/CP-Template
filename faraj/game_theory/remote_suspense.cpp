#include <bits/stdc++.h>
using namespace std;

map<int, vector<int>> graph;

map<int, int> rememo, sememo;

// Determine if a state is winning (exists a move to a losing state)
map<int, bool> winmemo;
bool is_winning(int u) {
    if (winmemo.count(u)) return winmemo[u];
    if (graph[u].empty()) return winmemo[u] = false;  // terminal is losing
    for (int v : graph[u]) {
        if (!is_winning(v)) return winmemo[u] = true;
    }
    return winmemo[u] = false;
}

// Remoteness: minimal moves to force a win under best play (short rule)
int remoteness(int u) {
    if (rememo.count(u)) return rememo[u];
    if (graph[u].empty()) return rememo[u] = 0;
    if (!is_winning(u)) return rememo[u] = 0;
    int best = INT_MAX;
    // only consider winning moves (to losing positions)
    for (int v : graph[u]) {
        if (!is_winning(v)) {
            best = min(best, remoteness(v));
        }
    }
    return rememo[u] = 1 + best;
}

// Suspense: maximal moves until opponent forced win (long rule)
int suspense(int u) {
    if (sememo.count(u)) return sememo[u];
    if (graph[u].empty()) return sememo[u] = 0;
    if (!is_winning(u)) {
        // losing position: opponent will force your win
        int worst = 0;
        for (int v : graph[u]) {
            if (is_winning(v)) {
                worst = max(worst, suspense(v));
            }
        }
        return sememo[u] = 1 + worst;
    } else {
        // winning position: choose opponent's best delay
        int worst = 0;
        for (int v : graph[u]) {
            if (!is_winning(v)) {
                worst = max(worst, suspense(v));
            }
        }
        return sememo[u] = 1 + worst;
    }
}

int main() {
    graph[0] = {1, 2};
    graph[1] = {3};
    graph[2] = {3};
    graph[3] = {};  // terminal

    // Precompute win/lose
    for (auto& kv : graph) is_winning(kv.first);

    // Compute remoteness and suspense for each state
    for (auto& kv : graph) {
        int u = kv.first;
        cout << "State " << u << ": winning=" << is_winning(u) << ", remoteness=" << remoteness(u)
             << ", suspense=" << suspense(u) << "\n";
    }
    return 0;
}
