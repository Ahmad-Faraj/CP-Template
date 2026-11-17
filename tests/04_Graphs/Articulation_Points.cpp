// Independent random test for 04_Graphs/Articulation_Points.cpp (struct Articulation_points<T>).
// Oracle: delete each vertex in turn and count connected components by BFS; v is an articulation point
// iff the remaining graph has more components than the original.
// Build: g++ -std=c++17 -O1 -g -fsanitize=address,undefined Articulation_Points.cpp   (add -DINT_LL for #define int long long)
#include <bits/stdc++.h>
using namespace std;
#ifdef INT_LL
#define int long long
#endif
#include "../../04_Graphs/Articulation_Points.cpp"

static mt19937 rng(4242);
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
// 7 random tree (+ some duplicates), 8 dense
static vector<pair<int, int>> gen(int &n, int shape) {
    vector<pair<int, int>> e;
    if (shape == 2) n = 1;
    if (shape == 0) { int m = rnd(0, 2 * n); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 1) { int m = rnd(0, n / 2 + 1); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    if (shape == 2) { int m = rnd(0, 3); for (int i = 0; i < m; i++) e.push_back({1, 1}); }
    if (shape == 3) for (int i = 1; i < n; i++) e.push_back({i, i + 1});
    if (shape == 4) { int c = rnd(1, n); for (int i = 1; i <= n; i++) if (i != c) e.push_back({c, i}); }
    if (shape == 5) { for (int i = 1; i <= n; i++) e.push_back({i, i % n + 1}); }
    if (shape == 7) for (int i = 2; i <= n; i++) { int p = rnd(1, i - 1); e.push_back({p, i}); if (rnd(0, 3) == 0) e.push_back({i, p}); }
    if (shape == 8) { int m = rnd(n, n * n); for (int i = 0; i < m; i++) e.push_back({rnd(1, n), rnd(1, n)}); }
    vector<int> p(n); iota(p.begin(), p.end(), 1); shuffle(p.begin(), p.end(), rng);
    for (auto &[u, v] : e) { u = p[u - 1], v = p[v - 1]; if (rnd(0, 1)) swap(u, v); }
    shuffle(e.begin(), e.end(), rng);
    return e;
}

static int comps_without(int n, const vector<pair<int, int>> &e, int del) {
    vector<vector<int>> g(n + 1);
    for (auto [u, v] : e) if (u != del && v != del) g[u].push_back(v), g[v].push_back(u);
    vector<char> vis(n + 1, 0); int k = 0;
    for (int s = 1; s <= n; s++) {
        if (s == del || vis[s]) continue;
        k++; vis[s] = 1; queue<int> q; q.push(s);
        while (!q.empty()) { int u = q.front(); q.pop(); for (int v : g[u]) if (!vis[v]) vis[v] = 1, q.push(v); }
    }
    return k;
}

template <class AP> static void one(int n, const vector<pair<int, int>> &e, const char *tag) {
    string t = tag;
    AP ap(n);
    for (auto [u, v] : e) ap.add_edge(u, v);
    cov["add_edge" + t] += e.size();
    ap.solve();
    cov["solve/tarjan" + t]++;

    int base = comps_without(n, e, 0);
    set<long long> want;
    for (int v = 1; v <= n; v++) if (comps_without(n, e, v) > base) want.insert(v);

    auto got = ap.get_articulation_points();
    set<long long> gotl(got.begin(), got.end());
    CHECK("get_articulation_points" + t, gotl == want);

    // print(): count, then each point on its own line, ascending
    ostringstream exp;
    exp << want.size() << "\n";
    for (auto x : want) exp << x << "\n";
    ostringstream cap;
    auto *old = cout.rdbuf(cap.rdbuf());
    ap.print();
    cout.rdbuf(old);
    CHECK("print" + t, cap.str() == exp.str());
}

signed main() {
    {
        Articulation_points<> d; // default constructor compiles and is usable as a placeholder
        d = Articulation_points<>(3);
        d.add_edge(1, 2), d.add_edge(2, 3);
        d.solve();
        CHECK("default ctor + assign", d.get_articulation_points() == set<int>({2}));
    }
    for (int it = 0; it < 6000; it++) {
        int shape = it % 9, n = rnd(1, it % 50 == 0 ? 30 : 10);
        auto e = gen(n, shape);
        one<Articulation_points<>>(n, e, " [T=default]");
        one<Articulation_points<long long>>(n, e, " [T=long long]");
    }
    one<Articulation_points<>>(1, {}, " [T=default]");
    one<Articulation_points<>>(5, {{1, 2}, {2, 3}, {3, 1}, {3, 4}}, " [T=default]"); // 3 is a cut, 5 isolated is not
    // direct tarjan() call from an explicit root, same as solve() for a connected graph
    {
        Articulation_points<> ap(4);
        ap.add_edge(1, 2), ap.add_edge(2, 3), ap.add_edge(3, 4);
        ap.tarjan(2, -1);
        CHECK("tarjan(root,-1) direct", ap.get_articulation_points() == set<int>({2, 3}));
    }
    for (auto &[k, v] : cov) cout << "coverage " << k << ": " << v << '\n';
    cout << "mismatches: " << fails << '\n';
    cout << (fails ? "RESULT: FAIL" : "RESULT: PASS") << '\n';
    return fails ? 1 : 0;
}
