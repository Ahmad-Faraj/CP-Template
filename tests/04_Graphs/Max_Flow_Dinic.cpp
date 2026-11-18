// Test for 04_Graphs/Max_Flow_Dinic.cpp against brute force over all 2^n s-t cuts.
// Every public method is checked: add_edge (with and without id), max_flow (value = min cut,
// flow_through feasible + conserved, a second call after more edges), get_min_cut (s side,
// edges listed, capacity = flow), get_paths (count = flow, valid walks, capacity respected).
#include "../../04_Graphs/Max_Flow_Dinic.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(23);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct E { int u, v; long long c; };

long long brute_cut(int n, const vector<E> &es, int s, int t) {
    long long best = LLONG_MAX;
    for (int mask = 0; mask < (1 << n); mask++) {
        if (!(mask >> s & 1) || (mask >> t & 1)) continue;
        long long c = 0;
        for (auto &e : es)
            if ((mask >> e.u & 1) && !(mask >> e.v & 1)) c += e.c;
        best = min(best, c);
    }
    return best;
}

int main() {
    for (int it = 0; it < 40000; it++) {
        int n = rng() % 7 + 2, m = rng() % 15;
        int base = rng() % 2; // 0- or 1-based node labels
        long long cmax = vector<long long>{1, 3, 20, 4000000000000LL}[rng() % 4];
        Dinic F(n + base);
        vector<E> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            long long c = rng() % (cmax + 1);
            if (cmax > 100) c = (long long)(rng() % 1000) * (cmax / 1000);
            es.push_back({u, v, c});
            F.add_edge(u + base, v + base, c, i);
        }
        if (rng() % 3 == 0) F.add_edge(rng() % n + base, rng() % n + base, 0); // no id
        int s = rng() % n, t = rng() % n;
        while (t == s) t = rng() % n;
        long long want = brute_cut(n, es, s, t);
        long long got = F.max_flow(s + base, t + base);
        chk("max_flow value", got == want);
        // flow_through: bounds and conservation
        bool ok = true;
        vector<long long> net(n, 0);
        for (int i = 0; i < m; i++) {
            long long f = F.flow_through[i];
            ok &= 0 <= f && f <= es[i].c;
            net[es[i].u] -= f, net[es[i].v] += f;
        }
        for (int v = 0; v < n; v++)
            if (v != s && v != t) ok &= net[v] == 0;
        ok &= net[t] == got && net[s] == -got;
        chk("flow_through feasible", ok);
        // min cut
        auto cut = F.get_min_cut();
        ok = F.vis[s + base] && !F.vis[t + base];
        multiset<pair<int, int>> exp_cut, got_cut(cut.begin(), cut.end());
        long long cap = 0;
        for (auto &e : es)
            if (F.vis[e.u + base] && !F.vis[e.v + base]) {
                cap += e.c;
                if (e.c > 0) exp_cut.insert({e.u + base, e.v + base});
            }
        ok &= cap == got && exp_cut == got_cut;
        chk("get_min_cut", ok);
        // paths (only for small capacities: one path per unit)
        if (cmax <= 20) {
            auto paths = F.get_paths();
            ok = (long long)paths.size() == got;
            map<pair<int, int>, long long> capuv, used;
            for (auto &e : es) capuv[{e.u + base, e.v + base}] += e.c;
            for (auto &p : paths) {
                ok &= p.front() == s + base && p.back() == t + base;
                for (int i = 0; i + 1 < (int)p.size(); i++) used[{p[i], p[i + 1]}]++;
            }
            for (auto &[k, c] : used) ok &= capuv.count(k) && c <= capuv[k];
            chk(cmax == 1 ? "get_paths (unit caps)" : "get_paths (caps>1)", ok);
        }
    }
    // undirected edges (two arcs) and a second max_flow call after adding edges
    for (int it = 0; it < 20000; it++) {
        int n = rng() % 6 + 2, m = rng() % 10;
        Dinic F(n);
        vector<E> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = rng() % n;
            long long c = rng() % 6;
            es.push_back({u, v, c}), es.push_back({v, u, c});
            F.add_edge(u, v, c), F.add_edge(v, u, c);
        }
        int s = rng() % n, t = (s + 1 + rng() % (n - 1)) % n;
        long long f1 = F.max_flow(s, t);
        chk("max_flow undirected", f1 == brute_cut(n, es, s, t));
        int extra = rng() % 5;
        for (int i = 0; i < extra; i++) {
            int u = rng() % n, v = rng() % n;
            long long c = rng() % 6;
            es.push_back({u, v, c});
            F.add_edge(u, v, c);
        }
        long long f2 = F.max_flow(s, t);
        chk("max_flow second call adds", f1 + f2 == brute_cut(n, es, s, t));
    }
    // samples: Library Checker bipartitematching example_00 (answer 3), CSES-style tiny graph
    {
        int L = 4, R = 4;
        vector<pair<int, int>> ed = {{1, 1}, {2, 2}, {0, 0}, {3, 1}, {1, 2}, {2, 0}, {3, 2}};
        Dinic F(L + R + 2);
        int S = L + R, T = S + 1;
        for (int i = 0; i < L; i++) F.add_edge(S, i, 1);
        for (int j = 0; j < R; j++) F.add_edge(L + j, T, 1);
        for (int i = 0; i < (int)ed.size(); i++) F.add_edge(ed[i].first, L + ed[i].second, 1, i);
        long long k = F.max_flow(S, T);
        set<int> ls, rs;
        int cnt = 0;
        for (int i = 0; i < (int)ed.size(); i++)
            if (F.flow_through[i]) cnt++, ls.insert(ed[i].first), rs.insert(ed[i].second);
        chk("samples", k == 3 && cnt == 3 && ls.size() == 3 && rs.size() == 3);
    }
    {
        int L = 100000, R = 100000, M = 200000;
        Dinic F(L + R + 2);
        int S = L + R, T = S + 1;
        for (int i = 0; i < L; i++) F.add_edge(S, i, 1);
        for (int j = 0; j < R; j++) F.add_edge(L + j, T, 1);
        for (int i = 0; i < M; i++) F.add_edge(rng() % L, L + rng() % R, 1);
        auto t0 = clock();
        long long k = F.max_flow(S, T);
        printf("bipartite L=R=1e5 M=2e5: %.3fs (matching=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, k);
        int n = 5000, m = 100000;
        Dinic G(n);
        for (int i = 0; i < m; i++) {
            int u = rng() % n, v = min(n - 1, u + 1 + (int)(rng() % 30));
            G.add_edge(u, v, rng() % 1000000000);
        }
        t0 = clock();
        k = G.max_flow(0, n - 1);
        printf("layered n=5000 m=1e5 caps<=1e9: %.3fs (flow=%lld)\n",
               double(clock() - t0) / CLOCKS_PER_SEC, k);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
