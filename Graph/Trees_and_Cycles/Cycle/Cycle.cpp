#include "../../../core.h"
/*
 * Algorithm: Eulerian Cycle
 * Purpose: Finds a cycle that traverses every edge exactly once.
 * Complexity: Time O(V + E) | Space O(V + E)
 * Verified: ..........
 */
template <typename T = int>
struct CycleUndirected {
    T n;
    vector<vector<T>> adj;
    vector<bool> visited;
    vector<T> parent;
    T cycle_start, cycle_end;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize the CycleUndirected struct with the number of nodes.
    CycleUndirected(T _n) : n(_n), adj(_n + 1), visited(_n + 1, false), parent(_n + 1, -1) {
        cycle_start = -1;
        cycle_end = -1;
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Add an undirected edge between nodes u and v.
    void add_edge(T u, T v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }




    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // DFS helper to find a cycle in the graph.
    bool dfs(T v, T par) {
        visited[v] = true;
        for (auto &u : adj[v]) {
            if (u == par) continue;
            if (visited[u]) {
                cycle_end = v;
                cycle_start = u;
                return true;
            }
            parent[u] = v;
            if (dfs(u, v)) return true;
        }
        return false;
    }

    // Get the nodes forming a cycle, or empty vector if no cycle exists.
    vector<T> get_cycle() {
        cycle_start = -1;
        fill(visited.begin(), visited.end(), false);
        fill(parent.begin(), parent.end(), -1);
        
        for (T i = 0; i <= n; i++) {
            if (!visited[i] && dfs(i, parent[i])) {
                break;
            }
        }
        
        if (cycle_start == -1) return {};
        
        vector<T> res;
        res.push_back(cycle_start);
        for (T v = cycle_end; v != cycle_start; v = parent[v]) {
            res.push_back(v);
        }
        res.push_back(cycle_start);
        reverse(res.begin(), res.end());
        return res;
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        CycleUndirected<int> cyc(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            cyc.add_edge(u, v);
        }
        vector<int> cycle = cyc.get_cycle();
        if (cycle.empty()) {
            cout << "IMPOSSIBLE\n";
        } else {
            cout << cycle.size() << "\n";
            for (int i = 0; i < (int)cycle.size(); i++) {
                cout << cycle[i] << (i == (int)cycle.size() - 1 ? "" : " ");
            }
            cout << "\n";
        }
    }
    return 0;
}
