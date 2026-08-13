#include "../../../core.h"
/*
 * Topic: Bridge Finding
 * Description: Finds edges that disconnect the graph if removed.
 *              Also constructs the 2-Edge-Connected Components (2-ECC) condensed graph.
 * 
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
template <typename T = int>
struct Bridges {
    T dfs_timer;
    vector<vector<T>> adj;
    vector<pair<T, T>> bridges;
    vector<T> dfs_num, dfs_low, vis;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes structures for a graph with V vertices.
    Bridges(T n) {
        dfs_timer = 0;
        adj = vector<vector<T>>(n + 1);
        dfs_num = dfs_low = vis = vector<T>(n + 1);
    }

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Adds an edge between u and v, optionally bidirectional.
    void add_edge(T u, T v, bool bidirectional = true) {
        adj[u].push_back(v);
        if (bidirectional) adj[v].push_back(u);
    }

    // Time Complexity: O(V + E) overall
    // Space Complexity: O(V) recursion stack
    // DFS traversal to compute low-links and find bridges.
    void tarjan(T node, T parent) {
        dfs_num[node] = dfs_low[node] = ++dfs_timer;
        vis[node] = 1;
        for (auto child : adj[node]) {
            if (child == parent) continue;
            if (!dfs_num[child]) {
                tarjan(child, node);
                dfs_low[node] = min(dfs_low[node], dfs_low[child]);
                if (dfs_low[child] > dfs_num[node]) { bridges.push_back({node, child}); }
            } else if (vis[child]) {
                dfs_low[node] = min(dfs_low[node], dfs_num[child]);
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V)
    // Evaluates all graph components to find all bridges.
    void solve() {
        for (T i = 1; i < (T)(int)(adj).size(); ++i)
            if (!dfs_num[i]) tarjan(i, -1);
    }



    vector<T> comp_id;
    vector<vector<T>> comp_adj;
    T comp_cnt = 0;
    
    void make_scc_graph() {
        T n = (T)(int)(adj).size() - 1;
        comp_id.assign(n + 1, 0);
        comp_cnt = 0;
        set<pair<T, T>> is_bridge;
        for (auto& e : bridges) { is_bridge.insert({min(e.first, e.second), max(e.first, e.second)}); }
        for (T i = 1; i <= n; ++i) {
            if (comp_id[i]) continue;
            ++comp_cnt;
            stack<T> st;
            st.push(i);
            comp_id[i] = comp_cnt;
            while (!st.empty()) {
                T u = st.top();
                st.pop();
                for (T v : adj[u]) {
                    if (is_bridge.count({min(u, v), max(u, v)})) continue;
                    if (!comp_id[v]) {
                        comp_id[v] = comp_cnt;
                        st.push(v);
                    }
                }
            }
        }
        comp_adj.assign(comp_cnt + 1, vector<T>());
        set<pair<T, T>> used;
        for (auto& e : bridges) {
            T u = e.first, v = e.second;
            T cu = comp_id[u], cv = comp_id[v];
            if (cu == cv) continue;
            T a = min(cu, cv), b = max(cu, cv);
            if (used.insert({a, b}).second) {
                comp_adj[a].push_back(b);
                comp_adj[b].push_back(a);
            }
        }
    }

};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Bridges<int> bridges(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            bridges.add_edge(u, v);
        }
        bridges.solve();
        cout << bridges.bridges.size() << "\n";
        for (auto p : bridges.bridges) {
            cout << p.first << " " << p.second << "\n";
        }
    }
    return 0;
}
