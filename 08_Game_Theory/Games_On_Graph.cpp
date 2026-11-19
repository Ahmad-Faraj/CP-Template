// Games On Graph: win / lose / draw for every position of a game whose move graph may have cycles.
// Use when: positions can repeat so Grundy does not apply, "Win / Lose / Loop", token on a graph.
// Usage: Games_On_Graph g(n); g.add_move(u, v); g.run(); g.winning[v] / g.losing[v] / g.draw(v)
// Handles: cycles and self-loops (-> draw), multi-edges, several terminal positions.
// Time: O(n + m)
// Indexing: 0-based positions; add_move(u, v): the player to move at u may move the token to v
// Note: a position with no moves is lost for the player to move. Neither winning nor losing = draw
//       (play goes on forever). Call run() once, after all add_move. Recursive dfs, depth up to n:
//       run with a big stack for n ~ 1e5+.

#include <bits/stdc++.h>
using namespace std;

struct Games_On_Graph {
    int n;
    vector<vector<int>> adj_rev;
    vector<bool> winning, losing, visited;
    vector<int> degree;

    Games_On_Graph(int n) : n(n), adj_rev(n), winning(n), losing(n), visited(n), degree(n) {}

    void add_move(int u, int v) { adj_rev[v].push_back(u), degree[u]++; }

    void dfs(int v) {
        visited[v] = true;
        for (int u : adj_rev[v]) {
            if (!visited[u]) {
                if (losing[v])
                    winning[u] = true;
                else if (--degree[u] == 0)
                    losing[u] = true;
                else
                    continue;
                dfs(u);
            }
        }
    }

    void run() {
        for (int v = 0; v < n; v++)
            if (degree[v] == 0) losing[v] = true;
        for (int v = 0; v < n; v++)
            if (degree[v] == 0 && !visited[v]) dfs(v);
    }

    bool draw(int v) const { return !winning[v] && !losing[v]; }
};
