#include "../../../core.h"
/*
 * Algorithm: Hopcroft-Karp Bipartite Matching
 * Purpose: Fast maximum bipartite matching using BFS and DFS.
 * Complexity: Time O(E * sqrt(V)) | Space O(V + E)
 * Verified: [CSES 1696 - School Dance](https://cses.fi/problemset/task/1696)
 */

struct HopcroftKarp {
    int n, m;
    vector<int> l, r, d;
    vector<vector<int>> g;
    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes the HopcroftKarp structure with left side size _n and right side size _m.
    HopcroftKarp(int _n, int _m) {
        n = _n;
        m = _m;
        int p = _n + _m + 1;
        g.resize(p);
        l.resize(p, 0);
        r.resize(p, 0);
        d.resize(p, 0);
    }
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Adds a directed edge from left node u to right node v.
    void add_edge(int u, int v) {
        g[u].push_back(v + n); // right id is increased by n, so is l[u]
    }
    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Finds the shortest augmenting paths in the residual graph using BFS.
    bool bfs() {
        queue<int> q;
        for (int u = 1; u <= n; u++) {
            if (!l[u])
                d[u] = 0, q.push(u);
            else
                d[u] = 1e9;
        }
        d[0] = 1e9;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (auto v : g[u]) {
                if (d[r[v]] == 1e9) {
                    d[r[v]] = d[u] + 1;
                    q.push(r[v]);
                }
            }
        }
        return d[0] != 1e9;
    }
    // Time Complexity: O(E)
    // Space Complexity: O(V) for recursion stack
    // Finds a maximal set of disjoint augmenting paths using DFS.
    bool dfs(int u) {
        if (!u) return true;
        for (auto v : g[u]) {
            if (d[r[v]] == d[u] + 1 && dfs(r[v])) {
                l[u] = v;
                r[v] = u;
                return true;
            }
        }
        d[u] = 1e9;
        return false;
    }
    // Time Complexity: O(E * sqrt(V))
    // Space Complexity: O(V)
    // Computes the maximum bipartite matching.
    int maximum_matching() {
        int ans = 0;
        while (bfs()) {
            for (int u = 1; u <= n; u++)
                if (!l[u] && dfs(u)) ans++;
        }
        return ans;
    }


};

/*
 * Takes n (left size), m (right size), and k (edges). 
 * Gives maximum matching size and the matched pairs.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    if (cin >> n >> m >> k) {
        HopcroftKarp M(n, m);
        for (int i = 0; i < k; ++i) {
            int u, v;
            cin >> u >> v;
            M.add_edge(u, v);
        }
        cout << M.maximum_matching() << "\n";
        for (int i = 1; i <= n; ++i) {
            if (M.l[i] != 0) {
                cout << i << " " << M.l[i] - n << "\n";
            }
        }
    }
    return 0;
}