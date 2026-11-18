// Test for 03_Trees/DSU_On_Tree.cpp against a brute "collect the subtree into a std::set" per node.
// Every public method is checked: add_edge, run (any root, rerun on the same object), and through run
// the internals dfs / add / dsu; sz[] is compared to brute subtree sizes.
#include "../../03_Trees/DSU_On_Tree.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(11);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

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

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 16 + 1, root = rng() % n + 1;
        int C = vector<int>{1, 2, 3, n, 2 * n + 5}[rng() % 5];
        auto e = rand_tree(n);
        DSU_On_Tree D(n);
        vector<vector<int>> g(n + 1);
        for (auto [u, v] : e) D.add_edge(u, v), g[u].push_back(v), g[v].push_back(u);
        for (int i = 1; i <= n; i++) D.col[i] = rng() % C;
        // brute: root by BFS, then walk each subtree
        vector<int> par(n + 1, 0), order = {root};
        for (int i = 0; i < (int)order.size(); i++)
            for (int v : g[order[i]])
                if (v != par[order[i]]) par[v] = order[i], order.push_back(v);
        vector<int> want(n + 1), wsz(n + 1);
        for (int u = 1; u <= n; u++) {
            set<int> s;
            int cnt = 0;
            for (int v = 1; v <= n; v++) {
                bool in = false;
                for (int x = v; x; x = par[x]) in |= x == u;
                if (in) s.insert(D.col[v]), cnt++;
            }
            want[u] = s.size(), wsz[u] = cnt;
        }
        auto ans = D.run(root);
        bool ok = true, szok = true;
        for (int u = 1; u <= n; u++) ok &= ans[u] == want[u], szok &= D.sz[u] == wsz[u];
        chk(root == 1 ? "run(root=1)" : "run(root!=1)", ok);
        chk("sz[] (dfs)", szok);
        // run keeps the root's counts (keep = 1), so cur = ans[root]
        chk("cur after run == ans[root]", D.cur == ans[root]);
        bool bigclear = true;
        for (int u = 1; u <= n; u++) bigclear &= D.big[u] == 0;
        chk("big[] reset after run", bigclear);
        // rerun with new colours on the same object
        for (int i = 1; i <= n; i++) D.col[i] = rng() % 3;
        auto ans2 = D.run(root);
        bool ok2 = true;
        for (int u = 1; u <= n; u++) {
            set<int> s;
            for (int v = 1; v <= n; v++) {
                bool in = false;
                for (int x = v; x; x = par[x]) in |= x == u;
                if (in) s.insert(D.col[v]);
            }
            ok2 &= ans2[u] == (int)s.size();
        }
        chk("run (rerun same object)", ok2);
    }
    {
        DSU_On_Tree D(1);
        D.col[1] = 5;
        chk("n=1", D.run()[1] == 1);
    }
    for (int shape = 0; shape < 2; shape++) {
        int n = 200000;
        DSU_On_Tree D(n);
        for (int i = 2; i <= n; i++) D.add_edge(i, shape ? i - 1 : (int)(rng() % (i - 1)) + 1);
        for (int i = 1; i <= n; i++) D.col[i] = rng() % n;
        auto t0 = clock();
        auto a = D.run(1);
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("%s n=2e5: %.3fs (ans[1]=%d)\n", shape ? "path" : "random", t, a[1]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
