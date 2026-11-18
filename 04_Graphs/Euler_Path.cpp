// Euler Path: a walk using every edge exactly once (a circuit if possible), directed or not.
// Use when: "traverse every road once", "visit every edge", rebuild a string from its overlapping
//           pieces (de Bruijn), domino chains, CSES Mail Delivery / Teleporters Path.
// Usage: Euler_Path E(n, directed); E.add_edge(u, v); if (E.run(src)) use E.path, E.edge_path;
// Handles: multi-edges, self loops, isolated nodes, edges spread over several components (fails),
//          a forced start src (fails if no trail starts there), m = 0 (true, path = {src} or {}).
// Time: O(n + m)
// Indexing: nodes 0..n (both 0- and 1-based work); edge ids 0-based in the order added.
//           path = v0..vm, edge_path = e0..e(m-1), edge e_i joins path[i] -> path[i+1].
// Note: run(-1) picks the start itself; when a circuit exists the walk returns to it.
//       Iterative Hierholzer, no recursion. Source: normalize-branch Eulerian_Path + KACTL
//       EulerWalk (edge ids on the stack).

#include <bits/stdc++.h>
using namespace std;

struct Euler_Path {
    int n, m = 0;
    bool directed;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<int> in_deg, path, edge_path;

    Euler_Path(int n, bool directed = false)
        : n(n), directed(directed), adj(n + 1), in_deg(n + 1) {}

    int add_edge(int u, int v) {
        adj[u].push_back({v, m});
        in_deg[v]++;
        if (!directed) adj[v].push_back({u, m}); // a self loop adds 2 to deg(u), as it should
        return m++;
    }

    bool run(int src = -1) { // false when no Euler trail exists (starting at src, if given)
        path.clear(), edge_path.clear();
        int start = -1, any = -1, odd = 0, plus = 0, minus = 0;
        for (int v = 0; v <= n; v++) {
            if (!adj[v].empty() && any == -1) any = v;
            if (directed) {
                int d = (int)adj[v].size() - in_deg[v];
                if (d == 1) plus++, start = v;
                else if (d == -1) minus++;
                else if (d != 0) return false;
            } else if (adj[v].size() % 2) {
                odd++;
                if (start == -1) start = v;
            }
        }
        if (directed ? plus > 1 || minus > 1 : odd != 0 && odd != 2) return false;
        if (m == 0) {
            if (src != -1) path = {src};
            return true;
        }
        bool circuit = directed ? plus == 0 : odd == 0;
        if (circuit) start = any;
        if (src != -1) {
            if (circuit ? adj[src].empty() : directed ? src != start : adj[src].size() % 2 == 0)
                return false;
            start = src;
        }
        vector<char> used(m, 0);
        vector<int> it(n + 1, 0);
        vector<pair<int, int>> st{{start, -1}}; // (node, edge used to reach it)
        while (!st.empty()) {
            auto [u, e] = st.back();
            if (it[u] < (int)adj[u].size()) {
                auto [v, id] = adj[u][it[u]++];
                if (!used[id]) used[id] = 1, st.push_back({v, id});
            } else {
                path.push_back(u);
                if (e != -1) edge_path.push_back(e);
                st.pop_back();
            }
        }
        if ((int)path.size() != m + 1) { // some edge lies in another component
            path.clear(), edge_path.clear();
            return false;
        }
        reverse(path.begin(), path.end());
        reverse(edge_path.begin(), edge_path.end());
        return true;
    }
};
