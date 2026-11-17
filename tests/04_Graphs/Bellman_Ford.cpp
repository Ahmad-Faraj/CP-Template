// Independent test for 04_Graphs/Bellman_Ford.cpp
// Oracle: reachability closure + walk DP (best walk of exactly k edges) + negative closed walk search.
//   node x "on a negative cycle"  <=> some closed walk x..x of 1..n edges has negative weight
//   v affected from src            <=> exists such x with src ->* x ->* v        (expected NEG)
//   v unreachable                  (expected INF), else min over walks of <= n-1 edges.
// Build: g++ -std=c++17 -O2 Bellman_Ford.cpp && ./a.out         (add "big" for the n = m = 2e5 timing run)
//        g++ -std=c++17 -O1 -g -fsanitize=address,undefined -DDEFINE_INT_LL Bellman_Ford.cpp
#include <bits/stdc++.h>
using namespace std;
#ifdef DEFINE_INT_LL
#define int long long
#endif
#include "../../04_Graphs/Bellman_Ford.cpp"

static long long mismatches = 0;
static map<string, long long> cov;
#define CHECK(name, cond)                                                                    \
    do {                                                                                     \
        cov[name]++;                                                                         \
        if (!(cond) && mismatches++ < 10) fprintf(stderr, "MISMATCH [%s] line %d\n", name, __LINE__); \
    } while (0)

mt19937_64 rng(20261118);
long long rnd(long long a, long long b) { return uniform_int_distribution<long long>(a, b)(rng); }

struct Oracle {
    int n;
    vector<vector<long long>> w;
    vector<vector<char>> has, reach;
    vector<char> oncyc;
    Oracle(int n) : n(n), w(n + 1, vector<long long>(n + 1, 0)), has(n + 1, vector<char>(n + 1, 0)),
                    reach(n + 1, vector<char>(n + 1, 0)), oncyc(n + 1, 0) {}
    void edge(int u, int v, long long c) {
        if (!has[u][v] || c < w[u][v]) w[u][v] = c;
        has[u][v] = 1;
    }
    // best[k][v] = min weight of a walk src..v with exactly k edges (valid where ok[k][v])
    void walks(int src, int K, vector<vector<long long>> &best, vector<vector<char>> &ok) {
        best.assign(K + 1, vector<long long>(n + 1, 0));
        ok.assign(K + 1, vector<char>(n + 1, 0));
        ok[0][src] = 1;
        for (int k = 1; k <= K; k++)
            for (int u = 1; u <= n; u++)
                if (ok[k - 1][u])
                    for (int v = 1; v <= n; v++)
                        if (has[u][v]) {
                            long long c = best[k - 1][u] + w[u][v];
                            if (!ok[k][v] || c < best[k][v]) best[k][v] = c, ok[k][v] = 1;
                        }
    }
    void run() {
        for (int u = 1; u <= n; u++) {
            reach[u][u] = 1;
            for (int v = 1; v <= n; v++)
                if (has[u][v]) reach[u][v] = 1;
        }
        for (int k = 1; k <= n; k++)
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++)
                    if (reach[u][k] && reach[k][v]) reach[u][v] = 1;
        for (int x = 1; x <= n; x++) {
            vector<vector<long long>> b;
            vector<vector<char>> ok;
            walks(x, n, b, ok);
            for (int k = 1; k <= n; k++)
                if (ok[k][x] && b[k][x] < 0) oncyc[x] = 1;
        }
    }
    bool affected(int s, int v) {
        for (int x = 1; x <= n; x++)
            if (oncyc[x] && reach[s][x] && reach[x][v]) return true;
        return false;
    }
    long long shortest(int s, int v) { // only for reachable, unaffected v
        vector<vector<long long>> b;
        vector<vector<char>> ok;
        walks(s, n - 1, b, ok); // n >= 1
        bool f = false;
        long long r = 0;
        for (int k = 0; k <= n - 1; k++)
            if (ok[k][v] && (!f || b[k][v] < r)) r = b[k][v], f = true;
        return r;
    }
};

template <class T> void random_case(long long lo, long long hi, bool dag) {
    using BF = Bellman_Ford<T>;
    int n = rnd(1, 7), m = rnd(0, 16);
    BF g(n);
    Oracle o(n);
    for (int i = 0; i < m; i++) {
        int u = rnd(1, n), v = rnd(1, n);
        if (!dag && rnd(0, 7) == 0) v = u; // self loop
        if (dag) {
            if (u == v) continue;
            if (u > v) swap(u, v);
        }
        T c = rnd(0, 4) == 0 ? 0 : rnd(lo, hi);
        int mode = dag ? 2 : rnd(0, 3);
        if (mode == 2) g.add_edge(u, v, c); // default: directed
        else g.add_edge(u, v, c, mode == 1);
        cov["add_edge"]++;
        o.edge(u, v, c);
        if (mode == 1) o.edge(v, u, c);
    }
    o.run();
    const T INF = BF::INF, NEG = BF::NEG;
    for (int s = 1; s <= n; s++) {
        vector<T> d = g.shortest_path(s);
        bool ok = (int)d.size() == n + 1;
        vector<T> expect(n + 1, INF);
        for (int v = 1; v <= n; v++) {
            if (!o.reach[s][v]) expect[v] = INF;
            else if (o.affected(s, v)) expect[v] = NEG;
            else expect[v] = o.shortest(s, v);
            ok = ok && d[v] == expect[v];
            CHECK("is_inf", BF::is_inf(d[v]) == (!o.reach[s][v]));
            CHECK("is_neg", BF::is_neg(d[v]) == (o.reach[s][v] && o.affected(s, v)));
        }
        CHECK("shortest_path(src)", ok);
        CHECK("member dist", g.dist == d);
        bool anyneg = false;
        for (int x = 1; x <= n; x++) anyneg = anyneg || (o.oncyc[x] && o.reach[s][x]);
        CHECK("has_negative_cycle", g.has_negative_cycle(s) == anyneg);
        for (int t = 1; t <= n; t++) {
            CHECK("shortest_path(src,dest)", g.shortest_path(s, t) == expect[t]);
            vector<int> p = g.path(s, t);
            if (expect[t] == INF || expect[t] == NEG) {
                CHECK("path", p.empty());
                continue;
            }
            bool pok = !p.empty() && p.front() == s && p.back() == t && (int)p.size() <= n;
            long long sum = 0;
            for (int i = 0; pok && i + 1 < (int)p.size(); i++) {
                if (p[i] < 1 || p[i] > n || !o.has[p[i]][p[i + 1]]) pok = false;
                else sum += o.w[p[i]][p[i + 1]];
            }
            CHECK("path", pok && sum == expect[t]);
        }
    }
}

void edge_cases() {
    using BF = Bellman_Ford<long long>;
    { // single node with a negative self loop
        BF g(1);
        g.add_edge(1, 1, -1);
        CHECK("is_neg", BF::is_neg(g.shortest_path(1, 1)));
        CHECK("has_negative_cycle", g.has_negative_cycle(1));
    }
    { // negative cycle not reachable from the source
        BF g(4);
        g.add_edge(1, 2, 3);
        g.add_edge(3, 4, -5), g.add_edge(4, 3, 1);
        CHECK("has_negative_cycle", !g.has_negative_cycle(1));
        CHECK("shortest_path(src,dest)", g.shortest_path(1, 2) == 3);
        CHECK("is_inf", BF::is_inf(g.shortest_path(1, 3)));
        CHECK("has_negative_cycle", g.has_negative_cycle(3));
    }
    { // undirected negative edge is a negative cycle
        BF g(2);
        g.add_edge(1, 2, -1, true);
        CHECK("has_negative_cycle", g.has_negative_cycle(1));
        CHECK("path", g.path(1, 2).empty());
    }
    { // large weights, 1e9 * 60 overflows 32 bits
        int n = 61;
        BF g(n);
        for (int i = 1; i < n; i++) g.add_edge(i, i + 1, -1000000000LL);
        CHECK("shortest_path(src,dest)", g.shortest_path(1, n) == -60LL * 1000000000LL);
        CHECK("path", (int)g.path(1, n).size() == n);
    }
}

void big() {
    // random graph, non-negative weights; independent oracle: a plain set-based Dijkstra written here
    int n = 200000, m = 200000;
    Bellman_Ford<long long> g(n);
    vector<vector<pair<int, long long>>> adj(n + 1);
    for (int i = 1; i < n; i++) { // spine, so every node is reachable
        long long c = rnd(0, 1000000000LL);
        g.add_edge(i, i + 1, c);
        adj[i].push_back({i + 1, c});
    }
    for (int i = 0; i < m; i++) {
        int u = rnd(1, n), v = rnd(1, n);
        long long c = rnd(0, 1000000000LL);
        g.add_edge(u, v, c);
        adj[u].push_back({v, c});
    }
    auto t0 = chrono::steady_clock::now();
    vector<long long> d = g.shortest_path(1);
    double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    vector<long long> e(n + 1, LLONG_MAX);
    set<pair<long long, int>> q;
    e[1] = 0, q.insert({0, 1});
    while (!q.empty()) {
        auto [du, u] = *q.begin();
        q.erase(q.begin());
        for (auto [v, c] : adj[u])
            if (du + c < e[v]) {
                if (e[v] != LLONG_MAX) q.erase({e[v], v});
                e[v] = du + c, q.insert({e[v], v});
            }
    }
    bool ok = true;
    for (int v = 1; v <= n; v++) ok = ok && (e[v] == LLONG_MAX ? Bellman_Ford<long long>::is_inf(d[v]) : d[v] == e[v]);
    CHECK("big n=m=2e5", ok);
    printf("big: n=%lld m=%lld (+spine) shortest_path %.1f ms (O(nm) worst case; random graph exits early)\n", (long long)n,
           (long long)m, ms);
    // a long chain in reverse edge order forces ~n rounds: time a smaller worst case
    int n2 = 5000;
    Bellman_Ford<long long> h(n2);
    for (int i = n2 - 1; i >= 1; i--) h.add_edge(i, i + 1, 1000000000LL);
    t0 = chrono::steady_clock::now();
    long long last = h.shortest_path(1, n2);
    ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    CHECK("big worst chain", last == (long long)(n2 - 1) * 1000000000LL);
    printf("big: worst-order chain n=m=%lld %.1f ms\n", (long long)n2, ms);
}

signed main(signed argc, char **argv) {
    for (int it = 0; it < 2000; it++) random_case<long long>(-10, 20, false);
    for (int it = 0; it < 1000; it++) random_case<long long>(-1000000000, 1000000000, false);
    for (int it = 0; it < 1000; it++) random_case<long long>(-1000000000, 1000000000, true);
    for (int it = 0; it < 1000; it++) random_case<long long>(0, 1000000000, false);
    for (int it = 0; it < 500; it++) random_case<int>(-50, 100, false);
    edge_cases();
    if (argc > 1 && string(argv[1]) == "big") big();
    const char *need[] = {"add_edge", "shortest_path(src)", "shortest_path(src,dest)", "has_negative_cycle", "path",
                          "is_inf", "is_neg", "member dist"};
    bool all = true;
    for (auto s : need) {
        printf("coverage: %-26s %lld checks\n", s, cov[s]);
        all = all && cov[s] > 0;
    }
    printf("mismatches: %lld\n", mismatches);
    bool pass = all && mismatches == 0;
    printf("RESULT: %s\n", pass ? "PASS" : "FAIL");
    return pass ? 0 : 1;
}
