// Test for 04_Graphs/Hopcroft_Karp.cpp against a bitmask DP over the right side.
// Every public method is checked: add_edge, maximum_matching (also re-called after more edges),
// pairs, and the l[] / r[] arrays; the matching is verified to be valid and of maximum size.
#include "../../04_Graphs/Hopcroft_Karp.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(13);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

int brute(int n, int m, const vector<vector<int>> &adj) { // f[mask] over left nodes in order
    vector<int> f(1 << m, -1);
    f[0] = 0;
    for (int u = 1; u <= n; u++) {
        vector<int> g = f;
        for (int mask = 0; mask < (1 << m); mask++)
            if (f[mask] >= 0)
                for (int v : adj[u])
                    if (!(mask >> (v - 1) & 1)) {
                        int nm = mask | 1 << (v - 1);
                        g[nm] = max(g[nm], f[mask] + 1);
                    }
        f = g;
    }
    return *max_element(f.begin(), f.end());
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 9, m = rng() % 9, e = rng() % 25;
        HopcroftKarp M(n, m);
        vector<vector<int>> adj(n + 1);
        set<pair<int, int>> E;
        auto add = [&](int k) {
            for (int i = 0; i < k && n && m; i++) {
                int u = rng() % n + 1, v = rng() % m + 1;
                M.add_edge(u, v), adj[u].push_back(v), E.insert({u, v});
            }
        };
        add(e);
        for (int round = 0; round < 2; round++) {
            int want = brute(n, m, adj), got = M.maximum_matching();
            chk(round ? "maximum_matching (re-call)" : "maximum_matching", got == want);
            auto p = M.pairs();
            set<int> L, R;
            bool ok = (int)p.size() == got;
            for (auto [u, v] : p) {
                ok &= E.count({u, v}) && L.insert(u).second && R.insert(v).second;
                ok &= M.l[u] == v + n && M.r[v + n] == u;
            }
            chk("pairs valid", ok);
            int matched = 0;
            for (int u = 1; u <= n; u++) matched += M.l[u] != 0;
            chk("l / r arrays", matched == got);
            add(rng() % 5);
        }
    }
    for (int k = 0; k < 2; k++) { // timing: random, and a dense-ish block that needs many phases
        int n = 100000, m = 100000;
        HopcroftKarp M(n, m);
        if (k == 0)
            for (int i = 0; i < 500000; i++) M.add_edge(rng() % n + 1, rng() % m + 1);
        else
            for (int u = 1; u <= n; u++) { // greedy takes u -> u + 1, so the 2nd phase must
                if (u < m) M.add_edge(u, u + 1); // augment n -> n, n-1 -> n-1, ..., 1 -> 1
                M.add_edge(u, u);
            }
        auto t0 = clock();
        int r = M.maximum_matching();
        double sec = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("n=m=1e5 %s: %.3fs (matching=%d)\n", k ? "chain" : "random E=5e5", sec, r);
        if (k) chk("maximum_matching", r == n);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-27s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
