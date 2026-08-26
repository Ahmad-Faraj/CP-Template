#include "../../../core.h"

/*
 * Topic: Tarjan's Strongly Connected Components (SCC)
 * Description: Finds SCCs in a directed graph using a single DFS pass.
 *              Also builds the condensed DAG of SCCs.
 *
 * Important Facts:
 * - 1-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
template <typename T = int> struct Tarjan {
    int n, timer = 0, scc_count = 0;
    vector<vector<T>> adj;
    vector<int> dfs_num, dfs_low, comp;
    vector<bool> in_stack;
    stack<T> st;
    vector<vector<T>> scc;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // Initializes Tarjan's state for the given number of nodes.
    Tarjan(int nodes) {
        n = nodes;
        adj.assign(n + 1, {});
        dfs_num.assign(n + 1, 0);
        dfs_low.assign(n + 1, 0);
        in_stack.assign(n + 1, false);
        comp.assign(n + 1, 0);
    }

    // Time Complexity: O(1) amortized
    // Space Complexity: O(1)
    // Adds a directed edge from u to v.
    void add_edge(T u, T v) { adj[u].push_back(v); }

    // Time Complexity: O(V + E) overall
    // Space Complexity: O(V) for stack
    // Recursively explores the graph to build strongly connected components.
    void dfs(T u) {
        dfs_num[u] = dfs_low[u] = ++timer;
        st.push(u);
        in_stack[u] = true;
        for (T v : adj[u]) {
            if (!dfs_num[v]) {
                dfs(v);
                dfs_low[u] = min(dfs_low[u], dfs_low[v]);
            } else if (in_stack[v]) {
                dfs_low[u] = min(dfs_low[u], dfs_num[v]);
            }
        }
        if (dfs_low[u] == dfs_num[u]) {
            scc_count++;
            scc.push_back({});
            while (true) {
                T v = st.top();
                st.pop();
                in_stack[v] = false;
                comp[v] = scc_count;
                scc.back().push_back(v);
                if (v == u) break;
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Iterates over all vertices to guarantee every component is found.
    void build_scc() {
        for (int i = 1; i <= n; i++)
            if (!dfs_num[i]) dfs(i);
    }

    vector<vector<T>> dag;

    void build_dag() {
        dag.assign(scc_count + 1, {});
        set<pair<int, int>> used;
        for (int u = 1; u <= n; u++) {
            for (auto v : adj[u]) {
                int cu = comp[u], cv = comp[v];
                if (cu != cv && !used.count({cu, cv})) {
                    dag[cu].push_back(cv);
                    used.insert({cu, cv});
                }
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        Tarjan<int> tarjan(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            tarjan.add_edge(u, v);
        }
        tarjan.build_scc();
        cout << tarjan.scc_count << "\n";
        for (int i = 1; i <= n; i++) {
            cout << tarjan.comp[i] << (i == n ? "" : " ");
        }
        cout << "\n";
    }
    return 0;
}
