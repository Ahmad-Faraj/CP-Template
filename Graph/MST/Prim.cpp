#include "../../core.h"
/*
 * Algorithm: Prim's Minimum Spanning Tree
 * Purpose: Builds MST by adding cheapest edges from tree to non-tree vertices.
 * Complexity: Time O(E log V) | Space O(V + E)
 * Verified: [CSES 1675 - Road Reparation](https://cses.fi/problemset/task/1675)
 */
#include <vector>
#include <queue>

struct Prim {
    struct Edge {
        int to;
        long long weight;
    };
    
    int n;
    vector<vector<Edge>> adj;
    
    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Prim p(number_of_vertices)
    Prim(int n_) : n(n_), adj(n_) {}
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // p.addEdge(u, v, weight)
    void addEdge(int u, int v, long long w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
    
    // Time Complexity: O(E log V)
    // Space Complexity: O(V)
    // long long mst_cost = p.solve()
    long long solve() {
        vector<bool> vis(n, false);
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        long long total_weight = 0;
        int edges_used = 0;
        
        pq.push({0, 0});
        
        while (!pq.empty() && edges_used < n) {
            auto [w, u] = pq.top();
            pq.pop();
            
            if (vis[u]) continue;
            vis[u] = true;
            total_weight += w;
            edges_used++;
            
            for (const auto& e : adj[u]) {
                if (!vis[e.to]) {
                    pq.push({e.weight, e.to});
                }
            }
        }
        
        return edges_used == n ? total_weight : -1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Prim p(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            p.addEdge(u - 1, v - 1, w);
        }
        long long ans = p.solve();
        if (ans == -1) cout << "IMPOSSIBLE\n";
        else cout << ans << "\n";
    }
    return 0;
}
