#include "../../../core.h"
/*
 * Algorithm: Push-Relabel Max Flow
 * Purpose: Fast max flow algorithm that pushes excess flow to neighbors. Great for dense graphs.
 * Complexity: Time O(V^3) | Space O(V + E)
 * Verified: [CSES 1694 - Download Speed](https://cses.fi/problemset/task/1694)
 */
struct PushRelabel {
    struct Edge {
        int to;
        long long cap, flow;
        int rev;
    };
    
    int n;
    vector<vector<Edge>> adj;
    vector<long long> excess;
    vector<int> height, count;
    vector<bool> active;
    queue<int> q;
    
    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initialize with number of vertices V.
    PushRelabel(int vertices) : n(vertices), adj(vertices), 
                                excess(vertices), height(vertices), 
                                count(vertices * 2), active(vertices) {}
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Add directed edge from u to v with given capacity.
    void add_edge(int u, int v, long long cap) {
        adj[u].push_back({v, cap, 0, static_cast<int>(adj[v].size())});
        adj[v].push_back({u, 0, 0, static_cast<int>(adj[u].size()) - 1});
    }
    
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Enqueue active node.
    void enqueue(int v) {
        if (!active[v] && excess[v] > 0) {
            active[v] = true;
            q.push(v);
        }
    }
    
    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Push flow on a specific edge.
    void push(int u, int i) {
        auto& edge = adj[u][i];
        long long d = min(excess[u], edge.cap - edge.flow);
        if (d == 0 || height[u] <= height[edge.to]) return;
        
        excess[u] -= d;
        excess[edge.to] += d;
        edge.flow += d;
        adj[edge.to][edge.rev].flow -= d;
        enqueue(edge.to);
    }
    
    // Time Complexity: O(V)
    // Space Complexity: O(1)
    // Relabel height of node u.
    void relabel(int u) {
        count[height[u]]--;
        int d = 2 * n;
        for (const auto& edge : adj[u]) {
            if (edge.cap - edge.flow > 0) {
                d = min(d, height[edge.to] + 1);
            }
        }
        height[u] = d;
        count[height[u]]++;
        enqueue(u);
    }
    
    // Time Complexity: O(V)
    // Space Complexity: O(1)
    // Perform gap heuristic.
    void gap(int k) {
        for (int v = 0; v < n; v++) {
            if (height[v] >= k && height[v] < n) {
                count[height[v]]--;
                height[v] = max(height[v], n + 1);
                count[height[v]]++;
                enqueue(v);
            }
        }
    }
    
    // Time Complexity: O(V^3)
    // Space Complexity: O(V)
    // Compute max flow from source s to sink t.
    long long max_flow(int s, int t) {
        count[0] = n - 1;
        count[n] = 1;
        height[s] = n;
        active[s] = active[t] = true;
        
        for (int i = 0; i < static_cast<int>(adj[s].size()); i++) {
            auto& edge = adj[s][i];
            excess[s] += edge.cap;
            push(s, i);
        }
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            active[u] = false;
            
            int v_h = height[u];
            for (int i = 0; i < static_cast<int>(adj[u].size()) && excess[u] > 0; i++) {
                push(u, i);
            }
            
            if (excess[u] > 0) {
                if (count[v_h] == 1) {
                    gap(v_h);
                } else {
                    relabel(u);
                }
            }
        }
        return excess[t];
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
    PushRelabel pr(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long c;
        cin >> u >> v >> c;
        pr.add_edge(u, v, c);
    }
    cout << pr.max_flow(1, n) << "\n";
    return 0;
}
