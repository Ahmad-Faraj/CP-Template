#include "../../core.h"
/*
 * Algorithm: Boruvka's Minimum Spanning Tree
 * Purpose: Builds MST in O(log V) parallel phases. Great for implicitly defined graphs.
 * Complexity: Time O(E log V) | Space O(V + E)
 * Verified: [CSES 1675 - Road Reparation](https://cses.fi/problemset/task/1675)
 */
#include <vector>
#include <numeric>

struct Boruvka {
    struct Edge {
        int u, v;
        long long w;
    };
    
    int n;
    vector<Edge> edges;
    vector<int> parent;
    
    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Boruvka b(number_of_vertices)
    Boruvka(int n_) : n(n_), parent(n_) {
        iota(parent.begin(), parent.end(), 0);
    }
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // b.addEdge(u, v, weight)
    void addEdge(int u, int v, long long w) {
        edges.push_back({u, v, w});
    }
    
    // Time Complexity: O(alpha(V))
    // Space Complexity: O(1) auxiliary
    // int root = b.findSet(u)
    int findSet(int u) {
        return parent[u] == u ? u : (parent[u] = findSet(parent[u]));
    }
    
    // Time Complexity: O(E log V)
    // Space Complexity: O(V)
    // long long mst_cost = b.solve()
    long long solve() {
        long long total_weight = 0;
        int components = n;
        
        while (components > 1) {
            vector<int> min_edge(n, -1);
            
            for (size_t i = 0; i < edges.size(); ++i) {
                int cu = findSet(edges[i].u);
                int cv = findSet(edges[i].v);
                
                if (cu != cv) {
                    if (min_edge[cu] == -1 || edges[i].w < edges[min_edge[cu]].w) {
                        min_edge[cu] = static_cast<int>(i);
                    }
                    if (min_edge[cv] == -1 || edges[i].w < edges[min_edge[cv]].w) {
                        min_edge[cv] = static_cast<int>(i);
                    }
                }
            }
            
            bool changed = false;
            for (int i = 0; i < n; ++i) {
                if (min_edge[i] != -1) {
                    int edge_idx = min_edge[i];
                    int cu = findSet(edges[edge_idx].u);
                    int cv = findSet(edges[edge_idx].v);
                    
                    if (cu != cv) {
                        parent[cu] = cv;
                        total_weight += edges[edge_idx].w;
                        components--;
                        changed = true;
                    }
                }
            }
            
            if (!changed) break;
        }
        
        return components == 1 ? total_weight : -1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Boruvka b(n);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            b.addEdge(u - 1, v - 1, w);
        }
        long long ans = b.solve();
        if (ans == -1) cout << "IMPOSSIBLE\n";
        else cout << ans << "\n";
    }
    return 0;
}
