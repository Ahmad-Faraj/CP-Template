// Strong Orientation: direct every undirected edge so the digraph has as few SCCs as possible.
// Use when: "make all roads one-way and keep every city reachable", "fewest one-way pieces".
// Usage: Strong_Orientation S(n); S.add_edge(u, v); int k = S.run(); S.orient[i] is '>' or '<'.
// Handles: disconnected graphs, multi-edges, self loops. min SCCs = components + bridges.
// Time: O(n + m)
// Indexing: 0-based nodes and edge ids (order added); '>' = u -> v as added, '<' = v -> u
// Note: one SCC iff connected and bridgeless (run() == 1). find_bridges recurses up to n deep.

#include <bits/stdc++.h>
using namespace std;

struct Strong_Orientation {
    int n, timer = 0, comp_cnt = 0, bridge_cnt = 0;
    vector<vector<pair<int, int>>> adj; // (neighbour, edge id)
    vector<pair<int, int>> edges;
    vector<int> tin, low;
    vector<bool> edge_used;
    string orient;

    Strong_Orientation(int n) : n(n), adj(n) {}

    int add_edge(int a, int b) {
        adj[a].push_back({b, (int)edges.size()});
        adj[b].push_back({a, (int)edges.size()});
        edges.push_back({a, b});
        return (int)edges.size() - 1;
    }

    void find_bridges(int v) { // orient each edge the first time it is walked
        low[v] = tin[v] = timer++;
        for (auto p : adj[v]) {
            if (edge_used[p.second]) continue;
            edge_used[p.second] = true;
            orient[p.second] = v == edges[p.second].first ? '>' : '<';
            int nv = p.first;
            if (tin[nv] == -1) {
                find_bridges(nv);
                low[v] = min(low[v], low[nv]);
                if (low[nv] > tin[v]) bridge_cnt++;
            } else {
                low[v] = min(low[v], tin[nv]);
            }
        }
    }

    int run() { // returns the minimum number of SCCs; fills orient, comp_cnt, bridge_cnt
        int m = (int)edges.size();
        tin.assign(n, -1), low.assign(n, -1);
        edge_used.assign(m, false);
        orient.assign(m, '>');
        timer = comp_cnt = bridge_cnt = 0;
        for (int v = 0; v < n; v++)
            if (tin[v] == -1) comp_cnt++, find_bridges(v);
        return comp_cnt + bridge_cnt;
    }
};
