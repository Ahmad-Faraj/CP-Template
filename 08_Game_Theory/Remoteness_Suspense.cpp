// Remoteness Suspense: on an acyclic move graph, who wins and how many moves the game lasts.
// Use when: "win as fast as possible / lose as slowly as possible" (remoteness), the reverse
//   (suspense), or sums where you move in EVERY component: game ends when ANY component ends ->
//   mover wins iff min remoteness is odd; ends when ALL end -> mover wins iff max suspense is odd.
// Usage: Remoteness_Suspense g(n); g.add_move(u, v); g.remoteness(u) (also suspense, is_winning)
// Time: O(n + m) total, memoised
// Indexing: 0-based positions; add_move(u, v): the player to move at u may move to v
// Note: graph must be a DAG. A terminal position (no moves) is lost for the mover, length 0.
//       remoteness: winner picks the shortest win, loser the longest loss; suspense: the opposite.
//       Recursive, depth up to the longest path: big stack for long chains.

#include <bits/stdc++.h>
using namespace std;

struct Remoteness_Suspense {
    int n;
    vector<vector<int>> g;
    vector<int> win, rem, sus;

    Remoteness_Suspense(int n) : n(n), g(n), win(n, -1), rem(n, -1), sus(n, -1) {}

    void add_move(int u, int v) { g[u].push_back(v); }

    bool is_winning(int u) {
        if (win[u] != -1) return win[u];
        for (int v : g[u])
            if (!is_winning(v)) return (win[u] = 1);
        return (win[u] = 0);
    }

    int remoteness(int u) {
        if (rem[u] != -1) return rem[u];
        if (g[u].empty()) return rem[u] = 0;
        if (is_winning(u)) {
            int best = INT_MAX;
            for (int v : g[u])
                if (!is_winning(v)) best = min(best, remoteness(v));
            return rem[u] = 1 + best;
        }
        int worst = 0;
        for (int v : g[u]) worst = max(worst, remoteness(v));
        return rem[u] = 1 + worst;
    }

    int suspense(int u) {
        if (sus[u] != -1) return sus[u];
        if (g[u].empty()) return sus[u] = 0;
        if (is_winning(u)) {
            int worst = 0;
            for (int v : g[u])
                if (!is_winning(v)) worst = max(worst, suspense(v));
            return sus[u] = 1 + worst;
        }
        int best = INT_MAX;
        for (int v : g[u]) best = min(best, suspense(v));
        return sus[u] = 1 + best;
    }
};
