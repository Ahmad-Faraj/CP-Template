#include "../../../core.h"
/*
 * Topic: Block-Cut Tree (Biconnected Components)
 * Description: Constructs a bipartite tree where nodes represent either
 *              articulation points or biconnected components (blocks).
 * 
 * Important Facts:
 * - 0-based indexing for nodes by default.
 * - Time Complexity: O(V + E)
 * - Space Complexity: O(V + E)
 */
struct BlockCutTree {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low, st;
    vector<vector<int>> bcc;
    vector<bool> is_art;

    // Time Complexity: O(V)
    // Space Complexity: O(V)
    // BlockCutTree bct(n);
    BlockCutTree(int n_) : n(n_), timer(0), adj(n_), tin(n_, -1), low(n_, -1), is_art(n_, false) {}

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // bct.addEdge(u, v);
    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // Used internally.
    void dfs(int v, int p) {
        tin[v] = low[v] = timer++;
        st.push_back(v);
        int children = 0;
        for (int to : adj[v]) {
            if (to == p) continue;
            if (tin[to] != -1) {
                low[v] = min(low[v], tin[to]);
            } else {
                children++;
                dfs(to, v);
                low[v] = min(low[v], low[to]);
                if (low[to] >= tin[v]) {
                    is_art[v] = (p != -1 || children > 1);
                    bcc.push_back({});
                    while (true) {
                        int u = st.back();
                        st.pop_back();
                        bcc.back().push_back(u);
                        if (u == to) break;
                    }
                    bcc.back().push_back(v);
                }
            }
        }
    }

    // Time Complexity: O(V + E)
    // Space Complexity: O(V + E)
    // bct.build();
    void build() {
        for (int i = 0; i < n; i++) {
            if (tin[i] == -1) {
                dfs(i, -1);
            }
        }
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    if (cin >> n >> m) {
        BlockCutTree bct(n);
        for (int i = 0; i < m; i++) {
            int u, v;
            cin >> u >> v;
            bct.addEdge(u - 1, v - 1);
        }
        bct.build();
        cout << bct.bcc.size() << "\n";
        for (const auto& comp : bct.bcc) {
            cout << comp.size() << " ";
            for (int v : comp) cout << v + 1 << " ";
            cout << "\n";
        }
    }
    return 0;
}
