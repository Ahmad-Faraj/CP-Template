// Remoteness: who wins AND how many moves the game lasts, when the winner hurries and the loser stalls.
// Use when: "win in the fewest moves", "survive as long as possible", or any game problem with a move-count tiebreak.
// Handles: several terminal positions, unreachable positions, and recovering the optimal move at every step.
// Time: O(n + m)
// Indexing: 0-based positions; add_move(u, v) means the player to move at u may move the token to v
// Note: the move graph must be ACYCLIC - with a cycle no length exists at all. Use Games_On_Graph.cpp for that case.

#include <bits/stdc++.h>
using namespace std;

struct Remoteness {
    static const int UNSET = -1;

    int n;
    vector<vector<int>> moves;
    vector<char> winning, done;
    vector<int> length, best_move; // length under optimal play; best_move is the move to make

    Remoteness(int n) : n(n), moves(n), winning(n, 0), done(n, 0), length(n, 0), best_move(n, UNSET) {}

    void add_move(int u, int v) { moves[u].push_back(v); }

    // One consistent rule for both sides: the player who is winning picks the SHORTEST win,
    // the player who is losing picks the LONGEST delay. That is what a tiebreak on moves means.
    void compute(int u) {
        if (done[u]) return;
        done[u] = 1;
        if (moves[u].empty()) { // no move available, so the player to move has already lost
            winning[u] = 0;
            length[u] = 0;
            return;
        }
        int shortest_win = INT_MAX, longest_loss = -1, win_move = UNSET, loss_move = UNSET;
        for (int v : moves[u]) {
            compute(v);
            if (!winning[v]) { // moving here hands the opponent a lost position
                if (length[v] + 1 < shortest_win) shortest_win = length[v] + 1, win_move = v;
            }
            if (length[v] + 1 > longest_loss) longest_loss = length[v] + 1, loss_move = v;
        }
        if (win_move != UNSET) {
            winning[u] = 1;
            length[u] = shortest_win;
            best_move[u] = win_move;
        } else {
            winning[u] = 0;
            length[u] = longest_loss;
            best_move[u] = loss_move;
        }
    }

    void compute_all() {
        for (int u = 0; u < n; u++) compute(u);
    }

    bool is_win(int u) { compute(u); return winning[u]; }

    // moves remaining under optimal play: the remoteness of a win, the suspense of a loss
    int game_length(int u) { compute(u); return length[u]; }

    int optimal_move(int u) { compute(u); return best_move[u]; } // UNSET at a terminal position

    vector<int> optimal_line(int u) { // the whole game, position by position
        vector<int> line{u};
        compute(u);
        while (best_move[u] != UNSET) {
            u = best_move[u];
            compute(u);
            line.push_back(u);
        }
        return line;
    }
};

// Standard problem: a move graph and a start - report the winner and how many moves the game takes
void solve() {
    int n, m, start;
    cin >> n >> m >> start;
    Remoteness g(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_move(u, v);
    }
    cout << (g.is_win(start) ? "FIRST" : "SECOND") << ' ' << g.game_length(start) << '\n';
}
