// HLD: path and subtree queries on a tree, by flattening it onto one array backed by a segment tree.
// Use when: "sum on the path u..v" with point updates, on a tree whose shape never changes.
// Handles: values on nodes (default) or on edges via HLD<int, 1>. Path query, point update, LCA.
// Time: build O(n) | query O(log^2 n) | update O(log n)
// Indexing: 1-based nodes; the adjacency is vector<vector<pair<neighbour, weight>>>
// Note: operation() and query()'s accumulator are both hardcoded to sum - change the two together.

#include <bits/stdc++.h>
using namespace std;
#define sz(x) (int)(x).size()

template <typename T = int, int Base = 0> struct Segment_Tree {
    struct Node {
        T val;
        Node(T V = 0) : val(V) {}
        Node operator=(const T rhs) {
            val = rhs;
            return *this;
        }
    };

    int size;
    Node DEFAULT;
    vector<Node> tree;

    Segment_Tree(int n = 0) {
        size = 1, DEFAULT = 0;
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
    }

    Node operation(const Node &a, const Node &b) { return a.val + b.val; }

    void build(const vector<T> &nums, int idx, int lx, int rx) {
        if (Base ? lx >= sz(nums) : lx > sz(nums)) return;
        if (rx == lx)
            tree[idx] = nums[lx - !Base];
        else {
            int mx = (rx + lx) / 2;
            build(nums, 2 * idx, lx, mx);
            build(nums, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void build(const vector<T> &nums) { build(nums, 1, 1, size); }

    void update(int index, T v, int idx, int lx, int rx) {
        if (rx == lx)
            tree[idx] = v;
        else {
            int mx = (rx + lx) / 2;
            if (index <= mx)
                update(index, v, 2 * idx, lx, mx);
            else
                update(index, v, 2 * idx + 1, mx + 1, rx);
            tree[idx] = operation(tree[2 * idx], tree[2 * idx + 1]);
        }
    }

    void update(int index, T v) { update(index, v, 1, 1, size); }

    Node query(int l, int r, int idx, int lx, int rx) {
        if (lx > r || l > rx) return DEFAULT;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        return operation(query(l, r, 2 * idx, lx, mx), query(l, r, 2 * idx + 1, mx + 1, rx));
    }

    T query(int l, int r) { return query(l, r, 1, 1, size).val; }
    T get(int idx) { return query(idx, idx); }
};

template <typename T = int, const int VAL_ON_EDGE = 0> struct HLD {
    vector<vector<pair<T, T>>> adj; // held by value: a reference member dangles if the caller's graph dies
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;
    Segment_Tree<T> seg;

    HLD(int n, const vector<vector<pair<T, T>>> &G, int treeRoot = 1) : adj(G) {
        nxtPos = 1;
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
        seg = Segment_Tree<T>(n + 5);
    }

    void init(int u, int p = -1, int d = 0) {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto [v, w] : adj[u]) {
            (void)w;
            if (v == p) continue;
            init(v, u, d + 1);
            SubtreeSz[u] += SubtreeSz[v];
            if (SubtreeSz[v] > SubtreeSz[child[u]]) child[u] = v;
        }
    }

    void build(int u, bool newChain = true) { // lay the chains out into consecutive array positions
        root[u] = newChain ? u : root[par[u]];
        pos[u] = nxtPos++;
        if (child[u]) build(child[u], false);
        for (auto [v, w] : adj[u]) {
            (void)w;
            if (v == par[u] || v == child[u]) continue;
            build(v, true);
        }
    }

    int get_lca(int u, int v) {
        while (root[u] != root[v]) {
            if (dep[root[u]] < dep[root[v]]) swap(u, v);
            u = par[root[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    void makeULower(int &u, int &v) {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v])) swap(u, v);
    }

    pair<T, T> moveUp(int &u) {
        pair<T, T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    vector<pair<T, T>> queryPath(int u, int v) { // the path u..v as segment-tree ranges
        vector<pair<T, T>> ret;
        while (root[u] != root[v]) {
            makeULower(u, v);
            ret.push_back(moveUp(u));
        }
        makeULower(u, v);
        if (!VAL_ON_EDGE)
            ret.push_back({pos[v], pos[u]});
        else if (u != v)
            ret.push_back({pos[v] + 1, pos[u]}); // skip the LCA, which owns no edge on this path
        return ret;
    }

    int getChild(int u, int v) { return par[u] == v ? u : v; } // the lower endpoint of edge u-v

    void update(int u, T val) { seg.update(pos[u], val); } // set node u's value

    void update(int u, int v, T val) { seg.update(pos[getChild(u, v)], val); } // set edge u-v's value

    T query(int u_q, int v_q) { // aggregate over the path u_q .. v_q
        T ret = 0;
        for (auto &[u, v] : queryPath(u_q, v_q)) ret += seg.query(u, v);
        return ret;
    }
};

// Standard problem: values on nodes; type 1 sets a node, type 2 asks the sum on the path u..v
void solve() {
    int n, q;
    cin >> n >> q;
    vector<vector<pair<int, int>>> g(n + 5);
    vector<int> val(n + 1);
    for (int i = 1; i <= n; i++) cin >> val[i];
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back({v, 0}), g[v].push_back({u, 0});
    }
    HLD<int> hld(n, g);
    for (int i = 1; i <= n; i++) hld.update(i, val[i]);
    while (q--) {
        int type, u, v;
        cin >> type >> u >> v;
        if (type == 1)
            hld.update(u, v);
        else
            cout << hld.query(u, v) << '\n';
    }
}
