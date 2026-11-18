// Test for 04_Graphs/DFS.cpp. A DFS forest is not unique, so the oracle checks the properties that
// define one, by brute force: roots are the smallest unvisited nodes, tree edges are real edges,
// depth/subtree/in/out agree with the par[] forest, times are a permutation of 1..2n with nested
// intervals, and every edge u->v has in[v] < out[u] (the DFS white-path property). Bipartiteness
// (undirected) is compared with trying all 2^n colourings.
// Every public method is checked: add_edge (directed and bidirectional), dfs (incl. rerun after adding
// edges), dfs_visit (one tree directly).
#include "../../04_Graphs/DFS.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(41);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

bool check_forest(DFS &D, int n, const vector<pair<int, int>> &e) { // e: directed arcs
    set<pair<int, int>> arcs(e.begin(), e.end());
    vector<int> reach_root(n + 1, 0);
    // roots: brute reachability from the roots in order 1..n
    vector<char> seen(n + 1, 0);
    bool ok = true;
    for (int r = 1; r <= n; r++) {
        if (seen[r]) continue;
        ok &= D.par[r] == 0 && D.depth[r] == 0;
        vector<int> st = {r};
        seen[r] = 1;
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            reach_root[u] = r;
            for (auto [a, b] : e)
                if (a == u && !seen[b]) seen[b] = 1, st.push_back(b);
        }
    }
    vector<int> times;
    for (int v = 1; v <= n; v++) {
        int root = v;
        while (D.par[root]) root = D.par[root];
        ok &= root == reach_root[v];
        if (D.par[v]) ok &= arcs.count({D.par[v], v}) && D.depth[v] == D.depth[D.par[v]] + 1;
        int sub = 0;
        for (int x = 1; x <= n; x++) {
            bool anc = false;
            for (int y = x; y; y = D.par[y]) anc |= y == v;
            sub += anc;
            bool by_time = D.in[v] <= D.in[x] && D.out[x] <= D.out[v];
            ok &= anc == by_time;
        }
        ok &= D.subtree[v] == sub;
        ok &= D.in[v] < D.out[v];
        times.push_back(D.in[v]), times.push_back(D.out[v]);
    }
    sort(times.begin(), times.end());
    for (int i = 0; i < 2 * n; i++) ok &= times[i] == i + 1;
    for (auto [u, v] : e) ok &= D.in[v] < D.out[u];
    return ok;
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 10 + 1, m = rng() % (2 * n);
        bool directed = rng() % 2;
        DFS D(n);
        vector<pair<int, int>> arcs, und;
        auto add = [&](int u, int v) {
            if (directed) D.add_edge(u, v), arcs.push_back({u, v});
            else {
                if (rng() % 2) D.add_edge(u, v, true);
                else D.add_edge(u, v), D.add_edge(v, u);
                arcs.push_back({u, v}), arcs.push_back({v, u}), und.push_back({u, v});
            }
        };
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            if (u == v && rng() % 3) continue;
            add(u, v);
        }
        string t = directed ? "dir:" : "undir:";
        bool bip = D.dfs();
        chk(t + "dfs(forest properties)", check_forest(D, n, arcs));
        if (!directed) {
            bool want = false;
            for (int mask = 0; mask < (1 << n) && !want; mask++) {
                bool ok = true;
                for (auto [u, v] : und) ok &= ((mask >> (u - 1)) & 1) != ((mask >> (v - 1)) & 1);
                want = ok;
            }
            chk("undir:dfs(bipartite)", bip == want);
        }
        // rerun after adding more edges
        for (int i = 0; i < 3; i++) add(rng() % n + 1, rng() % n + 1);
        D.dfs();
        chk(t + "dfs(rerun)", check_forest(D, n, arcs));
        {   // dfs_visit on its own from node 1 (vis/timer fresh)
            DFS d2(n);
            for (auto [u, v] : arcs) d2.add_edge(u, v);
            d2.dfs_visit(1);
            int cnt = 0;
            for (int v = 1; v <= n; v++) cnt += d2.vis[v];
            chk("dfs_visit(subtree[1] = reached)", d2.subtree[1] == cnt && d2.out[1] == 2 * cnt);
        }
    }
    for (int shape = 0; shape < 2; shape++) {
        int n = 200000;
        DFS D(n);
        for (int i = 2; i <= n; i++) D.add_edge(i, shape ? i - 1 : (int)(rng() % (i - 1)) + 1, true);
        if (!shape)
            for (int i = 0; i < 200000; i++) D.add_edge(rng() % n + 1, rng() % n + 1, true);
        auto t0 = clock();
        bool b = D.dfs();
        double tm = double(clock() - t0) / CLOCKS_PER_SEC;
        if (shape) chk("large path(bipartite, subtree)", b && D.subtree[1] == n && D.depth[n] == n - 1);
        printf("%s n=2e5: %.3fs\n", shape ? "path" : "random m=4e5", tm);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-34s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
