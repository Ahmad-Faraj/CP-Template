// Test for 03_Trees/Small_To_Large.cpp (Small To Large) against walking each subtree into a set.
// Every public method is checked: add_edge, colors[] (one and several values per node), run(root),
// and the distinct_num member it fills.
#include "../../03_Trees/Small_To_Large.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 12 + 1, shape = rng() % 3;
        int range = vector<int>{1, 3, 10, 1000000000}[rng() % 4];
        Small_To_Large S(n);
        vector<vector<int>> g(n + 1);
        for (int v = 2; v <= n; v++) {
            int u = shape == 0 ? v - 1 : shape == 1 ? 1 : (int)(rng() % (v - 1)) + 1;
            S.add_edge(u, v), g[u].push_back(v), g[v].push_back(u);
        }
        bool multi = rng() % 3 == 0;
        vector<vector<int>> val(n + 1);
        for (int u = 1; u <= n; u++) {
            int k = multi ? rng() % 3 : 1;
            for (int j = 0; j < k; j++) {
                int c = (int)(rng() % range) - (rng() % 2 ? range / 2 : 0); // negatives too
                val[u].push_back(c), S.colors[u].insert(c);
            }
        }
        int root = rng() % n + 1;
        vector<int> par(n + 1, 0), seen(n + 1, 0), q = {root};
        seen[root] = 1;
        for (int i = 0; i < (int)q.size(); i++)
            for (int v : g[q[i]])
                if (!seen[v]) seen[v] = 1, par[v] = q[i], q.push_back(v);
        auto d = S.run(root);
        bool ok = (int)d.size() == n + 1;
        for (int u = 1; ok && u <= n; u++) {
            set<int> s;
            for (int x = 1; x <= n; x++) {
                int y = x;
                while (y && y != u) y = par[y];
                if (y == u) s.insert(val[x].begin(), val[x].end());
            }
            ok &= d[u] == (int)s.size() && S.distinct_num[u] == d[u];
        }
        chk(multi ? "run (several values per node)" : "run (one colour per node)", ok);
    }
    for (int shape = 0; shape < 2; shape++) { // timing: random tree and a path (deep recursion)
        int n = 200000;
        Small_To_Large S(n);
        for (int v = 2; v <= n; v++) S.add_edge(shape ? v - 1 : (int)(rng() % (v - 1)) + 1, v);
        for (int u = 1; u <= n; u++) S.colors[u].insert(rng() % n);
        auto t0 = clock();
        auto d = S.run(1);
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5 %s: %.3fs (distinct at root=%d)\n", shape ? "path" : "random", sec, d[1]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-30s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
