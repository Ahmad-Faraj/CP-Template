// Test for 04_Graphs/Bipartite_Check.cpp against trying all 2^n colourings.
// Every public method is checked: add_edge, run (verdict, colouring validity, smallest node of each
// component gets colour 1, rerun on the same object), bfs (one component directly).
#include "../../04_Graphs/Bipartite_Check.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(31);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 11 + 1, m = rng() % (2 * n);
        vector<pair<int, int>> e;
        Bipartite_Check B(n);
        bool loops = rng() % 5 == 0;
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            if (u == v && !loops) continue;
            e.push_back({u, v}), B.add_edge(u, v);
            if (rng() % 6 == 0) e.push_back({u, v}), B.add_edge(v, u); // multi-edge
        }
        bool want = false;
        for (int mask = 0; mask < (1 << n) && !want; mask++) {
            bool ok = true;
            for (auto [u, v] : e) ok &= ((mask >> (u - 1)) & 1) != ((mask >> (v - 1)) & 1);
            want = ok;
        }
        auto c = B.run();
        chk(want ? "run(bipartite)" : "run(not bipartite)", !c.empty() == want);
        if (!c.empty()) {
            bool ok = (int)c.size() == n + 1;
            for (int v = 1; ok && v <= n; v++) ok &= c[v] == 1 || c[v] == 2;
            for (auto [u, v] : e) ok &= c[u] != c[v];
            // smallest node of each component is coloured 1 (brute components by repeated relaxation)
            vector<int> comp(n + 1);
            iota(comp.begin(), comp.end(), 0);
            for (int r = 0; r < n; r++)
                for (auto [u, v] : e) comp[u] = comp[v] = min(comp[u], comp[v]);
            for (int v = 1; ok && v <= n; v++)
                if (comp[v] == v) ok &= c[v] == 1;
            chk("run(colouring valid)", ok);
            chk("run(rerun same object)", B.run() == c);
        }
        {   // bfs on node 1's component alone
            Bipartite_Check b2(n);
            for (auto [u, v] : e) b2.add_edge(u, v);
            vector<int> comp(n + 1);
            iota(comp.begin(), comp.end(), 0);
            for (int r = 0; r < n; r++)
                for (auto [u, v] : e) comp[u] = comp[v] = min(comp[u], comp[v]);
            bool w1 = false; // brute on node 1's component only
            for (int mask = 0; mask < (1 << n) && !w1; mask++) {
                bool ok = true;
                for (auto [u, v] : e)
                    if (comp[u] == 1) ok &= ((mask >> (u - 1)) & 1) != ((mask >> (v - 1)) & 1);
                w1 = ok;
            }
            bool got = b2.bfs(1);
            bool touched = true;
            for (int v = 1; v <= n; v++) touched &= (b2.color[v] != 0) == (comp[v] == 1) || !got;
            chk("bfs(single component)", got == w1 && touched);
        }
    }
    {
        int n = 200000, m = 400000;
        Bipartite_Check B(n);
        for (int i = 0; i < m; i++) { // edges between odd and even nodes only: bipartite
            int u = 2 * (rng() % (n / 2)) + 1, v = 2 * (rng() % (n / 2)) + 2;
            B.add_edge(u, v);
        }
        auto t0 = clock();
        auto c = B.run();
        chk("large(bipartite)", !c.empty());
        B.add_edge(1, 3), B.add_edge(3, 2), B.add_edge(2, 1);
        chk("large(triangle)", B.run().empty());
        printf("n=2e5 m=4e5, two runs: %.3fs\n", double(clock() - t0) / CLOCKS_PER_SEC);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
