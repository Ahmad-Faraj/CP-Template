#include "../../../core.h"
/*
 * Algorithm: Edmonds-Karp Max Flow
 * Purpose: Uses BFS to find augmenting paths. Simple to code, good for small networks.
 * Complexity: Time O(V * E^2) | Space O(V + E)
 * Verified: [CSES 1694 - Download Speed](https://cses.fi/problemset/task/1694)
 */
struct EdmondsKarp {
    struct Edge {
        int to;
        long long cap, flow;
        int rev;
    };
    
    int n;
    vector<vector<Edge>> adj;
    
    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize with number of vertices V.
    EdmondsKarp(int vertices) : n(vertices), adj(vertices) {}
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Add directed edge from u to v with given capacity.
    void add_edge(int u, int v, long long cap) {
        adj[u].push_back({v, cap, 0, static_cast<int>(adj[v].size())});
        adj[v].push_back({u, 0, 0, static_cast<int>(adj[u].size()) - 1});
    }
    
    // Time Complexity: O(V * E^2)
    // Space Complexity: O(V)
    // Compute max flow from source s to sink t.
    long long max_flow(int s, int t) {
        long long flow = 0;
        vector<int> parent(n);
        vector<int> parent_edge(n);
        
        while (true) {
            fill(parent.begin(), parent.end(), -1);
            queue<pair<int, long long>> q;
            q.push({s, (long long)2e18});
            parent[s] = -2;
            
            long long pushed = 0;
            while (!q.empty()) {
                int curr = q.front().first;
                long long f = q.front().second;
                q.pop();
                
                if (curr == t) {
                    pushed = f;
                    break;
                }
                
                for (int i = 0; i < static_cast<int>(adj[curr].size()); ++i) {
                    auto& edge = adj[curr][i];
                    int next = edge.to;
                    if (parent[next] == -1 && edge.cap - edge.flow > 0) {
                        parent[next] = curr;
                        parent_edge[next] = i;
                        long long new_f = min(f, edge.cap - edge.flow);
                        q.push({next, new_f});
                    }
                }
            }
            
            if (pushed == 0) break;
            flow += pushed;
            
            int curr = t;
            while (curr != s) {
                int p = parent[curr];
                int idx = parent_edge[curr];
                int rev_idx = adj[p][idx].rev;
                
                adj[p][idx].flow += pushed;
                adj[curr][rev_idx].flow -= pushed;
                curr = p;
            }
        }
        return flow;
    }

};

/*
 * Takes N (number of nodes) and M (number of edges).
 * Followed by M lines of directed edges: u v w (from u to v with capacity w).
 * Gives the maximum flow from node 1 to node N.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    EdmondsKarp ek(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        ek.add_edge(u, v, c);
    }
    cout << ek.max_flow(1, n) << "\n";
    return 0;
}
