// Link-Cut Tree: a forest you can link and cut, with path and subtree queries, in O(log n) amortized.
// Use when: edges appear and vanish online and you still need path sums, connectivity, LCA or subtree sums.
// Handles: link, cut, connectivity, LCA, path sum, path add, point add, subtree size and sum, re-rooting.
// Time: every operation O(log n) amortized
// Indexing: 1-based nodes; node 0 is the null sentinel and must never be touched
// Note: call init(u, value) for every node before any link. cut(u) removes the edge above u, so u must not be a root.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct node {
    int p = 0, c[2] = {0, 0}, pp = 0;
    bool flip = 0;
    int sz = 0, ssz = 0, vsz = 0; // splay size, represented-tree subtree size, virtual subtree size
    ll val = 0, sum = 0, lazy = 0, subsum = 0, vsum = 0;

    node() {}

    node(ll x) : sz(1), ssz(1), vsz(0), val(x), sum(x), lazy(0), subsum(x), vsum(0) {}
};

struct LCT {
    vector<node> t;

    LCT() {}

    LCT(int n) : t(n + 1) {}

    void init(int u, ll val) { t[u] = node(val); } // set u's value; do this before linking

    int dir(int x, int y) { return t[x].c[1] == y; }

    void set(int x, int d, int y) {
        if (x) t[x].c[d] = y, pull(x);
        if (y) t[y].p = x;
    }

    void pull(int x) {
        if (!x) return;
        int &l = t[x].c[0], &r = t[x].c[1];
        push(l);
        push(r);
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
            t[x].subsum += t[x].lazy * t[x].ssz;
            t[x].vsum += t[x].lazy * t[x].vsz;
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
            push(z);
            push(y);
            push(x);
            int dx = dir(y, x), dy = dir(z, y);
            if (!z)
                rotate(x, !dx);
            else if (dx == dy)
                rotate(y, !dx), rotate(x, !dx);
            else
                rotate(x, dy), rotate(x, dx);
        }
    }

    void make_root(int u) { // re-root the represented tree at u
        access(u);
        int l = t[u].c[0];
        t[l].flip ^= 1;
        swap(t[l].p, t[l].pp);
        t[u].vsz += t[l].ssz;
        t[u].vsum += t[l].subsum;
        set(u, 0, 0);
    }

    int access(int _u) { // returns the last path-parent seen on the way up
        int last = _u;
        for (int v = 0, u = _u; u; u = t[v = u].pp) {
            splay(u);
            splay(v);
            t[u].vsz -= t[v].ssz;
            t[u].vsum -= t[v].subsum;
            int r = t[u].c[1];
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

    void link(int u, int v) { // make u the parent of v's tree
        make_root(v);
        access(u);
        splay(u);
        t[v].pp = u;
        t[u].vsz += t[v].ssz;
        t[u].vsum += t[v].subsum;
    }

    void cut(int u) { // remove the edge between u and its parent; u must not be a root
        access(u);
        assert(t[u].c[0] != 0);
        t[t[u].c[0]].p = 0;
        t[u].c[0] = 0;
        pull(u);
    }

    int get_parent(int u) { // u's parent, or 0 if u is a root
        access(u);
        splay(u);
        push(u);
        u = t[u].c[0];
        push(u);
        while (t[u].c[1]) {
            u = t[u].c[1];
            push(u);
        }
        splay(u);
        return u;
    }

    int find_root(int u) { // root of u's component
        access(u);
        splay(u);
        push(u);
        while (t[u].c[0]) {
            u = t[u].c[0];
            push(u);
        }
        splay(u);
        return u;
    }

    bool connected(int u, int v) { return find_root(u) == find_root(v); }

    int depth(int u) { // number of nodes from the root down to u
        access(u);
        splay(u);
        return t[u].sz;
    }

    int lca(int u, int v) { // u and v must already be connected
        if (u == v) return u;
        if (depth(u) > depth(v)) swap(u, v);
        access(v);
        return access(u);
    }

    int is_root(int u) { return get_parent(u) == 0; }

    int component_size(int u) { return t[find_root(u)].ssz; }

    int subtree_size(int u) { // size of u's subtree under the current root
        int p = get_parent(u);
        if (p == 0) return component_size(u);
        cut(u);
        int ans = component_size(u);
        link(p, u);
        return ans;
    }

    ll component_sum(int u) { return t[find_root(u)].subsum; }

    ll subtree_sum(int u) { // sum over u's subtree under the current root
        int p = get_parent(u);
        if (p == 0) return component_sum(u);
        cut(u);
        ll ans = component_sum(u);
        link(p, u);
        return ans;
    }

    ll subtree_query(int u, int root) { // sum over u's subtree when the tree is rooted at root
        int cur = find_root(u);
        make_root(root);
        ll ans = subtree_sum(u);
        make_root(cur);
        return ans;
    }

    ll query(int u, int v) { // sum of the values on the path u .. v
        int cur = find_root(u);
        make_root(u);
        access(v);
        ll ans = t[v].sum;
        make_root(cur);
        return ans;
    }

    void upd(int u, ll x) { // a[u] += x
        access(u);
        splay(u);
        t[u].val += x;
        pull(u); // without this the node's aggregates stay stale
    }

    void upd(int u, int v, ll x) { // add x to every node on the path u .. v
        int cur = find_root(u);
        make_root(u);
        access(v);
        t[v].lazy += x;
        make_root(cur);
    }
};

// Standard problem: 1 links, 2 cuts, 3 adds on a path, 4 asks a path sum, 5 asks whether two nodes are connected
void solve() {
    int n, q;
    cin >> n >> q;
    LCT lct(n);
    for (int i = 1; i <= n; i++) {
        ll v;
        cin >> v;
        lct.init(i, v);
    }
    while (q--) {
        int type, u, v;
        cin >> type >> u;
        if (type == 1) {
            cin >> v;
            lct.link(u, v);
        } else if (type == 2) {
            lct.cut(u);
        } else if (type == 3) {
            ll x;
            cin >> v >> x;
            lct.upd(u, v, x);
        } else if (type == 4) {
            cin >> v;
            cout << lct.query(u, v) << '\n';
        } else {
            cin >> v;
            cout << (lct.connected(u, v) ? "YES" : "NO") << '\n';
        }
    }
}
