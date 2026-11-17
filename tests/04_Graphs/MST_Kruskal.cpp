// Test for 04_Graphs/MST_Kruskal.cpp against brute force over all edge subsets.
// Every public method is checked: constructor, add_edge, build (min and max), is_spanning,
// component_count, cost, edge_ids, tree, find, join.
#include "../../04_Graphs/MST_Kruskal.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(4242);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct E { int u, v; ll w; };

int comps(int n, const vector<E> &es, int mask) {
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    int c = n;
    for (int i = 0; i < (int)es.size(); i++)
        if (mask >> i & 1) {
            int a = f(es[i].u), b = f(es[i].v);
            if (a != b) p[a] = b, c--;
        }
    return c;
}
bool is_forest(int n, const vector<E> &es, const vector<int> &ids) {
    vector<int> p(n + 1);
    iota(p.begin(), p.end(), 0);
    function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
    for (int id : ids) {
        int a = f(es[id].u), b = f(es[id].v);
        if (a == b) return false;
        p[a] = b;
    }
    return true;
}

int main() {
    for (int it = 0; it < 15000; it++) {
        int n = rng() % 6 + 1, m = rng() % 11;
        vector<E> es;
        for (int i = 0; i < m; i++) es.push_back({(int)(rng() % n + 1), (int)(rng() % n + 1), (ll)(rng() % 21) - 5});
        if (rng() % 8 == 0)
            for (auto &e : es) e.w = (ll)(rng() % 2000000000) * 2000000; // past int range
        int full = comps(n, es, (1 << m) - 1);
        for (int maximum = 0; maximum < 2; maximum++) {
            // brute: among subsets that are forests with n - full edges (spanning forest), best weight
            ll best = maximum ? LLONG_MIN : LLONG_MAX;
            for (int mask = 0; mask < (1 << m); mask++) {
                if (__builtin_popcount(mask) != n - full || comps(n, es, mask) != full) continue;
                ll s = 0;
                for (int i = 0; i < m; i++)
                    if (mask >> i & 1) s += es[i].w;
                best = maximum ? max(best, s) : min(best, s);
            }
            if (n - full == 0) best = 0;
            MST_Kruskal k(n);
            for (auto &e : es) k.add_edge(e.u, e.v, e.w);
            string t = maximum ? "max:" : "min:";
            chk(t + "build", k.build(maximum) == best);
            chk(t + "cost", k.cost() == best);
            chk(t + "is_spanning", k.is_spanning() == (full == 1));
            chk(t + "component_count", k.component_count() == full);
            auto ids = k.edge_ids();
            ll s = 0;
            for (int id : ids) s += es[id].w;
            chk(t + "edge_ids", (int)ids.size() == n - full && s == best && is_forest(n, es, ids));
            auto adj = k.tree();
            multiset<tuple<int, int, ll>> want, got;
            for (int id : ids) want.insert({min(es[id].u, es[id].v), max(es[id].u, es[id].v), es[id].w});
            for (int u = 1; u <= n; u++)
                for (auto [v, w] : adj[u])
                    if (u <= v) got.insert({u, v, w});
            // a self loop is never chosen, so each tree edge appears once with u <= v (u < v)
            chk(t + "tree", got == want);
            // find / join after build: everything in a component shares a root
            bool ok = true;
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++) {
                    int mask = (1 << m) - 1;
                    (void)mask;
                }
            vector<int> p(n + 1);
            iota(p.begin(), p.end(), 0);
            function<int(int)> f = [&](int x) { return p[x] == x ? x : p[x] = f(p[x]); };
            for (auto &e : es) p[f(e.u)] = f(e.v);
            for (int u = 1; u <= n; u++)
                for (int v = 1; v <= n; v++) ok &= (k.find(u) == k.find(v)) == (f(u) == f(v));
            chk(t + "find", ok);
            int u = rng() % n + 1, v = rng() % n + 1;
            chk(t + "join", k.join(u, v) == (f(u) != f(v)));
        }
    }
    {
        int n = 200000, m = 200000;
        MST_Kruskal k(n);
        for (int i = 1; i < n; i++) k.add_edge(i, i + 1, rng() % 1000000000);
        for (int i = 0; i < m; i++) k.add_edge(rng() % n + 1, rng() % n + 1, rng() % 1000000000);
        auto t0 = clock();
        k.build();
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("large n=m=2e5: %.3fs\n", t);
        chk("large_spanning", k.is_spanning());
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-24s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
