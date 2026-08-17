#include "../../core.h"
/*
Topic: Heavy-Light Decomposition
Rules & Heuristics: Decompose tree into paths to perform path queries/updates.
Problem Signatures: Path queries and updates on a tree structure.
Complexity: Path operations in O(log^2 N).
 *
 * # Heavy-Light Decomposition (HLD) Conceptuals
 *
 * HLD goes beyond a simple Euler tour by partitioning edges into "heavy" (to the child with the largest subtree) and "light" edges.
 * - Key Bound: Any path from root to a node passes through at most $O(\log N)$ light edges.
 * - Euler Tour synergy: By prioritizing heavy children during the DFS, each heavy path becomes a contiguous segment in the Euler tour array. This reduces path queries to $O(\log N)$ segment tree queries.
 */
#include <vector>
#include <algorithm>

struct HeavyLightDecomposition {
    int n;
    vector<int> parent;
    vector<int> depth;
    vector<int> head;
    vector<int> pos;
    vector<int> heavy;
    vector<int> sz;
    int current_pos;

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Usage: HeavyLightDecomposition hld(n);
    HeavyLightDecomposition(int n_nodes) : n(n_nodes),
                                           parent(n_nodes, -1),
                                           depth(n_nodes, 0),
                                           head(n_nodes, -1),
                                           pos(n_nodes, 0),
                                           heavy(n_nodes, -1),
                                           sz(n_nodes, 0),
                                           current_pos(0) {}

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Usage: hld.build(root, adj);
    void build(int root, const vector<vector<int>>& adj) {
        dfs_sz(root, -1, 0, adj);
        head[root] = root;
        dfs_hld(root, -1, adj);
    }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Usage: Internal DFS to compute subtree sizes and heavy children
    void dfs_sz(int v, int p, int d, const vector<vector<int>>& adj) {
        size_t u = v;
        sz[u] = 1;
        parent[u] = p;
        depth[u] = d;
        int max_sub_sz = 0;
        for (int to : adj[u]) {
            if (to != p) {
                dfs_sz(to, v, d + 1, adj);
                size_t uto = to;
                sz[u] += sz[uto];
                if (sz[uto] > max_sub_sz) {
                    max_sub_sz = sz[uto];
                    heavy[u] = to;
                }
            }
        }
    }

    // Time Complexity: O(N)
    // Space Complexity: O(N)
    // Usage: Internal DFS to assign HLD paths and positions
    void dfs_hld(int v, int p, const vector<vector<int>>& adj) {
        size_t u = v;
        pos[u] = current_pos++;
        if (heavy[u] != -1) {
            size_t uh = heavy[u];
            head[uh] = head[u];
            dfs_hld(heavy[u], v, adj);
        }
        for (int to : adj[u]) {
            if (to != p && to != heavy[u]) {
                size_t uto = to;
                head[uto] = to;
                dfs_hld(to, v, adj);
            }
        }
    }

    // Time Complexity: O(log N)
    // Space Complexity: O(1)
    // Usage: int lca = hld.get_lca(u, v);
    int get_lca(int u, int v) {
        while (head[u] != head[v]) {
            if (depth[head[u]] < depth[head[v]]) {
                swap(u, v);
            }
            u = parent[head[u]];
        }
        if (depth[u] > depth[v]) {
            swap(u, v);
        }
        return u;
    }
};
