// Independent random test for 04_Graphs/SCC_Tarjan.cpp (struct Strongly_Connected_Components).
// Oracle: BFS reachability from every node (u,v same SCC iff each reaches the other); a separate plain DFS
// with the same adjacency order gives the DFS forest for is_ancestor.
// Build: g++ -std=c++17 -O1 -g -fsanitize=address,undefined SCC_Tarjan.cpp   (add -DINT_LL for #define int long long)
#include <bits/stdc++.h>
using namespace std;
#ifdef INT_LL
#define int long long
#endif
#include "../../04_Graphs/SCC_Tarjan.cpp"

static mt19937 rng(12345);
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

// graph shapes: 0 random(multi/self), 1 sparse/disconnected, 2 single node, 3 path, 4 star, 5 cycle, 6 no edges, 7 dense
static vector<pair<int, int>> gen(int &n, int shape) {
    vector<pair<int, int>> e;
    if (shape == 2) n = 1;
    if (shape == 0) { int m = rnd(0, 3 * n); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 1) { int m = rnd(0, n / 2 + 1); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 2) { int m = rnd(0, 3); for (int i = 0; i < m; i++) e.push_back({1, 1}); }
    if (shape == 3) for (int i = 1; i < n; i++) rnd(0, 1) ? e.push_back({i, i + 1}) : e.push_back({i + 1, i});
    if (shape == 4) { int c = rnd(1, n); for (int i = 1; i <= n; i++) if (i != c) rnd(0, 1) ? e.push_back({c, i}) : e.push_back({i, c}); }
    if (shape == 5) { for (int i = 1; i <= n; i++) e.push_back({i, i % n + 1}); }
    if (shape == 7) { int m = rnd(n, n * n + 2); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    shuffle(e.begin(), e.end(), rng);
    return e;
}

static void one(int n, const vector<pair<int, int>> &e, bool use_bidir) {
    // build: some edges inserted as bidirectional pairs via add_edge(u, v, true)
    Strongly_Connected_Components scc(n);
    vector<vector<int>> g(n + 1);
    for (auto [u, v] : e) {
        bool bi = use_bidir && rnd(0, 3) == 0;
        if (bi) { scc.add_edge(u, v, true); g[u].push_back(v); g[v].push_back(u); cov["add_edge(bidirectional=true)"]++; }
        else { scc.add_edge(u, v); g[u].push_back(v); cov["add_edge(default)"]++; }
    }
    scc.run();
    cov["run/dfs"]++;

    // oracle reachability
    vector<vector<char>> R(n + 1, vector<char>(n + 1, 0));
    for (int s = 1; s <= n; s++) {
        queue<int> q; q.push(s); R[s][s] = 1;
        while (!q.empty()) { int u = q.front(); q.pop(); for (int v : g[u]) if (!R[s][v]) R[s][v] = 1, q.push(v); }
    }
    vector<int> cls(n + 1, 0); int k = 0;
    for (int u = 1; u <= n; u++) if (!cls[u]) { ++k; for (int v = u; v <= n; v++) if (R[u][v] && R[v][u]) cls[v] = k; }

    CHECK("component_count", scc.component_count() == k);
    for (int u = 1; u <= n; u++) {
        int c = scc.component_of(u);
        CHECK("component_of range", c >= 1 && c <= k);
    }
    for (int u = 1; u <= n; u++)
        for (int v = 1; v <= n; v++) {
            bool same = cls[u] == cls[v];
            CHECK("same_component", scc.same_component(u, v) == same);
            CHECK("component_of partition", (scc.component_of(u) == scc.component_of(v)) == same);
        }
    // components member: components[c-1] holds exactly the nodes with id c
    CHECK("components size", (int)scc.components.size() == k);
    if ((int)scc.components.size() == k) {
        vector<int> seen(n + 1, 0);
        for (int c = 1; c <= k; c++)
            for (int x : scc.components[c - 1]) {
                CHECK("components content", x >= 1 && x <= n && scc.component_of(x) == c);
                if (x >= 1 && x <= n) seen[x]++;
            }
        for (int u = 1; u <= n; u++) CHECK("components cover", seen[u] == 1);
    }
    // condensation: c1->c2 iff some edge u->v with u in c1, v in c2, c1 != c2; sorted, no dups; ids reverse-topological
    auto dag = scc.condensation();
    CHECK("condensation size", (int)dag.size() == k + 1);
    if ((int)dag.size() == k + 1) {
        set<pair<int, int>> want, got;
        for (int u = 1; u <= n; u++) for (int v : g[u]) if (cls[u] != cls[v]) want.insert({scc.component_of(u), scc.component_of(v)});
        size_t total = 0;
        for (int c = 1; c <= k; c++) {
            CHECK("condensation sorted/unique", is_sorted(dag[c].begin(), dag[c].end()) && adjacent_find(dag[c].begin(), dag[c].end()) == dag[c].end());
            for (int d : dag[c]) { got.insert({c, d}); total++; CHECK("condensation reverse-topological id", c > d); }
        }
        CHECK("condensation edges", got == want && total == got.size());
        CHECK("condensation dag[0] empty", dag[0].empty());
        // oracle cross-check: c1->c2 edge implies every node of c1 reaches every node of c2, never back
        for (auto [a, b] : got) {
            int ua = scc.components[a - 1][0], ub = scc.components[b - 1][0];
            CHECK("condensation edge is real reachability", R[ua][ub] && !R[ub][ua]);
        }
    }
    // is_ancestor: independent DFS forest in the same adjacency order (scc.adj; equals g by construction)
    CHECK("adj equals inserted graph", [&] { for (int u = 1; u <= n; u++) if (vector<int>(scc.adj[u].begin(), scc.adj[u].end()) != g[u]) return false; return true; }());
    vector<int> par(n + 1, -1); vector<char> vis(n + 1, 0);
    function<void(int)> go = [&](int u) { vis[u] = 1; for (int v : g[u]) if (!vis[v]) par[v] = u, go(v); };
    for (int s = 1; s <= n; s++) if (!vis[s]) par[s] = 0, go(s);
    for (int u = 1; u <= n; u++)
        for (int v = 1; v <= n; v++) {
            bool anc = false;
            for (int x = v; x != 0; x = par[x]) if (x == u) { anc = true; break; }
            CHECK("is_ancestor", scc.is_ancestor(u, v) == anc);
        }
}

signed main() {
    for (int it = 0; it < 6000; it++) {
        int shape = it % 8, n = rnd(1, it % 50 == 0 ? 40 : 12);
        auto e = gen(n, shape);
        one(n, e, it % 3 == 0);
    }
    // a hand case: two cycles joined one way, plus an isolated node
    one(5, {{1, 2}, {2, 1}, {2, 3}, {3, 4}, {4, 3}}, false);
    for (auto &[k, v] : cov) cout << "coverage " << k << ": " << v << '\n';
    cout << "mismatches: " << fails << '\n';
    cout << (fails ? "RESULT: FAIL" : "RESULT: PASS") << '\n';
    return fails ? 1 : 0;
}
