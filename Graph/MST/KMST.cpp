#include "../../core.h"
/*
 * Topic: K-th Minimum Spanning Tree
 * Description: Finds the first K minimum spanning trees using Lawler's procedure (Edge Partitioning).
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(K * V * E)
 * - Space Complexity: O(K * V)
 */
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>
#include <set>

using namespace std;

struct KMST {
    struct Edge {
        int u, v;
        long long w;
        int id;
        bool operator<(const Edge& o) const {
            return w < o.w;
        }
    };
    
    int n, m;
    vector<Edge> edges;
    
    KMST(int n_, int m_) : n(n_), m(m_) {}
    
    void addEdge(int u, int v, long long w, int id) {
        edges.push_back({u, v, w, id});
    }
    
    struct DSU {
        vector<int> parent;
        DSU(int n) {
            parent.resize(n);
            iota(parent.begin(), parent.end(), 0);
        }
        int findSet(int u) {
            return parent[u] == u ? u : (parent[u] = findSet(parent[u]));
        }
        bool unionSets(int u, int v) {
            u = findSet(u);
            v = findSet(v);
            if (u != v) {
                parent[u] = v;
                return true;
            }
            return false;
        }
    };
    
    struct State {
        long long cost;
        vector<int> tree_edges;
        set<int> forced;
        set<int> forbidden;
        
        bool operator>(const State& o) const {
            return cost > o.cost;
        }
    };
    
    pair<long long, vector<int>> get_mst(const set<int>& forced, const set<int>& forbidden) {
        DSU dsu(n);
        long long cost = 0;
        vector<int> tree_edges;
        int edges_used = 0;
        
        // Add forced edges first
        for (int id : forced) {
            auto it = find_if(edges.begin(), edges.end(), [&](const Edge& e) { return e.id == id; });
            if (it != edges.end()) {
                if (dsu.unionSets(it->u, it->v)) {
                    cost += it->w;
                    tree_edges.push_back(id);
                    edges_used++;
                } else {
                    return {-1, {}}; // Cycle in forced edges
                }
            }
        }
        
        // Add remaining edges greedily
        for (const auto& e : edges) {
            if (forced.count(e.id) || forbidden.count(e.id)) continue;
            if (dsu.unionSets(e.u, e.v)) {
                cost += e.w;
                tree_edges.push_back(e.id);
                edges_used++;
            }
        }
        
        if (edges_used < n - 1) return {-1, {}};
        return {cost, tree_edges};
    }
    
    vector<long long> solve(int k) {
        sort(edges.begin(), edges.end());
        vector<long long> results;
        
        priority_queue<State, vector<State>, greater<State>> pq;
        
        set<int> empty_set;
        auto initial = get_mst(empty_set, empty_set);
        if (initial.first == -1) return results; // No MST possible
        
        pq.push({initial.first, initial.second, empty_set, empty_set});
        
        while (!pq.empty() && (int)results.size() < k) {
            State curr = pq.top();
            pq.pop();
            
            results.push_back(curr.cost);
            
            set<int> current_forced = curr.forced;
            
            for (int i = 0; i < (int)curr.tree_edges.size(); ++i) {
                int e_id = curr.tree_edges[i];
                if (curr.forced.count(e_id)) continue;
                
                set<int> next_forbidden = curr.forbidden;
                next_forbidden.insert(e_id);
                
                auto next_mst = get_mst(current_forced, next_forbidden);
                if (next_mst.first != -1) {
                    pq.push({next_mst.first, next_mst.second, current_forced, next_forbidden});
                }
                
                current_forced.insert(e_id);
            }
        }
        
        return results;
    }
};

/*
 * Takes N (number of nodes), M (number of edges), and K (number of MSTs).
 * Followed by M lines of undirected edges: u v w.
 * Gives the weights of the K minimum spanning trees.
 */
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m, k;
    if (cin >> n >> m >> k) {
        KMST kmst(n, m);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            kmst.addEdge(u - 1, v - 1, w, i);
        }
        vector<long long> ans = kmst.solve(k);
        for (long long cost : ans) {
            cout << cost << "\n";
        }
    }
    return 0;
}
