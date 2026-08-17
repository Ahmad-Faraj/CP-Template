#include "../../core.h"
/*
Topic: Sparse Table LCA (Euler Tour + RMQ)
Rules & Heuristics: Uses Euler tour to convert LCA to RMQ on depth array.
Problem Signatures: Static trees requiring O(1) LCA queries.
Complexity: Precomputation O(N log N), Query O(1).
 *
 * # Sparse Tables on Trees
 *
 * Sparse tables are generally $O(N \log N)$ to build and $O(1)$ to query for idempotent operations (like min/max/gcd).
 *
 * - Run an Euler tour where you record a node every time you visit it (size $2N-1$).
 * - Keep track of the depth of each node visited.
 * - The LCA of $u$ and $v$ is the node with the minimum depth in the range `[first_visit[u], first_visit[v]]`.
 * - Build a RMQ Sparse Table over the depth array. This gives $O(N \log N)$ build and $O(1)$ LCA queries, superior to binary lifting's $O(\log N)$ queries when query volume is massive.
 */
#include <vector>
#include <algorithm>

struct SparseTableLCA {
    int n;
    vector<int> euler;
    vector<int> depth;
    vector<int> first;
    vector<vector<int>> st;
    vector<int> log2_val;

    // Time Complexity: O(1)
    // Space Complexity: O(N log N)
    // Usage: SparseTableLCA lca(n);
    SparseTableLCA(int n_nodes) : n(n_nodes), first(n_nodes, -1) {}

    // Time Complexity: O(N log N)
    // Space Complexity: O(N log N)
    // Usage: lca.build(root, adj);
    void build(int root, const vector<vector<int>>& adj) {
        euler.clear();
        depth.clear();
        dfs(root, -1, 0, adj);
        int m = static_cast<int>(euler.size());
        log2_val.resize(m + 1);
        log2_val[1] = 0;
        for (int i = 2; i <= m; i++) {
            log2_val[i] = log2_val[i / 2] + 1;
        }
        int k = log2_val[m] + 1;
        st.assign(m, vector<int>(k));
        for (int i = 0; i < m; i++) {
            st[i][0] = i;
        }
        for (int j = 1; j < k; j++) {
            for (int i = 0; i + (1 << j) <= m; i++) {
                int left = st[i][j - 1];
                int right = st[i + (1 << (j - 1))][j - 1];
                if (depth[left] < depth[right]) {
                    st[i][j] = left;
                } else {
                    st[i][j] = right;
                }
            }
        }
    }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Usage: Internal DFS for Euler tour
    void dfs(int v, int p, int d, const vector<vector<int>>& adj) {
        size_t u = v;
        first[u] = static_cast<int>(euler.size());
        euler.push_back(v);
        depth.push_back(d);
        for (int to : adj[u]) {
            if (to != p) {
                dfs(to, v, d + 1, adj);
                euler.push_back(v);
                depth.push_back(d);
            }
        }
    }

    // Time Complexity: O(1)
    // Space Complexity: O(1)
    // Usage: int lca_node = lca.get_lca(u, v);
    int get_lca(int u, int v) {
        int left = first[u];
        int right = first[v];
        if (left > right) {
            swap(left, right);
        }
        int j = log2_val[right - left + 1];
        int idx1 = st[left][j];
        int idx2 = st[right - (1 << j) + 1][j];
        if (depth[idx1] < depth[idx2]) {
            return euler[idx1];
        } else {
            return euler[idx2];
        }
    }
};
