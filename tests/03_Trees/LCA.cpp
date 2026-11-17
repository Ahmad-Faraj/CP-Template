// Independent random test for 03_Trees/LCA.cpp (struct LCA, binary lifting).
// Oracle: BFS from the root gives parent and depth; LCA and k-th ancestor by walking parents one step at a time;
// distance = number of edges on the BFS-tree path.
// Build: g++ -std=c++17 -O1 -g -fsanitize=address,undefined LCA.cpp   (add -DINT_LL for #define int long long)
#include <bits/stdc++.h>
using namespace std;
#ifdef INT_LL
#define int long long
#endif
#include "../../03_Trees/LCA.cpp"

static mt19937 rng(2026);
static int rnd(int a, int b) { return uniform_int_distribution<long long>(a, b)(rng); }
static long long fails = 0;
static map<string, long long> cov;
#define CHECK(name, cond)                                                                    \
    do {                                                                                     \
        cov[name]++;                                                                         \
        if (!(cond)) {                                                                       \
            if (++fails <= 10) cerr << "MISMATCH " << name << " line " << __LINE__ << '\n'; \
        }                                                                                    \
    } while (0)

// shapes: 0 random recursive tree, 1 single node, 2 path, 3 star, 4 caterpillar, 5 binary-ish, 6 deep random (parent near i)
static vector<pair<int, int>> gen(int &n, int shape) {
    vector<pair<int, int>> e;
    if (shape == 1) n = 1;
    for (int i = 2; i <= n; i++) {
        int p = 1;
        if (shape == 0) p = rnd(1, i - 1);
        if (shape == 2) p = i - 1;
        if (shape == 3) p = 1;
        if (shape == 4) p = (i % 2 == 0 || i < 3) ? max<int>(1, i - 2) : i - 1;
        if (shape == 5) p = i / 2;
        if (shape == 6) p = rnd(max<int>(1, i - 3), i - 1);
        e.push_back({p, i});
    }
    vector<int> perm(n); iota(perm.begin(), perm.end(), 1); shuffle(perm.begin(), perm.end(), rng);
    for (auto &[u, v] : e) { u = perm[u - 1], v = perm[v - 1]; if (rnd(0, 1)) swap(u, v); }
    shuffle(e.begin(), e.end(), rng);
    return e;
}

static void one(int n, const vector<pair<int, int>> &e, int root, bool via_dfs) {
    LCA t(n);
    for (auto [u, v] : e) t.add_edge(u, v);
    cov["add_edge"] += e.size();
    if (via_dfs) t.dfs(root), cov["dfs(root) direct"]++;
    else t.build(root), cov["build(root)"]++;

    vector<vector<int>> g(n + 1);
    for (auto [u, v] : e) g[u].push_back(v), g[v].push_back(u);
    vector<int> par(n + 1, 0), dep(n + 1, -1);
    queue<int> q; q.push(root); dep[root] = 0;
    while (!q.empty()) { int u = q.front(); q.pop(); for (int v : g[u]) if (dep[v] < 0) dep[v] = dep[u] + 1, par[v] = u, q.push(v); }
    auto up = [&](int u, int k) -> int { if (k > dep[u]) return -1; while (k--) u = par[u]; return u; };
    auto lca = [&](int u, int v) { while (dep[u] > dep[v]) u = par[u]; while (dep[v] > dep[u]) v = par[v]; while (u != v) u = par[u], v = par[v]; return u; };

    for (int u = 1; u <= n; u++) {
        CHECK("dep member", t.dep[u] == dep[u]);
        for (int k = 0; k <= dep[u] + 2; k++) CHECK(string(k > dep[u] ? "kth_ancestor (k>depth -> -1)" : "kth_ancestor"),t.kth_ancestor(u, k) == up(u, k));
        CHECK("kth_ancestor (huge k -> -1)", t.kth_ancestor(u, n + 5) == -1);
        for (int v = 1; v <= n; v++) {
            int w = lca(u, v);
            CHECK("get_lca", t.get_lca(u, v) == w);
            CHECK("dist", t.dist(u, v) == dep[u] + dep[v] - 2 * dep[w]);
        }
    }
}

signed main() {
    {
        LCA d; // default constructor (n = 0) is valid as a placeholder
        d = LCA(1);
        d.build();
        CHECK("default ctor + build() default root", d.get_lca(1, 1) == 1 && d.dist(1, 1) == 0 && d.kth_ancestor(1, 1) == -1);
    }
    for (int it = 0; it < 6000; it++) {
        int shape = it % 7, n = rnd(1, it % 40 == 0 ? 120 : 25);
        auto e = gen(n, shape);
        int root = it % 5 == 0 ? 1 : rnd(1, n);
        one(n, e, root, it % 4 == 3);
    }
    for (auto &[k, v] : cov) cout << "coverage " << k << ": " << v << '\n';
    cout << "mismatches: " << fails << '\n';
    cout << (fails ? "RESULT: FAIL" : "RESULT: PASS") << '\n';
    return fails ? 1 : 0;
}
