// Test for 03_Trees/HLD.cpp against walking the tree naively (BFS parents, climb to the LCA).
// Every public method is checked: HLD (node and edge mode, any root, int and long long), update (node),
// update (edge), query, query_subtree, get_lca, queryPath, getChild; and the bundled Segment_Tree:
// both constructors, build (Base 0 and 1), update, query, query_Node, get.
#include "../../03_Trees/HLD.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }
using ll = long long;

vector<pair<int, int>> rand_tree(int n) {
    vector<int> perm(n);
    iota(perm.begin(), perm.end(), 1);
    shuffle(perm.begin(), perm.end(), rng);
    int shape = rng() % 4;
    vector<pair<int, int>> e;
    for (int i = 1; i < n; i++) {
        int p = shape == 0 ? i - 1 : shape == 1 ? 0 : (int)(rng() % i);
        e.push_back({perm[i], perm[p]});
    }
    return e;
}

template <typename T, int EDGE> void run_case(const string &tag) {
    int n = rng() % 15 + 1, root = rng() % n + 1;
    auto e = rand_tree(n);
    vector<vector<pair<T, T>>> adj(n + 1 + rng() % 3);
    for (auto [u, v] : e) adj[u].push_back({v, (T)(rng() % 9)}), adj[v].push_back({u, (T)(rng() % 9)});
    HLD<T, EDGE> h(n, adj, root);
    // naive rooting
    vector<int> par(n + 1, 0), dep(n + 1, 0), order = {root};
    for (int i = 0; i < (int)order.size(); i++)
        for (auto [v, w] : adj[order[i]])
            if (v != par[order[i]]) par[v] = order[i], dep[v] = dep[order[i]] + 1, order.push_back(v);
    auto lca = [&](int u, int v) {
        while (u != v)
            if (dep[u] >= dep[v]) u = par[u];
            else v = par[v];
        return u;
    };
    auto path = [&](int u, int v) { // nodes on the path, LCA included
        int l = lca(u, v);
        vector<int> p = {l};
        for (int x = u; x != l; x = par[x]) p.push_back(x);
        for (int x = v; x != l; x = par[x]) p.push_back(x);
        return p;
    };
    auto in_sub = [&](int u, int x) {
        for (; x; x = par[x])
            if (x == u) return true;
        return false;
    };
    vector<T> val(n + 1, 0); // node value, or the value of the edge (x, par[x]) in edge mode
    T big = sizeof(T) == 8 ? (T)1e15 : (T)1e5;
    for (int op = 0; op < 40; op++) {
        int t = rng() % 6, u = rng() % n + 1, v = rng() % n + 1;
        if (t == 0) {
            T x = (T)(rng() % (2 * (ll)big)) - big;
            if (!EDGE) h.update(u, x), val[u] = x, chk(tag + "update(node)", h.query(u, u) == x);
            else if (u != root) { // edge (u, par[u]), endpoints in random order
                int a = u, b = par[u];
                if (rng() % 2) swap(a, b);
                chk(tag + "getChild", h.getChild(a, b) == u);
                h.update(a, b, x), val[u] = x;
                chk(tag + "update(edge)", h.query(a, b) == x);
            }
        } else if (t == 1) {
            T want = 0;
            int l = lca(u, v);
            for (int x : path(u, v))
                if (!EDGE || x != l) want += val[x];
            chk(tag + "query", h.query(u, v) == want);
        } else if (t == 2) {
            T want = 0;
            for (int x = 1; x <= n; x++)
                if (in_sub(u, x) && (!EDGE || x != u)) want += val[x];
            chk(tag + "query_subtree", h.query_subtree(u) == want);
        } else if (t == 3) {
            chk(tag + "get_lca", h.get_lca(u, v) == lca(u, v));
        } else if (t == 4) { // the ranges cover exactly the path's positions, once each
            multiset<int> got, want;
            for (auto [l, r] : h.queryPath(u, v))
                for (int p = l; p <= r; p++) got.insert(p);
            int l = lca(u, v);
            for (int x : path(u, v))
                if (!EDGE || x != l) want.insert(h.pos[x]);
            chk(tag + "queryPath", got == want);
        } else {
            set<int> ps;
            for (int x = 1; x <= n; x++) ps.insert(h.pos[x]);
            chk(tag + "pos is a permutation", (int)ps.size() == n && *ps.begin() == 1 && *ps.rbegin() == n);
        }
    }
}

void seg_case() {
    int n = rng() % 20 + 1;
    vector<ll> a(n + 1);
    for (int i = 1; i <= n; i++) a[i] = (ll)(rng() % 2000) - 1000;
    Segment_Tree<ll, 0> s0(n, vector<ll>(a.begin() + 1, a.end())); // 0-based input
    Segment_Tree<ll, 1> s1(n);
    s1.build(a); // 1-based input, a[0] unused
    for (int op = 0; op < 30; op++) {
        int l = rng() % n + 1, r = rng() % n + 1;
        if (l > r) swap(l, r);
        if (rng() % 3 == 0) {
            ll x = (ll)(rng() % 2000) - 1000;
            s0.update(l, x), s1.update(l, x), a[l] = x;
            continue;
        }
        ll want = accumulate(a.begin() + l, a.begin() + r + 1, 0LL);
        chk("Segment_Tree<0>(n,nums)+query", s0.query(l, r) == want);
        chk("Segment_Tree<1>::build+query", s1.query(l, r) == want);
        chk("Segment_Tree::query_Node", s0.query_Node(l, r).val == want);
        chk("Segment_Tree::get", s0.get(l) == a[l] && s1.get(r) == a[r]);
    }
}

int main() {
    for (int it = 0; it < 6000; it++) {
        run_case<ll, 0>("node<ll>:");
        run_case<ll, 1>("edge<ll>:");
        run_case<int, 0>("node<int>:");
        run_case<int, 1>("edge<int>:");
        seg_case();
    }
    for (int shape = 0; shape < 2; shape++) {
        int n = 200000, q = 200000;
        vector<vector<pair<int, int>>> adj(n + 1);
        for (int i = 2; i <= n; i++) {
            int p = shape ? i - 1 : (int)(rng() % (i - 1)) + 1;
            adj[i].push_back({p, 0}), adj[p].push_back({i, 0});
        }
        ll sum = 0;
        vector<vector<pair<ll, ll>>> adjl(n + 1);
        for (int i = 1; i <= n; i++)
            for (auto [v, w] : adj[i]) adjl[i].push_back({v, w});
        auto t0 = clock();
        HLD<ll> H(n, adjl, 1);
        for (int i = 0; i < q; i++) {
            int t = rng() % 3, u = rng() % n + 1, v = rng() % n + 1;
            if (t == 0) H.update(u, (ll)(rng() % 1000000000));
            else if (t == 1) sum += H.query(u, v);
            else sum += H.query_subtree(u);
        }
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("%s n=q=2e5 (build + queries): %.3fs (sum=%lld)\n", shape ? "path" : "random", t, sum);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
