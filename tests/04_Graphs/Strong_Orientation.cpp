// Test for 04_Graphs/Strong_Orientation.cpp against trying all 2^m orientations.
// Every public method is checked: add_edge (returned ids), run() (= the true minimum SCC count),
// orient (its SCC count equals run()), comp_cnt and bridge_cnt against brute force; run() twice.
#include "../../04_Graphs/Strong_Orientation.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(29);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int scc_count(int n, const vector<pair<int, int>> &arcs) { // via transitive closure
    vector<vector<bool>> r(n, vector<bool>(n, false));
    for (int i = 0; i < n; i++) r[i][i] = true;
    for (auto [a, b] : arcs) r[a][b] = true;
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (r[i][k] && r[k][j]) r[i][j] = true;
    int c = 0;
    for (int v = 0; v < n; v++) {
        bool first = true;
        for (int u = 0; u < v; u++) first &= !(r[u][v] && r[v][u]);
        c += first;
    }
    return c;
}

int components(int n, const vector<pair<int, int>> &es, int skip) {
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    int c = n;
    for (int i = 0; i < (int)es.size(); i++)
        if (i != skip && f(es[i].first) != f(es[i].second))
            p[f(es[i].first)] = f(es[i].second), c--;
    return c;
}

int main() {
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 6 + 1, m = rng() % 10;
        Strong_Orientation S(n);
        vector<pair<int, int>> es;
        bool ids = true;
        for (int i = 0; i < m; i++) {
            int a = rng() % n, b = rng() % n; // self loops and multi-edges allowed
            ids &= S.add_edge(a, b) == i;
            es.push_back({a, b});
        }
        chk("add_edge", ids);
        int best = INT_MAX;
        for (int mask = 0; mask < (1 << m); mask++) {
            vector<pair<int, int>> arcs;
            for (int i = 0; i < m; i++)
                arcs.push_back(mask >> i & 1 ? make_pair(es[i].second, es[i].first) : es[i]);
            best = min(best, scc_count(n, arcs));
        }
        int comps = components(n, es, -1), bridges = 0;
        for (int i = 0; i < m; i++) bridges += components(n, es, i) > comps;
        for (int rep = 0; rep < 2; rep++) {
            int got = S.run();
            chk("run", got == best);
            chk("comp_cnt / bridge_cnt", S.comp_cnt == comps && S.bridge_cnt == bridges);
            bool ok = (int)S.orient.size() == m;
            vector<pair<int, int>> arcs;
            for (int i = 0; ok && i < m; i++) {
                ok &= S.orient[i] == '>' || S.orient[i] == '<';
                arcs.push_back(S.orient[i] == '<' ? make_pair(es[i].second, es[i].first) : es[i]);
            }
            chk("orient achieves run()", ok && scc_count(n, arcs) == got);
        }
    }
    { // timing: n = 2e5, m = 4e5 (random edges over a spanning path, deep recursion)
        int n = 200000, m = 400000;
        Strong_Orientation S(n);
        for (int v = 1; v < n; v++) S.add_edge(v - 1, v);
        for (int i = n - 1; i < m; i++) S.add_edge(rng() % n, rng() % n);
        auto t0 = clock();
        int r = S.run();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=2e5 m=4e5: %.3fs (min SCCs=%d, bridges=%d)\n", sec, r, S.bridge_cnt);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-22s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
