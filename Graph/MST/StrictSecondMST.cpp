#include "../../core.h"
/*
 * Algorithm: Strict Second Minimum Spanning Tree
 * Purpose: Finds the tree with strictly second minimum weight using binary lifting.
 * Complexity: Time O(E log V) | Space O(V log V)
 * Verified: ..........
 */
#include <vector>
#include <algorithm>
#include <numeric>

struct StrictSecondMST {
    struct Edge {
        int u, v;
        long long w;
        int id;
        bool in_mst;
        bool operator<(const Edge& o) const {
            return w < o.w;
        }
    };
    
    int n, m;
    vector<Edge> edges;
    vector<int> parent;
    vector<vector<pair<int, long long>>> adj;
    vector<vector<int>> up;
    vector<vector<long long>> mx1;
    vector<vector<long long>> mx2;
    vector<int> depth;
    int LOG;
    
    // Time Complexity: O(V log V)
    // Space Complexity: O(V log V)
    // StrictSecondMST ssmst(number_of_vertices, number_of_edges)
    StrictSecondMST(int n_, int m_) : n(n_), m(m_), parent(n_), adj(n_), depth(n_, 0) {
        LOG = 0;
        while ((1 << LOG) <= n) LOG++;
        up.assign(n_, vector<int>(LOG, 0));
        mx1.assign(n_, vector<long long>(LOG, -1));
        mx2.assign(n_, vector<long long>(LOG, -1));
        iota(parent.begin(), parent.end(), 0);
    }
    
    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // ssmst.addEdge(u, v, weight, id)
    void addEdge(int u, int v, long long w, int id) {
        edges.push_back({u, v, w, id, false});
    }
    
    // Time Complexity: O(alpha(V))
    // Space Complexity: O(1) auxiliary
    // int root = ssmst.findSet(u)
    int findSet(int u) {
        return parent[u] == u ? u : (parent[u] = findSet(parent[u]));
    }
    
    // Time Complexity: O(V log V)
    // Space Complexity: O(log V) auxiliary
    void dfs(int u, int p, int d) {
        up[u][0] = p;
        depth[u] = d;
        for (int i = 1; i < LOG; ++i) {
            up[u][i] = up[up[u][i - 1]][i - 1];
            vector<long long> vals = {
                mx1[u][i - 1],
                mx2[u][i - 1],
                mx1[up[u][i - 1]][i - 1],
                mx2[up[u][i - 1]][i - 1]
            };
            sort(vals.rbegin(), vals.rend());
            mx1[u][i] = vals[0];
            mx2[u][i] = -1;
            for (size_t j = 1; j < vals.size(); ++j) {
                if (vals[j] != -1 && vals[j] < vals[0]) {
                    mx2[u][i] = vals[j];
                    break;
                }
            }
        }
        for (const auto& e : adj[u]) {
            if (e.first != p) {
                mx1[e.first][0] = e.second;
                mx2[e.first][0] = -1;
                dfs(e.first, u, d + 1);
            }
        }
    }

    // Time Complexity: O(log V)
    // Space Complexity: O(1) auxiliary
    pair<long long, long long> getMax(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        long long m1 = -1, m2 = -1;
        
        auto update = [&](long long val) {
            if (val > m1) {
                m2 = m1;
                m1 = val;
            } else if (val < m1 && val > m2) {
                m2 = val;
            }
        };
        
        for (int i = LOG - 1; i >= 0; --i) {
            if (depth[u] - (1 << i) >= depth[v]) {
                update(mx1[u][i]);
                update(mx2[u][i]);
                u = up[u][i];
            }
        }
        if (u == v) return {m1, m2};
        for (int i = LOG - 1; i >= 0; --i) {
            if (up[u][i] != up[v][i]) {
                update(mx1[u][i]);
                update(mx2[u][i]);
                update(mx1[v][i]);
                update(mx2[v][i]);
                u = up[u][i];
                v = up[v][i];
            }
        }
        update(mx1[u][0]);
        update(mx2[u][0]);
        update(mx1[v][0]);
        update(mx2[v][0]);
        return {m1, m2};
    }
    
    // Time Complexity: O(E log V)
    // Space Complexity: O(V + E)
    // long long second_mst_cost = ssmst.solve()
    long long solve() {
        sort(edges.begin(), edges.end());
        long long mst_cost = 0;
        int edges_used = 0;
        for (auto& e : edges) {
            int u = findSet(e.u);
            int v = findSet(e.v);
            if (u != v) {
                parent[u] = v;
                mst_cost += e.w;
                e.in_mst = true;
                adj[e.u].push_back({e.v, e.w});
                adj[e.v].push_back({e.u, e.w});
                edges_used++;
            }
        }
        if (edges_used < n - 1) return -1;
        
        dfs(0, 0, 0);
        
        long long ans = -1;
        for (const auto& e : edges) {
            if (!e.in_mst) {
                auto mxs = getMax(e.u, e.v);
                long long mx = -1;
                if (mxs.first != e.w) {
                    mx = mxs.first;
                } else if (mxs.second != -1) {
                    mx = mxs.second;
                }
                
                if (mx != -1) {
                    long long cand = mst_cost + e.w - mx;
                    if (ans == -1 || cand < ans) {
                        ans = cand;
                    }
                }
            }
        }
        return ans;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        StrictSecondMST ssmst(n, m);
        for (int i = 0; i < m; ++i) {
            int u, v;
            long long w;
            cin >> u >> v >> w;
            ssmst.addEdge(u - 1, v - 1, w, i);
        }
        long long ans = ssmst.solve();
        if (ans == -1) cout << "IMPOSSIBLE\n";
        else cout << ans << "\n";
    }
    return 0;
}
