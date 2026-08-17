// Euler Tour: flattens a tree so that every subtree becomes one contiguous range of an array.
// Use when: "sum / min over the subtree of u" with point updates - pair it with any range structure.
// Handles: subtree ranges via start[u]..finish[u], point updates on nodes. Subtrees only, never paths.
// Time: build O(n), then whatever the range structure costs
// Indexing: 1-based nodes; start[u] and finish[u] are 1-based positions in the flattened array
// Note: this gives subtree ranges only. For path queries use HLD.cpp. build() is recursive.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct EulerTour {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> start, finish; // u's subtree is exactly the range [start[u], finish[u]]

    EulerTour(int n) : n(n), adj(n + 1), start(n + 1), finish(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void dfs(int u, int par) {
        start[u] = ++timer;
        for (int v : adj[u])
            if (v != par) dfs(v, u);
        finish[u] = timer;
    }

    void build(int root = 1) { dfs(root, -1); } // call once, after every add_edge

    vector<ll> flatten(const vector<ll> &val) { // val[u] laid out at position start[u]
        vector<ll> out(n + 1, 0);
        for (int u = 1; u <= n; u++) out[start[u]] = val[u];
        return out;
    }
};

struct SegTree {
    int size;
    vector<ll> tree;

    SegTree(int n) {
        size = 1;
        while (size < n) size *= 2;
        tree.assign(2 * size, 0);
    }

    void build(const vector<ll> &a) { // a is 1-based, a[1..n]
        for (int i = 1; i < (int)a.size(); i++) tree[size + i - 1] = a[i];
        for (int i = size - 1; i >= 1; i--) tree[i] = tree[2 * i] + tree[2 * i + 1];
    }

    void update(int pos, ll v) { // a[pos] = v
        int i = size + pos - 1;
        tree[i] = v;
        for (i >>= 1; i >= 1; i >>= 1) tree[i] = tree[2 * i] + tree[2 * i + 1];
    }

    ll query(int l, int r) { // sum over a[l .. r]
        ll res = 0;
        for (int lo = l + size - 1, hi = r + size - 1; lo <= hi; lo >>= 1, hi >>= 1) {
            if (lo & 1) res += tree[lo++];
            if (!(hi & 1)) res += tree[hi--];
        }
        return res;
    }
};

// Standard problem: type 1 sets a node's value, type 2 asks the sum over that node's whole subtree
void solve() {
    int n, q;
    cin >> n >> q;
    vector<ll> val(n + 1);
    for (int i = 1; i <= n; i++) cin >> val[i];
    EulerTour et(n);
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        et.add_edge(u, v);
    }
    et.build(1);

    SegTree seg(n);
    seg.build(et.flatten(val));
    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int s;
            ll x;
            cin >> s >> x;
            seg.update(et.start[s], x);
        } else {
            int s;
            cin >> s;
            cout << seg.query(et.start[s], et.finish[s]) << '\n';
        }
    }
}
