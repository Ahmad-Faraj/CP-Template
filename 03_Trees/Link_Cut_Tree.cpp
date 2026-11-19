// Link Cut Tree: a rooted forest under link / cut / re-root, with path and subtree sums.
// Use when: edges appear and vanish online and you need connectivity, LCA, path or subtree sums.
// Usage: LCT t(n); t.upd(u, val); t.link(p, u); t.cut(u); t.query(u, v); t.subtree_sum(u);
// Handles: make_root, find_root, get_parent, depth, lca, path add, subtree sum under any root.
// Time: every operation O(log n) amortized
// Indexing: 1-based nodes, all start isolated with value 0; node 0 is the null sentinel
// Note: link needs 2 trees, cut(u) a non-root u, lca connected nodes. Queries keep the root.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct LCT {
    struct node {
        int p = 0, c[2] = {0, 0}, pp = 0;
        bool flip = 0;
        int sz = 0, ssz = 0, vsz = 0;
        ll val = 0, sum = 0, lazy = 0, subsum = 0, vsum = 0;
        node() {}
        node(ll x) : sz(1), ssz(1), val(x), sum(x), subsum(x) {}
    };
    vector<node> t;
    LCT(int n) : t(n + 1, node(0)) { t[0] = node(); }

    int dir(int x, int y) { return t[x].c[1] == y; }
    void set(int x, int d, int y) {
        if (x) t[x].c[d] = y, pull(x);
        if (y) t[y].p = x;
    }
    void pull(int x) {
        if (!x) return;
        int &l = t[x].c[0], &r = t[x].c[1];
        push(l), push(r);
        t[x].sum = t[l].sum + t[r].sum + t[x].val;
        t[x].sz = t[l].sz + t[r].sz + 1;
        t[x].ssz = t[l].ssz + t[r].ssz + t[x].vsz + 1;
        t[x].subsum = t[l].subsum + t[r].subsum + t[x].vsum + t[x].val;
    }
    void push(int x) {
        if (!x) return;
        int &l = t[x].c[0], &r = t[x].c[1];
        if (t[x].flip) {
            swap(l, r);
            if (l) t[l].flip ^= 1;
            if (r) t[r].flip ^= 1;
            t[x].flip = 0;
        }
        if (t[x].lazy) {
            t[x].val += t[x].lazy;
            t[x].sum += t[x].lazy * t[x].sz;
            t[x].subsum += t[x].lazy * t[x].sz;
            if (l) t[l].lazy += t[x].lazy;
            if (r) t[r].lazy += t[x].lazy;
            t[x].lazy = 0;
        }
    }
    void rotate(int x, int d) {
        int y = t[x].p, z = t[y].p, w = t[x].c[d];
        swap(t[x].pp, t[y].pp);
        set(y, !d, w);
        set(x, d, y);
        set(z, dir(z, y), x);
    }
    void splay(int x) {
        for (push(x); t[x].p;) {
            int y = t[x].p, z = t[y].p;
            push(z), push(y), push(x);
            int dx = dir(y, x), dy = dir(z, y);
            if (!z) rotate(x, !dx);
            else if (dx == dy) rotate(y, !dx), rotate(x, !dx);
            else rotate(x, dy), rotate(x, dx);
        }
    }

    void make_root(int u) { // u becomes the root of its tree
        access(u);
        int l = t[u].c[0];
        t[l].flip ^= 1;
        swap(t[l].p, t[l].pp);
        t[u].vsz += t[l].ssz;
        t[u].vsum += t[l].subsum;
        set(u, 0, 0);
    }
    int access(int _u) {
        int last = _u;
        for (int v = 0, u = _u; u; u = t[v = u].pp) {
            splay(u), splay(v);
            t[u].vsz -= t[v].ssz;
            t[u].vsum -= t[v].subsum;
            int r = t[u].c[1];
            push(r);
            t[u].vsz += t[r].ssz;
            t[u].vsum += t[r].subsum;
            t[v].pp = 0;
            swap(t[r].p, t[r].pp);
            set(u, 1, v);
            last = u;
        }
        splay(_u);
        return last;
    }
    void link(int u, int v) { // edge u -> v: v (re-rooted) becomes a child of u
        make_root(v);
        access(u), splay(u);
        t[v].pp = u;
        t[u].vsz += t[v].ssz;
        t[u].vsum += t[v].subsum;
    }
    void cut(int u) { // remove the edge parent(u) -> u
        access(u);
        assert(t[u].c[0] != 0);
        t[t[u].c[0]].p = 0;
        t[u].c[0] = 0;
        pull(u);
    }
    int get_parent(int u) { // 0 if u is a root
        access(u), splay(u), push(u);
        u = t[u].c[0], push(u);
        while (t[u].c[1]) u = t[u].c[1], push(u);
        splay(u);
        return u;
    }
    int find_root(int u) {
        access(u), splay(u), push(u);
        while (t[u].c[0]) u = t[u].c[0], push(u);
        splay(u);
        return u;
    }
    bool connected(int u, int v) { return find_root(u) == find_root(v); }
    int depth(int u) { // nodes on the path root..u
        access(u), splay(u);
        return t[u].sz;
    }
    int lca(int u, int v) {
        if (u == v) return u;
        if (depth(u) > depth(v)) swap(u, v);
        access(v);
        return access(u);
    }
    bool is_root(int u) { return get_parent(u) == 0; }
    int component_size(int u) { return t[find_root(u)].ssz; }
    int subtree_size(int u) {
        int p = get_parent(u);
        if (p == 0) return component_size(u);
        cut(u);
        int ans = component_size(u);
        link(p, u);
        return ans;
    }
    ll component_sum(int u) { return t[find_root(u)].subsum; }
    ll subtree_sum(int u) {
        int p = get_parent(u);
        if (p == 0) return component_sum(u);
        cut(u);
        ll ans = component_sum(u);
        link(p, u);
        return ans;
    }
    ll subtree_query(int u, int root) { // subtree sum of u if the tree were rooted at root
        int cur = find_root(u);
        make_root(root);
        ll ans = subtree_sum(u);
        make_root(cur);
        return ans;
    }
    ll query(int u, int v) { // sum of values on the path u..v
        int cur = find_root(u);
        make_root(u), access(v);
        ll ans = t[v].sum;
        make_root(cur);
        return ans;
    }
    void upd(int u, ll x) { // value[u] += x
        access(u), splay(u);
        t[u].val += x, pull(u);
    }
    void upd(int u, int v, ll x) { // value += x on every node of the path u..v
        int cur = find_root(u);
        make_root(u), access(v);
        t[v].lazy += x;
        make_root(cur);
    }
};
