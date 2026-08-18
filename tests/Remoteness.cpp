// Brute-force test for:
//   Game_Theory/Remoteness.cpp
// Compares the template against an INDEPENDENT reference over randomised inputs.
// Run with: py tools/test.py

#include <bits/stdc++.h>
using namespace std;

namespace RM {
#include "../Game_Theory/Remoteness.cpp"
}

int main() {
    mt19937_64 gen(13579);
    int winFail = 0, lenFail = 0, moveFail = 0, lineFail = 0, termFail = 0;
    int checked = 0;

    for (int it = 0; it < 20000; it++) {
        int n = 1 + (int)(gen() % 9);
        // random DAG: edges only go from lower to higher index, so it is acyclic by construction
        RM::Remoteness g(n);
        vector<vector<int>> adj(n);
        for (int u = 0; u < n; u++)
            for (int v = u + 1; v < n; v++)
                if (gen() % 3 == 0) {
                    g.add_move(u, v);
                    adj[u].push_back(v);
                }
        g.compute_all();
        checked++;

        // reference: plain recursion over the DAG, computed independently
        // winner = exists a move to a losing position
        // length  = winner hurries (min over winning moves), loser stalls (max over all moves)
        vector<int> win(n, -1), len(n, -1);
        function<void(int)> rec = [&](int u) {
            if (win[u] != -1) return;
            if (adj[u].empty()) { win[u] = 0; len[u] = 0; return; }
            int shortest = INT_MAX, longest = -1;
            for (int v : adj[u]) {
                rec(v);
                if (!win[v]) shortest = min(shortest, len[v] + 1);
                longest = max(longest, len[v] + 1);
            }
            if (shortest != INT_MAX) win[u] = 1, len[u] = shortest;
            else win[u] = 0, len[u] = longest;
        };
        for (int u = 0; u < n; u++) rec(u);

        for (int u = 0; u < n; u++) {
            if ((int)g.is_win(u) != win[u]) winFail++;
            if (g.game_length(u) != len[u]) lenFail++;
            if (adj[u].empty()) {
                if (g.game_length(u) != 0) termFail++;
                if (g.optimal_move(u) != RM::Remoteness::UNSET) termFail++;
                continue;
            }
            // the reported move must be legal, and must realise the claimed length
            int mv = g.optimal_move(u);
            bool legal = false;
            for (int v : adj[u])
                if (v == mv) legal = true;
            if (!legal) { moveFail++; continue; }
            if (g.game_length(mv) + 1 != g.game_length(u)) moveFail++;
            // a winner's move must hand the opponent a loss
            if (win[u] && g.is_win(mv)) moveFail++;
        }

        // the optimal line must be a real walk whose length matches, and must end at a terminal
        for (int u = 0; u < n; u++) {
            vector<int> line = g.optimal_line(u);
            if (line.empty() || line.front() != u) { lineFail++; continue; }
            if ((int)line.size() - 1 != g.game_length(u)) lineFail++;
            for (size_t i = 0; i + 1 < line.size(); i++) {
                bool ok = false;
                for (int v : adj[line[i]])
                    if (v == line[i + 1]) ok = true;
                if (!ok) lineFail++;
            }
            if (!adj[line.back()].empty()) lineFail++;
        }
    }

    // ---------- a hand-checked case: two routes to a win, one shorter ----------
    {
        // 0 -> 1 -> 2 (terminal)   and   0 -> 3 (terminal)
        // 3 is terminal so it is a LOSS for whoever moves there: 0 wins in 1 move.
        RM::Remoteness g(4);
        g.add_move(0, 1);
        g.add_move(1, 2);
        g.add_move(0, 3);
        g.compute_all();
        if (!g.is_win(0) || g.game_length(0) != 1 || g.optimal_move(0) != 3) lenFail++;
        if (g.is_win(2) || g.game_length(2) != 0) lenFail++;
        // 1 can move to terminal 2, handing the opponent a lost position, so 1 WINS in 1
        if (!g.is_win(1) || g.game_length(1) != 1) lenFail++;
    }
    // a loser really does pick the LONGEST line
    {
        // 0 -> 1 -> 2 -> 3(term), and 0 -> 4(term); make 0 losing by giving it only winning moves
        RM::Remoteness g(5);
        g.add_move(0, 1); // 1 will be winning
        g.add_move(1, 2);
        g.add_move(2, 3);
        g.compute_all();
        // 3 terminal: lose, len 0. 2: can move to 3 (a loss) so 2 wins, len 1.
        // 1: only move is to 2 which wins, so 1 loses and stalls: len 2.
        // 0: only move is to 1 which loses, so 0 wins, len 3.
        if (g.is_win(3) || g.game_length(3) != 0) lenFail++;
        if (!g.is_win(2) || g.game_length(2) != 1) lenFail++;
        if (g.is_win(1) || g.game_length(1) != 2) lenFail++;
        if (!g.is_win(0) || g.game_length(0) != 3) lenFail++;
    }

    cout << "Remoteness: " << checked << " random DAGs\n";
    cout << "  winner vs independent recursion : " << winFail << " failures\n";
    cout << "  game length (hurry / stall)     : " << lenFail << " failures\n";
    cout << "  optimal_move legal and realises : " << moveFail << " failures\n";
    cout << "  optimal_line is a real walk     : " << lineFail << " failures\n";
    cout << "  terminal positions              : " << termFail << " failures\n";
}
