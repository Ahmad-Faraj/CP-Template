// Test for 04_Graphs/Floyd_Warshall.cpp against Bellman-Ford style relaxation from every source.
// Every public method is checked: constructor, add_edge (directed and bidirectional), build, get, is_inf,
// has_negative_cycle, affected_by_negative_cycle, path, add_edge_after_build.
#include "../../04_Graphs/Floyd_Warshall.cpp"

map<string, pair<long long, long long>> cov;
mt19937 rng(777);
void chk(const string &m, bool ok) { cov[m].first++, cov[m].second += !ok; }

const ll BIG = LLONG_MAX / 4, FLOOR = -(1LL << 50);
struct E { int u, v; ll w; };

// best[u][v] after `rounds` rounds of relaxing every edge from source u (clamped below FLOOR).
vector<vector<ll>> relax_all(int n, const vector<E> &es, int rounds) {
    vector<vector<ll>> d(n + 1, vector<ll>(n + 1, BIG));
    for (int s = 1; s <= n; s++) {
        d[s][s] = 0;
        for (int r = 0; r < rounds; r++)
            for (auto &e : es)
                if (d[s][e.u] < BIG) d[s][e.v] = min(d[s][e.v], max(FLOOR, d[s][e.u] + e.w));
    }
    return d;
}

void one_case(int n, vector<E> es, int extra_after_build) {
    // es: directed edges (bidirectional ones are already expanded). The last `extra_after_build`
    // logical additions are applied with add_edge_after_build.
    Floyd<ll> f(n);
    vector<vector<ll>> w(n + 1, vector<ll>(n + 1, BIG));
    for (int i = 1; i <= n; i++) w[i][i] = 0;
    int split = (int)es.size() - extra_after_build;
    for (int i = 0; i < split; i++) f.add_edge(es[i].u, es[i].v, es[i].w), w[es[i].u][es[i].v] = min(w[es[i].u][es[i].v], es[i].w);
    f.build();
    for (int i = split; i < (int)es.size(); i++) f.add_edge_after_build(es[i].u, es[i].v, es[i].w), w[es[i].u][es[i].v] = min(w[es[i].u][es[i].v], es[i].w);
    string tag = extra_after_build ? "after_build:" : "";

    auto exact = relax_all(n, es, n);          // exact when no negative cycle interferes
    auto deep = relax_all(n, es, 4 * n + 4);   // negative closed walks show up as further decrease
    vector<bool> negk(n + 1);
    bool anyneg = false;
    for (int k = 1; k <= n; k++) negk[k] = deep[k][k] < 0, anyneg |= negk[k];
    // add_edge_after_build is only exact while there is no negative cycle (documented)
    if (extra_after_build && anyneg) {
        chk(tag + "has_negative_cycle", f.has_negative_cycle());
        return;
    }
    chk(tag + "has_negative_cycle", f.has_negative_cycle() == anyneg);
    for (int u = 1; u <= n; u++)
        for (int v = 1; v <= n; v++) {
            bool aff = false;
            for (int k = 1; k <= n; k++) aff |= negk[k] && exact[u][k] < BIG && exact[k][v] < BIG;
            chk(tag + "affected_by_negative_cycle", f.affected_by_negative_cycle(u, v) == aff);
            if (aff) continue;
            bool unreach = exact[u][v] >= BIG;
            chk(tag + "is_inf", Floyd<ll>::is_inf(f.get(u, v)) == unreach);
            if (unreach) {
                chk(tag + "path", f.path(u, v).empty());
                continue;
            }
            chk(tag + "get", f.get(u, v) == exact[u][v]);
            auto p = f.path(u, v);
            bool ok = !p.empty() && p.front() == u && p.back() == v;
            ll s = 0;
            for (int i = 0; ok && i + 1 < (int)p.size(); i++) {
                ok &= w[p[i]][p[i + 1]] < BIG;
                if (ok) s += w[p[i]][p[i + 1]];
            }
            chk(tag + "path", ok && s == exact[u][v]);
        }
}

int main() {
    for (int it = 0; it < 6000; it++) {
        int n = rng() % 7 + 1, m = rng() % 14;
        bool allow_neg = rng() % 3 == 0;
        vector<E> es;
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            ll w = allow_neg ? (ll)(rng() % 25) - 6 : (ll)(rng() % 20);
            if (rng() % 10 == 0) w = (ll)(rng() % 1000000000) * 1000; // large weights
            if (rng() % 3 == 0) { // bidirectional: test the flag on a separate object below
                es.push_back({u, v, w});
                es.push_back({v, u, w});
            } else es.push_back({u, v, w});
        }
        one_case(n, es, 0);
        if (!es.empty()) one_case(n, es, rng() % min<int>(3, es.size()) + 1);
    }
    // bidirectional flag: add_edge(u, v, w, true) must equal two directed edges
    for (int it = 0; it < 3000; it++) {
        int n = rng() % 6 + 1, m = rng() % 10;
        Floyd<ll> a(n), b(n);
        for (int i = 0; i < m; i++) {
            int u = rng() % n + 1, v = rng() % n + 1;
            ll w = rng() % 30;
            a.add_edge(u, v, w, true);
            b.add_edge(u, v, w), b.add_edge(v, u, w);
        }
        a.build(), b.build();
        bool ok = true;
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) ok &= a.get(u, v) == b.get(u, v);
        chk("add_edge(bidirectional)", ok);
        int u = rng() % n + 1, v = rng() % n + 1;
        ll w = rng() % 30;
        a.add_edge_after_build(u, v, w, true);
        b.add_edge_after_build(u, v, w), b.add_edge_after_build(v, u, w);
        ok = true;
        for (int x = 1; x <= n; x++)
            for (int y = 1; y <= n; y++) ok &= a.get(x, y) == b.get(x, y);
        chk("add_edge_after_build(bidirectional)", ok);
    }
    // overflow guard: complete digraph, every weight -1 (the case that broke the unclamped version)
    {
        int n = 30;
        Floyd<ll> f(n);
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++)
                if (u != v) f.add_edge(u, v, -1);
        f.build();
        bool ok = f.has_negative_cycle();
        for (int u = 1; u <= n; u++)
            for (int v = 1; v <= n; v++) ok &= f.affected_by_negative_cycle(u, v) && f.get(u, v) < 0;
        chk("negative_cycle_no_overflow", ok);
    }
    // timing: n = 400
    {
        int n = 400;
        Floyd<ll> f(n);
        for (int i = 0; i < 4 * n; i++) f.add_edge(rng() % n + 1, rng() % n + 1, rng() % 1000);
        auto t0 = clock();
        f.build();
        double t = double(clock() - t0) / CLOCKS_PER_SEC;
        printf("build n=400: %.3fs\n", t);
    }
    bool ok = true;
    for (auto &[m, c] : cov) {
        printf("coverage %-40s checks=%-9lld mismatches=%lld\n", m.c_str(), c.first, c.second);
        ok &= c.second == 0;
    }
    puts(ok ? "RESULT: PASS" : "RESULT: FAIL");
    return ok ? 0 : 1;
}
