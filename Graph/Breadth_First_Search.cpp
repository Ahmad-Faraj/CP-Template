// Breadth First Search: shortest paths when every edge costs the same, or costs only 0 or 1.
// Use when: fewest moves on an unweighted graph or grid, "nearest of these sources", 0/1 weights without Dijkstra.
// Handles: unweighted graphs, 0/1 weighted graphs via a deque, several sources at once, grids, and path recovery.
// Time: O(n + m) for both the plain and the 0/1 version
// Indexing: 1-based nodes for the graph form; grids are 0-based (row, col)
// Note: 0/1 weights do NOT need Dijkstra - a deque gives O(n + m). Any other weights do; use Dijkstra.cpp.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Breadth_First_Search {
    static constexpr int INF = numeric_limits<int>::max() / 4;

    int n;
    vector<vector<pair<int, int>>> adj; // (neighbour, weight 0 or 1)
    vector<int> dist, par;

    Breadth_First_Search(int n) : n(n), adj(n + 1), dist(n + 1, INF), par(n + 1, -1) {}

    void add_edge(int u, int v, int w = 1, bool bidirectional = false) {
        adj[u].push_back({v, w});
        if (bidirectional) adj[v].push_back({u, w});
    }

    static bool is_inf(int d) { return d >= INF; }

    vector<int> run(const vector<int> &sources) { // distance to every node from the nearest source
        dist.assign(n + 1, INF);
        par.assign(n + 1, -1);
        deque<int> q;
        for (int s : sources) {
            dist[s] = 0;
            q.push_back(s);
        }
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            for (auto [v, w] : adj[u]) {
                if (dist[u] + w >= dist[v]) continue;
                dist[v] = dist[u] + w;
                par[v] = u;
                // a 0-weight edge keeps the node in the current layer, so it goes to the FRONT
                if (w == 0) q.push_front(v);
                else q.push_back(v);
            }
        }
        return dist;
    }

    vector<int> run(int source) { return run(vector<int>{source}); }

    int distance(int source, int target) { return run(source)[target]; }

    vector<int> path(int source, int target) { // the nodes source..target, or empty if unreachable
        run(source);
        if (is_inf(dist[target])) return {};
        vector<int> p;
        for (int v = target; v != -1; v = par[v]) p.push_back(v);
        reverse(p.begin(), p.end());
        return p;
    }
};

// Grid shortest path in steps; '#' blocks, anything else is walkable
vector<vector<int>> grid_bfs(const vector<string> &grid, const vector<pair<int, int>> &sources, char wall = '#') {
    int rows = (int)grid.size(), cols = rows ? (int)grid[0].size() : 0;
    const int INF = numeric_limits<int>::max() / 4;
    vector<vector<int>> dist(rows, vector<int>(cols, INF));
    deque<pair<int, int>> q;
    for (auto [r, c] : sources)
        if (r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] != wall) {
            dist[r][c] = 0;
            q.push_back({r, c});
        }
    const int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop_front();
        for (int k = 0; k < 4; k++) {
            int nr = r + dr[k], nc = c + dc[k];
            if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
            if (grid[nr][nc] == wall || dist[nr][nc] <= dist[r][c] + 1) continue;
            dist[nr][nc] = dist[r][c] + 1;
            q.push_back({nr, nc});
        }
    }
    return dist;
}

// Standard problem: unweighted graph, report the fewest edges from 1 to n and the route
void solve() {
    int n, m;
    cin >> n >> m;
    Breadth_First_Search g(n);
    for (int i = 0, u, v; i < m; i++) {
        cin >> u >> v;
        g.add_edge(u, v, 1, true);
    }
    vector<int> p = g.path(1, n);
    if (p.empty()) {
        cout << "IMPOSSIBLE\n";
        return;
    }
    cout << p.size() << '\n';
    for (size_t i = 0; i < p.size(); i++) cout << p[i] << " \n"[i + 1 == p.size()];
}
