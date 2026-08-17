// Games On Graph: win, lose or draw for every position of a game whose move graph may contain cycles.
// Use when: positions can repeat, so Grundy numbers do not apply - "can the token be forced home", pursuit games.
// Handles: cycles (which become draws), several terminal positions, unreachable positions, and both players to move.
// Time: O(n + m)
// Indexing: 0-based positions; add_move(u, v) means the player to move at u may move the token to v
// Note: a position that is neither winning nor losing is a DRAW, i.e. play never ends. Grundy.cpp cannot say that.

#include <bits/stdc++.h>
using namespace std;

struct Games_On_Graph {
    int n;
    vector<vector<int>> reverse_moves; // reverse_moves[v] lists the u that can move to v
    vector<int> out_degree;
    vector<char> winning, losing;

    Games_On_Graph(int n) : n(n), reverse_moves(n), out_degree(n, 0), winning(n, 0), losing(n, 0) {}

    void add_move(int u, int v) {
        reverse_moves[v].push_back(u);
        out_degree[u]++;
    }

    // classify every position by working backwards from the terminal ones
    void solve_all() {
        vector<int> queue_;
        for (int v = 0; v < n; v++)
            if (out_degree[v] == 0) { // no moves available, so the player to move has lost
                losing[v] = 1;
                queue_.push_back(v);
            }
        for (int i = 0; i < (int)queue_.size(); i++) {
            int v = queue_[i];
            for (int u : reverse_moves[v]) {
                if (winning[u] || losing[u]) continue;
                if (losing[v]) { // a move into a losing position makes u winning
                    winning[u] = 1;
                    queue_.push_back(u);
                } else if (--out_degree[u] == 0) { // every move leads to a win for the opponent
                    losing[u] = 1;
                    queue_.push_back(u);
                }
            }
        }
    }

    bool is_win(int v) const { return winning[v]; }
    bool is_lose(int v) const { return losing[v]; }
    bool is_draw(int v) const { return !winning[v] && !losing[v]; } // stuck in a cycle forever

    string verdict(int v) const { return winning[v] ? "WIN" : (losing[v] ? "LOSE" : "DRAW"); }
};

// Standard problem: a move graph and a starting position - report WIN, LOSE or DRAW for the player to move
void solve() {
    int n, m, start;
    cin >> n >> m >> start;
    Games_On_Graph g(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_move(u, v);
    }
    g.solve_all();
    cout << g.verdict(start) << '\n';
}
