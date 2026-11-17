// Independent test for 04_Graphs/Dijkstra.cpp
// Oracle: Floyd-Warshall on an adjacency matrix with a separate reachability matrix (no INF arithmetic).
// Build: g++ -std=c++17 -O2 Dijkstra.cpp && ./a.out            (add "big" for the n = m = 2e5 timing run)
//        g++ -std=c++17 -O1 -g -fsanitize=address,undefined -DDEFINE_INT_LL Dijkstra.cpp
#include <bits/stdc++.h>
using namespace std;
#ifdef DEFINE_INT_LL
#define int long long
#endif
#include "../../04_Graphs/Dijkstra.cpp"

static long long mismatches = 0;
static map<string, long long> cov;
#define CHECK(name, cond)                                                                    \
    do {                                                                                     \
        cov[name]++;                                                                         \
        if (!(cond) && mismatches++ < 10) fprintf(stderr, "MISMATCH [%s] line %d\n", name, __LINE__); \
    } while (0)

mt19937_64 rng(20261117);
long long rnd(long long a, long long b) { return uniform_int_distribution<long long>(a, b)(rng); }

struct Oracle {
    int n;
    vector<vector<long long>> d;
    vector<vector<char>> r;
    vector<vector<long long>> w; // min direct weight u->v, valid when has[u][v]
    vector<vector<char>> has;
    Oracle(int n) : n(n), d(n + 1, vector<long long>(n + 1, 0)), r(n + 1, vector<char>(n + 1, 0)),
                    w(n + 1, vector<long long>(n + 1, 0)), has(n + 1, vector<char>(n + 1, 0)) {}
    void edge(int u, int v, long long c) {
        if (!has[u][v] || c < w[u][v]) w[u][v] = c;
        has[u][v] = 1;
    }
    void run() {
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) {
                r[u][v] = (u == v) || has[u][v];
                d[u][v] = (u == v) ? 0 : (has[u][v] ? w[u][v] : 0); // weights are non-negative here
            }
        for (int k = 1; k <= n; k++)
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++)
                    if (r[u][k] && r[k][v]) {
                        long long c = d[u][k] + d[k][v];
                        if (!r[u][v] || c < d[u][v]) d[u][v] = c, r[u][v] = 1;
                    }
    }
};

template <class D> void check_path(D &g, Oracle &o, int s, int t) {
    vector<int> p = g.path(s, t);
    if (!o.r[s][t]) {
        CHECK("path", p.empty());
        return;
    }
    bool ok = !p.empty() && p.front() == s && p.back() == t && (int)p.size() <= o.n;
    long long sum = 0;
    for (int i = 0; ok && i + 1 < (int)p.size(); i++) {
        if (p[i] < 1 || p[i] > o.n || !o.has[p[i]][p[i + 1]]) ok = false;
        else sum += o.w[p[i]][p[i + 1]];
    }
    CHECK("path", ok && sum == o.d[s][t]);
}

template <class T> void random_case(int maxw) {
    using D = Dijkstra<T>;
    int n = rnd(1, 8), m = rnd(0, 22);
    D g(n);
    Oracle o(n);
    CHECK("constructor", (int)g.adj.size() == n + 1);
    for (int i = 0; i < m; i++) {
        int u = rnd(1, n), v = rnd(1, n);
        if (rnd(0, 6) == 0) v = u; // self loops
        T c = rnd(0, 3) == 0 ? 0 : rnd(0, maxw);
        int mode = rnd(0, 2);
        if (mode == 0) g.add_edge(u, v, c); // default: bidirectional
        else g.add_edge(u, v, c, mode == 1);
        cov["add_edge"]++;
        o.edge(u, v, c);
        if (mode != 2) o.edge(v, u, c);
        if (rnd(0, 4) == 0 && i + 1 < m) { // parallel edge
            T c2 = rnd(0, maxw);
            g.add_edge(u, v, c2, false), cov["add_edge"]++;
            o.edge(u, v, c2);
        }
    }
    o.run();
    const T INF = D::INF;
    for (int s = 1; s <= n; s++) {
        vector<T> d = g.shortest_path(s);
        bool ok = (int)d.size() == n + 1 && d[0] == INF;
        for (int v = 1; ok && v <= n; v++) ok = o.r[s][v] ? d[v] == (T)o.d[s][v] : d[v] == INF;
        CHECK("shortest_path(src)", ok);
        CHECK("member dist", g.dist == d);
        int t = rnd(1, n);
        T one = g.shortest_path(s, t);
        CHECK("shortest_path(src,dest)", o.r[s][t] ? one == (T)o.d[s][t] : one == INF);
        for (int t2 = 1; t2 <= n; t2++) check_path(g, o, s, t2);
    }
    // multi-source, including an empty source list and duplicates
    for (int rep = 0; rep < 3; rep++) {
        vector<int> S;
        int k = rnd(0, n);
        for (int i = 0; i < k; i++) S.push_back(rnd(1, n));
        vector<T> d = g.shortest_path(S);
        bool ok = (int)d.size() == n + 1;
        for (int v = 1; ok && v <= n; v++) {
            bool reach = false;
            long long best = 0;
            for (int s : S)
                if (o.r[s][v] && (!reach || o.d[s][v] < best)) best = o.d[s][v], reach = true;
            ok = reach ? d[v] == (T)best : d[v] == INF;
        }
        CHECK("shortest_path(srcs)", ok);
    }
}

void edge_cases() {
    { // single node, no edges
        Dijkstra<long long> g(1);
        CHECK("shortest_path(src)", g.shortest_path(1)[1] == 0);
        CHECK("path", g.path(1, 1) == vector<int>{1});
    }
    { // disconnected: 1-2 and 3 alone
        Dijkstra<long long> g(3);
        g.add_edge(1, 2, 5);
        CHECK("shortest_path(src,dest)", g.shortest_path(1, 3) == Dijkstra<long long>::INF);
        CHECK("path", g.path(1, 3).empty());
        CHECK("shortest_path(src,dest)", g.shortest_path(2, 1) == 5); // default is bidirectional
    }
    { // directed edge is one way
        Dijkstra<long long> g(2);
        g.add_edge(1, 2, 7, false);
        CHECK("shortest_path(src,dest)", g.shortest_path(2, 1) == Dijkstra<long long>::INF);
        CHECK("shortest_path(src,dest)", g.shortest_path(1, 2) == 7);
    }
    { // weights near 1e9 summing far past 32-bit
        int n = 50;
        Dijkstra<long long> g(n);
        for (int i = 1; i < n; i++) g.add_edge(i, i + 1, 1000000000LL, false);
        CHECK("shortest_path(src,dest)", g.shortest_path(1, n) == 49LL * 1000000000LL);
        CHECK("path", (int)g.path(1, n).size() == n);
    }
}

void big() {
    int n = 200000, m = 200000;
    Dijkstra<long long> g(n);
    for (int i = 1; i < n; i++) g.add_edge(i, i + 1, 1000000000LL, false); // spine
    for (int i = 0; i < m; i++) g.add_edge(rnd(1, n), rnd(1, n), rnd(0, 1000000000LL), rnd(0, 1));
    auto t0 = chrono::steady_clock::now();
    vector<long long> d = g.shortest_path(1);
    vector<int> p = g.path(1, n);
    double ms = chrono::duration<double, milli>(chrono::steady_clock::now() - t0).count();
    // independent check: every edge is relaxed (d[v] <= d[u] + w) and d[1] = 0, every reached node is tight
    bool ok = d[1] == 0 && d[n] <= (long long)(n - 1) * 1000000000LL;
    vector<char> tight(n + 1, 0);
    tight[1] = 1;
    for (int u = 1; u <= n; u++)
        for (auto &e : g.adj[u]) {
            if (d[u] + e.w < d[e.v]) ok = false;
            if (d[u] + e.w == d[e.v]) tight[e.v] = 1;
        }
    for (int v = 1; v <= n; v++) ok = ok && tight[v];
    CHECK("big n=m=2e5", ok && !p.empty() && p.front() == 1 && p.back() == n);
    printf("big: n=%lld m=%lld (+spine) shortest_path+path %.1f ms\n", (long long)n, (long long)(m + n - 1), ms);
}

signed main(signed argc, char **argv) {
    for (int it = 0; it < 3000; it++) random_case<long long>(1000000000);
    for (int it = 0; it < 1000; it++) random_case<long long>(10);
    for (int it = 0; it < 1000; it++) random_case<int>(1000);
    edge_cases();
    if (argc > 1 && string(argv[1]) == "big") big();
    const char *need[] = {"constructor", "add_edge", "shortest_path(src)", "shortest_path(src,dest)",
                          "shortest_path(srcs)", "path", "member dist"};
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
