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

struct DirectedCycle {
    int n, m;
    vector<vector<int>> adj, radj;
    vector<int> in, out, tour;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Usage: Initialize Eulerian cycle structure for a directed graph.
    DirectedCycle(int _n, int _m) : n(_n), m(_m), adj(n), radj(n), in(n, 0), out(n, 0) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Usage: Add a directed edge from u to v.
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        radj[v].push_back(u);
        out[u]++;
        in[v]++;
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Usage: Check connectivity of the graph using DFS.
    void dfs1(int s, const vector<vector<int>>& g, vector<char>& vis) {
        stack<int> st;
        st.push(s);
        vis[s] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            for (int u : g[v]) {
                if (!vis[u]) {
                    vis[u] = 1;
                    st.push(u);
                }
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Usage: Extract the Eulerian cycle using Hierholzer's algorithm.
    void dfs2(int v) {
        while (!adj[v].empty()) {
            int u = adj[v].back();
            adj[v].pop_back();
            dfs2(u);
        }
        tour.push_back(v);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Usage: Verify existence and compute the directed Eulerian cycle.
    bool get() {
        for (int i = 0; i < n; ++i) {
            if (in[i] != out[i]) return false;
        }
        int start = -1;
        for (int i = 0; i < n; ++i) {
            if (out[i] > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) {
            tour = {0};
            return (m == 0);
        }
        vector<char> vis(n, 0);
        dfs1(start, adj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;
        fill(vis.begin(), vis.end(), 0);
        dfs1(start, radj, vis);
        for (int i = 0; i < n; ++i)
            if ((in[i] + out[i]) > 0 && !vis[i]) return false;
        tour.clear();
        dfs2(start);
        reverse(tour.begin(), tour.end());
        return (int)(tour).size() == m + 1;
    }
};
