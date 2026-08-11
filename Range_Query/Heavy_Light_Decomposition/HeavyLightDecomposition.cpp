/*
    [1] Definition:
    Heavy-Light Decomposition (HLD) splits a tree into straight paths (chains).
    It converts tree path queries into 1D array ranges for Segment Trees or Fenwick Trees.

    [2] Time & Space Complexity:
    - Build Time: O(N)
    - Space: O(N)
    - Path Query / LCA: O(log N) chain jumps

    [3] Important Notes:
    - Uses 1-based indexing.
    - VAL_ON_EDGE = 0 for node values, 1 for edge values.
    - pos[u] maps node u to its 1D position.
*/

template <typename T = int, const int VAL_ON_EDGE = 0>
struct HLD {
    // 0 = values on nodes | 1 = values on edges

    const vector<vector<T>>& adj;
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;

    HLD(int n, const vector<vector<T>>& G, int treeRoot = 1) : adj(G) {
        nxtPos = 1;
        // Stores heavy child for each node
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
    }

    // Calculates depth, parent, subtree size, and heavy child
    void init(int u, int p = -1, int d = 0) {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto v : adj[u]) {
            if (v == p) continue;
            init(v, u, d + 1);
            SubtreeSz[u] += SubtreeSz[v];
            if (SubtreeSz[v] > SubtreeSz[child[u]]) child[u] = v;
        }
    }

    // Finds Lowest Common Ancestor (LCA) of u and v
    int get_lca(int u, int v) {
        while (root[u] != root[v]) {
            if (dep[root[u]] < dep[root[v]]) swap(u, v);
            u = par[root[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // Assigns chain roots and flattens tree into 1D positions
    void build(int u, bool newChain = true) {
        root[u] = newChain ? u : root[par[u]];
        pos[u] = nxtPos++;
        if (child[u]) build(child[u], false);
        for (auto v : adj[u]) {
            if (v == par[u] || v == child[u]) continue;
            build(v, true);
        }
    }

    // Ensures u is deeper in the tree than v
    void makeULower(int& u, int& v) {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v])) swap(u, v);
    }

    // Moves u up to the parent of its chain root
    pair<T, T> moveUp(int& u) {
        pair<T, T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    // Returns 1D array ranges [L, R] covering path from u to v
    vector<pair<T, T>> queryPath(int u, int v) {
        vector<pair<T, T>> ret;
        while (root[u] != root[v]) {
            makeULower(u, v);
            ret.push_back(moveUp(u));
        }
        makeULower(u, v);
        if (!VAL_ON_EDGE)  // Include LCA node
            ret.push_back({pos[v], pos[u]});
        else if (u != v)  // Skip LCA node for edge values
            ret.push_back({pos[v] + 1, pos[u]});
        return ret;
    }

    // Returns the deeper node representing edge (u, v)
    int getChild(int u, int v) {
        if (par[u] == v) return u;
        return v;
    }
};