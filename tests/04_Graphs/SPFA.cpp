// Test for 04_Graphs/SPFA.cpp against a plain n-round Bellman-Ford.
// Every public method is checked: add_edge (directed and bidirectional), shortest_path(src),
// shortest_path(src, dest), has_negative_cycle, path, is_inf, neg_cycle (and its reset between runs).
#include "../../04_Graphs/SPFA.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(17);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

struct E { int u, v; ll w; };
const ll BIG = LLONG_MAX / 4;

// returns {dist, reachable negative cycle}
pair<vector<ll>, bool> brute(int n, const vector<E> &es, int src) {
    vector<ll> d(n + 1, BIG);
    d[src] = 0;
    for (int r = 0; r < n; r++)
        for (auto &e : es)
            if (d[e.u] < BIG && d[e.u] + e.w < d[e.v]) d[e.v] = d[e.u] + e.w;
    bool neg = false;
    for (auto &e : es)
        if (d[e.u] < BIG && d[e.u] + e.w < d[e.v]) neg = true;
    return {d, neg};
}

int main() {
    for (int it = 0; it < 30000; it++) {
        int n = rng() % 7 + 1, m = rng() % 14;
        int lo = vector<int>{0, -2, -10}[rng() % 3];
        vector<E> es;
        SPFA<ll> sp(n);
        bool any_bi = false;
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            ll w = lo + (ll)(rng() % 21);
            bool bi = rng() % 4 == 0;
            sp.add_edge(u, v, w, bi);
            es.push_back({u, v, w});
            if (bi) es.push_back({v, u, w}), any_bi = true;
        }
        for (int src = 1; src <= n; src++) {
            auto [d, neg] = brute(n, es, src);
            string t = any_bi ? "bi:" : "dir:";
            auto got = sp.shortest_path(src);
            chk(t + "neg_cycle", sp.neg_cycle == neg);
            chk(t + "has_negative_cycle", sp.has_negative_cycle(src) == neg);
            if (neg) {
                chk(t + "path(neg)", sp.path(src, rng() % n + 1).empty());
                continue;
            }
            bool ok = true;
            for (int v = 1; v <= n; v++) ok &= (d[v] == BIG) ? SPFA<ll>::is_inf(got[v]) : got[v] == d[v];
            chk(t + "shortest_path(src)", ok);
            int dst = rng() % n + 1;
            ll one = sp.shortest_path(src, dst);
            chk(t + "shortest_path(src,dest)", d[dst] == BIG ? SPFA<ll>::is_inf(one) : one == d[dst]);
            chk(t + "neg_cycle reset", !sp.neg_cycle);
            auto p = sp.path(src, dst);
            if (d[dst] == BIG) chk(t + "path(unreachable)", p.empty());
            else {
                ll s = 0;
                bool valid = !p.empty() && p.front() == src && p.back() == dst;
                for (int i = 0; valid && i + 1 < (int)p.size(); i++) {
                    ll best = BIG;
                    for (auto &e : es)
                        if (e.u == p[i] && e.v == p[i + 1]) best = min(best, e.w);
                    valid = best < BIG, s += best;
                }
                chk(t + "path", valid && s == d[dst]);
            }
        }
    }
    {
        SPFA<int> sp(3);
        sp.add_edge(1, 2, -5), sp.add_edge(2, 3, 7);
        chk("int:shortest_path", sp.shortest_path(1, 3) == 2 && SPFA<int>::is_inf(sp.shortest_path(3, 1)));
    }
    {
        int n = 200000, m = 400000;
        SPFA<ll> sp(n);
        for (int i = 1; i < n; i++) sp.add_edge(i, i + 1, rng() % 1000);
        for (int i = 0; i < m; i++) sp.add_edge(rng() % n + 1, rng() % n + 1, rng() % 1000000);
        auto t0 = clock();
        auto d = sp.shortest_path(1);
        printf("random n=2e5 m=6e5: %.3fs (d[n]=%lld)\n", double(clock() - t0) / CLOCKS_PER_SEC, d[n]);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-28s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
