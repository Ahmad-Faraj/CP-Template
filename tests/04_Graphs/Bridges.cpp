// Independent random test for 04_Graphs/Bridges.cpp (struct Bridges).
// Oracle: delete each edge in turn and BFS; edge e is a bridge iff its endpoints become disconnected.
// u,v are 2-edge-connected iff they stay connected after deleting ANY single edge (Menger), computed directly.
// Build: g++ -std=c++17 -O1 -g -fsanitize=address,undefined Bridges.cpp   (add -DINT_LL for #define int long long)
#include <bits/stdc++.h>
using namespace std;
#ifdef INT_LL
#define int long long
#endif
#include "../../04_Graphs/Bridges.cpp"

static mt19937 rng(777);
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

// shapes: 0 random(multi/self), 1 sparse/disconnected, 2 single node, 3 path, 4 star, 5 cycle, 6 no edges,
// 7 tree + duplicated edges, 8 dense
static vector<pair<int, int>> gen(int &n, int shape) {
    vector<pair<int, int>> e;
    if (shape == 2) n = 1;
    if (shape == 0) { int m = rnd(0, 2 * n); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 1) { int m = rnd(0, n / 2 + 1); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 2) { int m = rnd(0, 3); for (int i = 0; i < m; i++) e.push_back({1, 1}); }
    if (shape == 3) for (int i = 1; i < n; i++) e.push_back({i, i + 1});
    if (shape == 4) { int c = rnd(1, n); for (int i = 1; i <= n; i++) if (i != c) e.push_back({c, i}); }
    if (shape == 5) { for (int i = 1; i <= n; i++) e.push_back({i, i % n + 1}); }
    if (shape == 7) {
        for (int i = 2; i <= n; i++) { int p = rnd(1, i - 1); e.push_back({p, i}); if (rnd(0, 2) == 0) e.push_back({i, p}); }
        if (rnd(0, 1)) e.push_back({rnd(1, n), rnd(1, n)});
    }
    if (shape == 8) { int m = rnd(n, n * n); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    // random relabel + shuffle + random orientation
    vector<int> p(n); iota(p.begin(), p.end(), 1); shuffle(p.begin(), p.end(), rng);
    for (auto &[u, v] : e) { u = p[u - 1], v = p[v - 1]; if (rnd(0, 1)) swap(u, v); }
    shuffle(e.begin(), e.end(), rng);
    return e;
}

static vector<int> labels(int n, const vector<pair<int, int>> &e, int skip) {
    vector<vector<int>> g(n + 1);
    for (int i = 0; i < (int)e.size(); i++) if (i != skip) g[e[i].first].push_back(e[i].second), g[e[i].second].push_back(e[i].first);
    vector<int> lab(n + 1, 0); int k = 0;
    for (int s = 1; s <= n; s++) {
        if (lab[s]) continue;
        lab[s] = ++k; queue<int> q; q.push(s);
        while (!q.empty()) { int u = q.front(); q.pop(); for (int v : g[u]) if (!lab[v]) lab[v] = k, q.push(v); }
    }
    return lab;
}

static void one(int n, const vector<pair<int, int>> &e) {
    Bridges b(n);
    for (auto [u, v] : e) b.add_edge(u, v);
    cov["add_edge"] += e.size();
    b.run();
    cov["run/dfs/label_components"]++;

    int m = e.size();
    vector<int> base = labels(n, e, -1);
    vector<vector<int>> without(m);
    vector<char> isb(m);
    set<pair<int, int>> want_bridges;
    for (int i = 0; i < m; i++) {
        without[i] = labels(n, e, i);
        isb[i] = without[i][e[i].first] != without[i][e[i].second];
        if (isb[i]) want_bridges.insert(minmax(e[i].first, e[i].second));
    }
    // 2-edge-connected: connected in the original and after every single-edge deletion
    vector<int> cls(n + 1, 0); int k = 0;
    for (int u = 1; u <= n; u++) {
        if (cls[u]) continue;
        cls[u] = ++k;
        for (int v = u + 1; v <= n; v++) {
            if (cls[v] || base[u] != base[v]) continue;
            bool ok = true;
            for (int i = 0; i < m && ok; i++) ok = without[i][u] == without[i][v];
            if (ok) cls[v] = k;
        }
    }

    // bridge list / count
    set<pair<int, int>> got;
    for (auto [u, v] : b.bridge_list) got.insert(minmax(u, v));
    CHECK("bridge_list (as edge set)", got == want_bridges);
    CHECK("bridge_list no duplicates", got.size() == b.bridge_list.size());
    CHECK("bridge_count", b.bridge_count() == (int)want_bridges.size());
    // is_bridge on every existing edge (documented contract), incl. parallel edges and self loops
    for (int i = 0; i < m; i++) {
        CHECK("is_bridge(u,v) on existing edge", b.is_bridge(e[i].first, e[i].second) == (bool)isb[i]);
        CHECK("is_bridge(v,u) on existing edge", b.is_bridge(e[i].second, e[i].first) == (bool)isb[i]);
        CHECK("is_bridge_edge[id]", (bool)b.is_bridge_edge[i] == (bool)isb[i]);
        bool parallel = false;
        for (int j = 0; j < m; j++) if (j != i && minmax(e[i].first, e[i].second) == minmax(e[j].first, e[j].second)) parallel = true;
        if (parallel || e[i].first == e[i].second) CHECK("parallel/self-loop edge never a bridge", !b.is_bridge(e[i].first, e[i].second));
    }
    // 2-edge-connected components
    CHECK("component_count", b.component_count() == k);
    for (int u = 1; u <= n; u++) CHECK("component_of range", b.component_of(u) >= 1 && b.component_of(u) <= k);
    for (int u = 1; u <= n; u++)
        for (int v = 1; v <= n; v++) CHECK("component_of partition", (b.component_of(u) == b.component_of(v)) == (cls[u] == cls[v]));
    // bridge tree: vertices = components, one edge per bridge between its endpoints' components; a forest
    auto tree = b.bridge_tree();
    CHECK("bridge_tree size", (int)tree.size() == k + 1);
    if ((int)tree.size() == k + 1) {
        multiset<pair<int, int>> want, have;
        for (int i = 0; i < m; i++) if (isb[i]) want.insert(minmax(b.component_of(e[i].first), b.component_of(e[i].second)));
        long long deg = 0;
        for (int c = 0; c <= k; c++) for (int d : tree[c]) { deg++; if (c < d) have.insert({c, d}); CHECK("bridge_tree no self loop", c != d); }
        CHECK("bridge_tree edges", have == want && deg == 2 * (long long)want.size());
        int comps = *max_element(base.begin() + 1, base.end());
        CHECK("bridge_tree is a forest (edges = comps2e - comps)", (int)want.size() == k - comps);
    }
}

signed main() {
    for (int it = 0; it < 6000; it++) {
        int shape = it % 9, n = rnd(1, it % 50 == 0 ? 30 : 10);
        auto e = gen(n, shape);
        one(n, e);
    }
    one(4, {{1, 2}, {1, 2}, {2, 3}, {3, 3}});      // parallel pair not a bridge, 2-3 is, self loop is not
    one(1, {});
    for (auto &[k, v] : cov) cout << "coverage " << k << ": " << v << '\n';
    cout << "mismatches: " << fails << '\n';
    cout << (fails ? "RESULT: FAIL" : "RESULT: PASS") << '\n';
    return fails ? 1 : 0;
}
