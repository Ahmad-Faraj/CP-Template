#include <bits/stdc++.h>
using namespace std;

#define ll long long
#define sz(x) (int)x.size()
#define nl '\n'

/*
    Undirected Euler Path / Circuit

    [1] Time & Space Complexity
    - Time: O(n + m)
    - Space: O(n + m)

    [2] When You Should Use It
    - Find an Euler Path or Euler Circuit.
    - Every edge is used exactly once.

    [3] Notes
    - Euler Path: exactly 2 vertices have odd degree.
    - Euler Circuit: all vertices have even degree.
    - Uses Hierholzer's Algorithm.
    - Edge IDs handle multiple edges.
*/

int n, m;
vector<vector<pair<int, int>>> adj; // this will include the vertex and edge id
vector<int> deg;
bool Undirected_Euler() // this function for path & circuit
{
    int st = -1, cycle_st = -1, odds = 0;
    for (int u = 1; u <= n; u++) {
        if (deg[u] & 1) odds++, (st == -1 ? st = u : st);
        if (!adj[u].empty()) cycle_st = u;
    }

    if (odds > 2 || odds == 1) return false;
    if (st == -1) st = cycle_st;

    if (cycle_st == -1) {
        cout << "Yes" << nl << 0 << nl << nl;
        return true;
    }

    vector<int> nodes, edges, vis(m);
    auto dfs = [&](auto &&dfs, int u) -> void {
        while (!adj[u].empty()) {
            auto [v, e] = adj[u].back();
            adj[u].pop_back();

            if (vis[e]) continue;

            vis[e] = 1;
            dfs(dfs, v);
            nodes.push_back(v);
            edges.push_back(e);
        }
    };

    dfs(dfs, st);
    if (edges.size() != m) return false;

    nodes.push_back(st);
    ::reverse(edges.begin(), edges.end());
    ::reverse(nodes.begin(), nodes.end());

    cout << "Yes" << endl;

    for (auto &i : nodes) cout << i - 1 << ' ';
    cout << endl;

    for (auto &i : edges) cout << i << ' ';
    cout << endl;

    return true;
}

signed main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int t = 1;
    cin >> t;
    while (t--) {

        cin >> n >> m;
        adj.assign(n + 1, {});
        deg.assign(n + 1, 0);
        for (int e = 0; e < m; e++) {
            int u, v;
            cin >> u >> v;
            u++, v++;
            adj[u].push_back({v, e});
            adj[v].push_back({u, e});

            deg[u]++, deg[v]++;
        }

        if (!Undirected_Euler()) cout << "No" << nl;
    }
}
