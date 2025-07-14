#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define ll long long
using namespace std;
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

    Segment_Tree(int n, const vector<T> &nums) {
        size = 1, DEFAULT = 0;
        while (size < n) size *= 2;
        tree = vector<Node>(2 * size, DEFAULT);
        build(nums);
    }

    // Main operation to do

    Node operation(const Node &a, const Node &b) { return a.val + b.val; }

    // If Base is 1 so the array is 1-based else the array is 0-based

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

    void update(const int index, const T v) { update(index, v, 1, 1, size); }

    Node query(int l, int r, int idx, int lx, int rx) {
        if (lx > r || l > rx) return DEFAULT;
        if (lx >= l && rx <= r) return tree[idx];
        int mx = (lx + rx) / 2;
        return operation(query(l, r, 2 * idx, lx, mx), query(l, r, 2 * idx + 1, mx + 1, rx));
    }

    Node query_Node(const int l, const int r) { return query(l, r, 1, 1, size); }

    T query(const int l, const int r) { return query_Node(l, r).val; }

    T get(const int idx) { return query_Node(idx, idx).val; }

    friend ostream &operator<<(ostream &out, const Node &node) {
        out << node.val << ' ';
        return out;
    }
};

template <typename T = int, const int VAL_ON_EDGE = 0> struct HLD {
    // VAL_ON_EDGE = 0 if value on nodes
    // VAL_ON_EDGE = 1 if value on edges

    const vector<vector<pair<T, T>>> &adj;
    vector<T> dep, par, root, pos, SubtreeSz, child;
    int nxtPos;
    Segment_Tree<T> seg;

    HLD(int n, const vector<vector<pair<T, T>>> &G, int treeRoot = 1) : adj(G) {
        nxtPos = 1;
        // child is heavy child
        dep = par = root = pos = SubtreeSz = child = vector<T>(n + 5);
        init(treeRoot);
        build(treeRoot);
        seg = Segment_Tree<T>(n + 5);
    }

    // init the tree
    void init(int u, int p = -1, int d = 0) {
        dep[u] = d, par[u] = p, SubtreeSz[u] = 1;
        for (auto [v, _] : adj[u]) {
            if (v == p) continue;
            init(v, u, d + 1);
            SubtreeSz[u] += SubtreeSz[v];
            if (SubtreeSz[v] > SubtreeSz[child[u]]) child[u] = v;
        }
    }

    // get the root of the chain
    int get_lca(int u, int v) {
        while (root[u] != root[v]) {
            if (dep[root[u]] < dep[root[v]]) swap(u, v);
            u = par[root[u]];
        }
        return dep[u] < dep[v] ? u : v;
    }

    // build the chains
    void build(int u, bool newChain = true) {
        root[u] = newChain ? u : root[par[u]];
        pos[u] = nxtPos++;
        if (child[u]) build(child[u], false);
        for (auto [v, _] : adj[u]) {
            if (v == par[u] || v == child[u]) continue;
            build(v, true);
        }
    }

    // make u lower
    void makeULower(int &u, int &v) {
        if (dep[root[u]] < dep[root[v]] || (root[u] == root[v] && dep[u] < dep[v])) swap(u, v);
    }

    // move up the chain and also change the next position
    pair<T, T> moveUp(int &u) {
        pair<T, T> ret = {pos[root[u]], pos[u]};
        u = par[root[u]];
        return ret;
    }

    vector<pair<T, T>> queryPath(int u, int v) {
        // return all ranges in segment tree
        vector<pair<T, T>> ret;
        while (root[u] != root[v]) {
            makeULower(u, v);
            ret.push_back(moveUp(u));
        }
        // add range between u and v
        makeULower(u, v);
        if (!VAL_ON_EDGE) // value on nodes
            ret.push_back({pos[v], pos[u]});
        else if (u != v) // don't include the root node
            ret.push_back({pos[v] + 1, pos[u]});
        return ret;
    }

    // get child of a node for problem that value on edges
    int getChild(int u, int v) {
        if (par[u] == v) return u;
        return v;
    }

    // update value of node u
    void update(int u, T val) { seg.update(pos[u], val); }

    // update value of edge u-v
    void update(int u, int v, T val) {
        u = getChild(u, v);
        seg.update(pos[u], val);
    }

    // query value
    T query(int u_q, int v_q) {
        T ret = 0;
        for (auto &[u, v] : queryPath(u_q, v_q)) ret += seg.query(u, v);
        return ret;
    }
};