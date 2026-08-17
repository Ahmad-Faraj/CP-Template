#include "../../core.h"
/*
 * Topic: Eulerian Paths and Cycles
 * 
 * Rules & Heuristics:
 * - A path that visits every edge exactly once.
 * - Exists in Undirected if: 0 or 2 nodes have odd degree, and graph is connected.
 * - Exists in Directed if: At most 1 node has out-in=1, 1 node has in-out=1, others in=out.
 * - Uses Hierholzer's Algorithm (DFS deleting edges).
 * - Complexity: O(V + E) time.
 */
// Time Complexity: O(V + E)
// Space Complexity: O(V + E)

// Time Complexity: O(V + E)
// Space Complexity: O(V + E)
struct DirectedPath {
    int n, m;
    vector<vector<int>> adj;
    vector<int> in, out, tour;
    int st, en;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Usage: Initialize Eulerian path structure for a directed graph.
    DirectedPath(int _n, int _m) : n(_n), m(_m), adj(n), in(n, 0), out(n, 0), st(-1), en(-1) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Usage: Add a directed edge from u to v.
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        out[u]++;
        in[v]++;
    }

    // Time Complexity: O(V)
    // Space Complexity: O(1)
    // Usage: Validate in/out degrees to ensure a directed Eulerian path can exist.
    bool check_euler() {
        for (int i = 0; i < n; i++) {
            if (abs(in[i] - out[i]) > 1) return false;
            if (in[i] == out[i] + 1) {
                if (en != -1) return false;
                en = i;
            }
            if (out[i] == in[i] + 1) {
                if (st != -1) return false;
                st = i;
            }
        }
        if (st == -1 && en == -1) {
            for (int i = 0; i < n; ++i) {
                if (out[i] > 0) {
                    st = i;
                    break;
                }
            }
            if (st == -1) st = 0;
        }
        return true;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Usage: Hierholzer's algorithm to recursively traverse the path.
    void dfs(int node) {
        while (out[node]) { dfs(adj[node][--out[node]]); }
        tour.emplace_back(node);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Usage: Compute and return the directed Eulerian path if one exists.
    bool get() {
        if (!check_euler()) return false;
        dfs(st);
        if ((int)(tour).size() != m + 1) return false;
        reverse(tour.begin(), tour.end());
        return true;
    }
};
