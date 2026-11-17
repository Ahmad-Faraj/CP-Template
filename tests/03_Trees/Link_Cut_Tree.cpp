// Test for 03_Trees/Link_Cut_Tree.cpp against a naive rooted forest (parent array, re-root by
// reversing the root path, sums by walking). Every public operation is checked after random
// sequences: link, cut, make_root, find_root, connected, get_parent, is_root, depth, lca,
// component_size, subtree_size, component_sum, subtree_sum, subtree_query, query, upd (both).
// After each operation the whole represented forest (every parent) is compared too.
#include "../../03_Trees/Link_Cut_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(7);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct Naive {
    int n;
    vector<int> par;
    vector<ll> val;
    Naive(int n) : n(n), par(n + 1, 0), val(n + 1, 0) {}
    int root(int u) { while (par[u]) u = par[u]; return u; }
    void make_root(int u) { // reverse the path u..root
        int prev = 0;
        while (u) {
            int nx = par[u];
            par[u] = prev, prev = u, u = nx;
        }
    }
    vector<int> up(int u) { vector<int> p; for (; u; u = par[u]) p.push_back(u); return p; }
    int depth(int u) { return (int)up(u).size(); }
    int lca(int u, int v) {
        auto a = up(u);
        set<int> s(a.begin(), a.end());
        for (; v; v = par[v])
            if (s.count(v)) return v;
        return 0;
    }
    vector<int> path(int u, int v) {
        int w = lca(u, v);
        vector<int> p;
        for (int x = u; x != w; x = par[x]) p.push_back(x);
        p.push_back(w);
        for (int x = v; x != w; x = par[x]) p.push_back(x);
        return p;
    }
    bool in_sub(int x, int u) { for (; x; x = par[x]) if (x == u) return true; return false; }
    pair<int, ll> sub(int u) { // size and sum of u's subtree
        int c = 0;
        ll s = 0;
        for (int x = 1; x <= n; x++)
            if (in_sub(x, u)) c++, s += val[x];
        return {c, s};
    }
};

int main() {
    for (int it = 0; it < 4000; it++) {
        int n = rng() % 9 + 1;
        LCT t(n);
        Naive b(n);
        for (int u = 1; u <= n; u++) {
            ll x = (ll)(rng() % 41) - 20;
            if (rng() % 2) t.upd(u, x), b.val[u] += x;
        }
        for (int op = 0; op < 120; op++) {
            int u = rng() % n + 1, v = rng() % n + 1, ty = rng() % 17;
            if (ty <= 2) { // link (only between different trees)
                if (b.root(u) == b.root(v)) continue;
                t.link(u, v);
                b.make_root(v), b.par[v] = u;
                chk("link", true);
            } else if (ty == 3) {
                if (!b.par[u]) continue;
                t.cut(u);
                b.par[u] = 0;
                chk("cut", true);
            } else if (ty == 4) {
                t.make_root(u);
                b.make_root(u);
                chk("make_root", true);
            } else if (ty == 5) {
                chk("find_root", t.find_root(u) == b.root(u));
                chk("connected", t.connected(u, v) == (b.root(u) == b.root(v)));
            } else if (ty == 6) {
                chk("get_parent", t.get_parent(u) == b.par[u]);
                chk("is_root", t.is_root(v) == (b.par[v] == 0));
            } else if (ty == 7) {
                chk("depth", t.depth(u) == b.depth(u));
            } else if (ty == 8) {
                if (b.root(u) != b.root(v)) continue;
                chk("lca", t.lca(u, v) == b.lca(u, v));
            } else if (ty == 9) {
                chk("component_size", t.component_size(u) == b.sub(b.root(u)).first);
                chk("component_sum", t.component_sum(v) == b.sub(b.root(v)).second);
            } else if (ty == 10) {
                chk("subtree_size", t.subtree_size(u) == b.sub(u).first);
            } else if (ty == 11) {
                chk("subtree_sum", t.subtree_sum(u) == b.sub(u).second);
            } else if (ty == 12) {
                if (b.root(u) != b.root(v)) continue;
                int cur = b.root(u);
                b.make_root(v);
                ll want = b.sub(u).second;
                b.make_root(cur);
                chk("subtree_query", t.subtree_query(u, v) == want);
            } else if (ty == 13) {
                if (b.root(u) != b.root(v)) continue;
                ll want = 0;
                for (int x : b.path(u, v)) want += b.val[x];
                chk("query", t.query(u, v) == want);
            } else if (ty == 14) {
                ll x = (ll)(rng() % 41) - 20;
                t.upd(u, x), b.val[u] += x;
                chk("upd(u,x)", true);
            } else {
                if (b.root(u) != b.root(v)) continue;
                ll x = (ll)(rng() % 41) - 20;
                t.upd(u, v, x);
                for (int y : b.path(u, v)) b.val[y] += x;
                chk("upd(u,v,x)", true);
            }
            if (op % 10 == 0) { // full structure and value check
                bool ok = true;
                for (int x = 1; x <= n; x++) ok &= t.get_parent(x) == b.par[x];
                chk("forest shape", ok);
                ok = true;
                for (int x = 1; x <= n; x++) ok &= t.query(x, x) == b.val[x];
                chk("values (query(x,x))", ok);
            }
        }
    }
    { // big values: sums past 2^31
        LCT t(3);
        t.upd(1, (ll)2e12), t.upd(2, (ll)3e12), t.link(1, 2), t.link(2, 3);
        t.upd(1, 3, (ll)1e12);
        chk("query", t.query(1, 3) == (ll)8e12);
        chk("subtree_sum", t.subtree_sum(2) == (ll)5e12);
    }
    { // timing: n = 2e5, 4e5 random link / cut / query
        int n = 200000;
        LCT t(n);
        vector<int> par(n + 1, 0);
        for (int u = 2; u <= n; u++) par[u] = rng() % (u - 1) + 1, t.link(par[u], u), t.upd(u, u);
        auto t0 = clock();
        ll acc = 0;
        for (int q = 0; q < 400000; q++) {
            int u = rng() % (n - 1) + 2, ty = rng() % 4;
            if (ty == 0) { // cut u, then hang it under a node outside its subtree
                t.cut(u);
                int w = rng() % n + 1;
                if (t.connected(u, w)) w = 1;
                t.link(w, u);
            } else if (ty == 1) acc += t.query(u, rng() % n + 1);
            else if (ty == 2) acc += t.subtree_sum(u);
            else t.upd(u, rng() % n + 1, 1);
        }
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5, 4e5 mixed ops: %.3fs (acc=%lld)\n", sec, acc);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-22s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
